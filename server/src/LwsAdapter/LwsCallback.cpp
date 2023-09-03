/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <iostream>

#include "easywebsocket/server/IEventHandler.hpp"
#include "Consts.hpp"
#include "LwsAdapter/ILwsCallbackContext.hpp"
#include "LwsAdapter/ILwsSessions.hpp"
#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsSession.hpp"
#include "SessionInfo.hpp"

namespace ews::srv
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

auto getSessionId(lws* wsInstance) -> SessionId
{
    return lws_get_socket_fd(wsInstance);
}

auto getSessionPath(lws* wsInstance) -> Path
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
    auto sessionId = getSessionId(wsInstance);

    switch(reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
    {
        auto sessions = callbackContext.getSessions();
        sessions->add(std::make_shared<LwsSession>(sessionId, wsInstance));

        auto sessionInfo =
            std::make_shared<SessionInfo>(sessionId, getSessionPath(wsInstance));
        eventHandler->onConnect(sessionInfo);
        break;
    }
    case LWS_CALLBACK_SERVER_WRITEABLE:
    {
        auto sessions = callbackContext.getSessions();
        if (auto session = sessions->get(sessionId))
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
                    eventHandler->onError(sessionId, "Error writing data to socket");
                }
            }
        }
        else
        {
            // Never should be here
            eventHandler->onWarning(sessionId, "Referring to unknown or deleted session. "
                                               "Dropping connection for this session");
            return CLOSE_SESSION;
        }
        break;
    }
    case LWS_CALLBACK_RECEIVE:
    {
        const auto* inAsChar = reinterpret_cast<const char *>(in);
        const auto bytesRemains = static_cast<size_t>(lws_remaining_packet_payload(wsInstance));
        if (lws_frame_is_binary(wsInstance) == 1)
        {
            eventHandler->onBinaryDataReceive(sessionId, std::vector<char>{inAsChar, inAsChar + len}, bytesRemains);
        }
        else
        {
            eventHandler->onTextDataReceive(sessionId, std::string{inAsChar, len}, bytesRemains);
        }
        break;
    }
    case LWS_CALLBACK_CLOSED:
    {
        auto sessions = callbackContext.getSessions();
        sessions->remove(sessionId);
        eventHandler->onDisconnect(sessionId);
        break;
    }
    default:
        break;
    }
    return 0;
}

} // namespace ews::srv
