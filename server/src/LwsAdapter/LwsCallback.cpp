/*
 * lwspp - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "ConnectionInfo.hpp"
#include "Consts.hpp"
#include "LwsAdapter/ILwsCallbackContext.hpp"
#include "LwsAdapter/ILwsConnections.hpp"
#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsConnection.hpp"
#include "lwspp/server/IEventHandler.hpp"

namespace lwspp
{
namespace srv
{
namespace
{
// The libwebsockets closes current connection if callback returns -1
const int CLOSE_SESSION = -1;

auto getCallbackContext(lws* wsInstance) -> ILwsCallbackContext&
{
    void* contextData = lws_context_user(lws_get_context(wsInstance));
    return *(reinterpret_cast<ILwsCallbackContext *>(contextData));
}

auto getConnectionId(lws* wsInstance) -> ConnectionId
{
    return lws_get_socket_fd(wsInstance);
}

auto getConnectionIP(lws* wsInstance) -> IP
{
    const int MAX_IP_SIZE = 40;
    std::array<char, MAX_IP_SIZE> buffer{};
    lws_get_peer_simple(wsInstance, buffer.data(), MAX_IP_SIZE);

    return static_cast<IP>(buffer.data());
}

auto getConnectionPath(lws* wsInstance) -> Path
{
    std::array<char, MAX_PATH_SIZE> buffer{};
    lws_hdr_copy(wsInstance, buffer.data(), MAX_PATH_SIZE, WSI_TOKEN_GET_URI);

    return static_cast<Path>(buffer.data());
}

auto sendMessage(lws* wsInstance, const Message& message) -> bool
{
    // C-style cast to convert from const char* to unsigned char*
    auto* messageBegin = (unsigned char*)(message.second.data() + LWS_PRE);
    const int expectedSize = static_cast<int>(message.second.size() - LWS_PRE);

    auto writeProtocol = message.first == DataType::Text ? LWS_WRITE_TEXT : LWS_WRITE_BINARY;
    const int actualSize = lws_write(wsInstance, messageBegin, expectedSize, writeProtocol);
    return expectedSize == actualSize;
}

} // namespace

auto lwsCallback_v1(
        lws* wsInstance,
        lws_callback_reasons reason,
        void* /*userData*/,
        void* in/*pointer*/,
        size_t len/*length*/)
-> int
{
    auto& callbackContext = getCallbackContext(wsInstance);
    auto eventHandler = callbackContext.getEventHandler();
    auto connectionId = getConnectionId(wsInstance);

    switch(reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
    {
        auto connections = callbackContext.getConnections();
        connections->add(std::make_shared<LwsConnection>(connectionId, wsInstance));

        auto connectionInfo =
            std::make_shared<ConnectionInfo>(connectionId, getConnectionIP(wsInstance),
                                             getConnectionPath(wsInstance));
        eventHandler->onConnect(connectionInfo);
        break;
    }
    case LWS_CALLBACK_SERVER_WRITEABLE:
    {
        auto connections = callbackContext.getConnections();
        if (auto connection = connections->get(connectionId))
        {
            if (connection->markedToClose())
            {
                lws_close_reason(wsInstance, LWS_CLOSE_STATUS_GOINGAWAY, nullptr, 0);
                return CLOSE_SESSION;
            }

            auto& messages = connection->getPendingData();
            if (!messages.empty() && !callbackContext.isStopping())
            {
                auto& message = messages.front();
                if (sendMessage(wsInstance, message))
                {
                    messages.pop();
                    if (!messages.empty())
                    {
                        lws_callback_on_writable(wsInstance);
                    }
                }
                else
                {
                    eventHandler->onError(connectionId, "Error writing data to socket");
                }
            }
        }
        else
        {
            // Never should be here
            eventHandler->onWarning(connectionId, "Referring to unknown or deleted connection. "
                                                  "Dropping this connection");
            return CLOSE_SESSION;
        }
        break;
    }
    case LWS_CALLBACK_RECEIVE:
    {
        const auto* inAsChar = reinterpret_cast<const char *>(in);
        const auto remains = static_cast<size_t>(lws_remaining_packet_payload(wsInstance));

        if (lws_is_first_fragment(wsInstance) != 0)
        {
            eventHandler->onFirstDataPacket(connectionId, len + remains);
        }

        if (lws_frame_is_binary(wsInstance) == 1)
        {
            eventHandler->onBinaryDataReceive(connectionId, DataPacket{inAsChar, len, remains});
        }
        else
        {
            eventHandler->onTextDataReceive(connectionId, DataPacket{inAsChar, len, remains});
        }
        break;
    }
    case LWS_CALLBACK_CLOSED:
    {
        auto connections = callbackContext.getConnections();
        connections->remove(connectionId);
        eventHandler->onDisconnect(connectionId);
        break;
    }
    default:
        break;
    }
    return 0;
}

} // namespace srv
} // namespace lwspp
