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

#include <iostream>
#include <vector>

#include "lwspp/client/IEventHandler.hpp"
#include "LwsAdapter/ILwsCallbackContext.hpp"
#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsSession.hpp"
#include "SessionInfo.hpp"

namespace lwspp
{
namespace cli
{
namespace
{
// The libwebsockets closes current session if callback returns -1
const int CLOSE_SESSION = -1;

auto getCallbackContext(lws* wsInstance) -> ILwsCallbackContext&
{
    void* contextData = lws_context_user(lws_get_context(wsInstance));
    return *(reinterpret_cast<ILwsCallbackContext *>(contextData));
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

    switch(reason)
    {
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    {
        auto errorMessage = std::string{"CLIENT_CONNECTION_ERROR: "};
        errorMessage.append(in != nullptr ?
                            std::string{reinterpret_cast<const char *>(in), len} : "(null)");

        lwsl_err("%s\n", errorMessage.c_str());
        eventHandler->onError(errorMessage);
        break;
    }
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
    {
        callbackContext.setSession(std::make_shared<LwsSession>(wsInstance));
        eventHandler->onConnect(std::make_shared<SessionInfo>());
        break;
    }
    case LWS_CALLBACK_CLIENT_WRITEABLE:
    {
        if (auto session = callbackContext.getSession())
        {
            auto& messages = session->getPendingData();
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
                    eventHandler->onError("Error writing data to socket");
                }
            }
        }
        else
        {
            // Never should be here
            eventHandler->onWarning("Referring to unknown or deleted session. "
                                    "Dropping connection for this session");
            return CLOSE_SESSION;
        }
        break;
    }
    case LWS_CALLBACK_CLIENT_RECEIVE:
    {
        const auto* inAsChar = reinterpret_cast<const char *>(in);
        const auto bytesRemains = static_cast<size_t>(lws_remaining_packet_payload(wsInstance));
        if (lws_frame_is_binary(wsInstance) == 1)
        {
            eventHandler->onBinaryDataReceive(std::vector<char>{inAsChar, inAsChar + len}, bytesRemains);
        }
        else
        {
            eventHandler->onTextDataReceive(std::string{inAsChar, len}, bytesRemains);
        }
        break;
    }
    case LWS_CALLBACK_CLIENT_CLOSED:
    {
        callbackContext.resetSession();
        eventHandler->onDisconnect();
        break;
    }
    default:
        break;
    }

    return 0;
}

} // namespace cli
} // namespace lwspp
