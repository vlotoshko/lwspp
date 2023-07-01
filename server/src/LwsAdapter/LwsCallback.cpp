/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <iostream>

#include "easywebsocket/server/IEventHandler.hpp"
#include "LwsAdapter/ILwsCallbackContext.hpp"
#include "LwsAdapter/ILwsSessions.hpp"
#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsSession.hpp"
#include "Consts.hpp"
#include "SessionInfoExternal.hpp"

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

auto reasonToString(lws_callback_reasons reason) -> std::string
{
    auto alpha = std::string{" ("}.append(std::to_string(reason)).append(")");

    switch (reason)
    {
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        return std::string{"LWS_CALLBACK_CLIENT_CONNECTION_ERROR"}.append(alpha);
    case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH:
        return std::string{"LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH"}.append(alpha);
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        return std::string{"LWS_CALLBACK_CLIENT_ESTABLISHED"}.append(alpha);
    case LWS_CALLBACK_CLIENT_CLOSED:
        return std::string{"LWS_CALLBACK_CLIENT_CLOSED"}.append(alpha);
    case LWS_CALLBACK_CLIENT_RECEIVE:
        return std::string{"LWS_CALLBACK_CLIENT_RECEIVE"}.append(alpha);
    case LWS_CALLBACK_CLIENT_WRITEABLE:
        return std::string{"LWS_CALLBACK_CLIENT_WRITEABLE"}.append(alpha);
    case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        return std::string{"LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER"}.append(alpha);
    case LWS_CALLBACK_CLOSED_HTTP:
        return std::string{"LWS_CALLBACK_CLOSED_HTTP"}.append(alpha);
    case LWS_CALLBACK_PROTOCOL_INIT:
        return std::string{"LWS_CALLBACK_PROTOCOL_INIT"}.append(alpha);
    case LWS_CALLBACK_PROTOCOL_DESTROY:
        return std::string{"LWS_CALLBACK_PROTOCOL_DESTROY"}.append(alpha);
    case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
        return std::string{"LWS_CALLBACK_FILTER_NETWORK_CONNECTION"}.append(alpha);
    case LWS_CALLBACK_WSI_CREATE:
        return std::string{"LWS_CALLBACK_WSI_CREATE"}.append(alpha);
    case LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED:
        return std::string{"LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED"}.append(alpha);
    case LWS_CALLBACK_HTTP_CONFIRM_UPGRADE:
        return std::string{"LWS_CALLBACK_HTTP_CONFIRM_UPGRADE"}.append(alpha);
    case LWS_CALLBACK_HTTP_BIND_PROTOCOL:
        return std::string{"LWS_CALLBACK_HTTP_BIND_PROTOCOL"}.append(alpha);
    case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
        return std::string{"LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION"}.append(alpha);
    case LWS_CALLBACK_ADD_HEADERS:
        return std::string{"LWS_CALLBACK_ADD_HEADERS"}.append(alpha);
    case LWS_CALLBACK_ESTABLISHED:
        return std::string{"LWS_CALLBACK_ESTABLISHED"}.append(alpha);
    case LWS_CALLBACK_SERVER_WRITEABLE:
        return std::string{"LWS_CALLBACK_SERVER_WRITEABLE"}.append(alpha);
    case LWS_CALLBACK_RECEIVE:
        return std::string{"LWS_CALLBACK_RECEIVE"}.append(alpha);
    case LWS_CALLBACK_GET_THREAD_ID:
        return std::string{"LWS_CALLBACK_GET_THREAD_ID"}.append(alpha);
    case LWS_CALLBACK_EVENT_WAIT_CANCELLED:
        return std::string{"LWS_CALLBACK_EVENT_WAIT_CANCELLED"}.append(alpha);
    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
        return std::string{"LWS_CALLBACK_WS_PEER_INITIATED_CLOSE"}.append(alpha);
    case LWS_CALLBACK_CLOSED:
        return std::string{"LWS_CALLBACK_CLOSED"}.append(alpha);
    case LWS_CALLBACK_WSI_DESTROY:
        return std::string{"LWS_CALLBACK_WSI_DESTROY"}.append(alpha);
    case LWS_CALLBACK_ADD_POLL_FD:
        return std::string{"LWS_CALLBACK_ADD_POLL_FD"}.append(alpha);
    case LWS_CALLBACK_DEL_POLL_FD:
        return std::string{"LWS_CALLBACK_DEL_POLL_FD"}.append(alpha);
    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
        return std::string{"LWS_CALLBACK_CHANGE_MODE_POLL_FD"}.append(alpha);
    case LWS_CALLBACK_LOCK_POLL:
        return std::string{"LWS_CALLBACK_LOCK_POLL"}.append(alpha);
    case LWS_CALLBACK_UNLOCK_POLL:
        return std::string{"LWS_CALLBACK_UNLOCK_POLL"}.append(alpha);
    default:
        return std::string{"Unknown"}.append(alpha);
    }
}

auto sendMessage(lws* wsInstance, const std::string& message) -> bool
{
    std::array<unsigned char, LWS_PRE + MAX_MESSAGE_SIZE> msg{};
    std::copy(message.cbegin(), message.cend(), msg.data() + LWS_PRE);

    const int expectedSize = static_cast<int>(message.size());
    const int actualSize = lws_write(wsInstance, msg.data() + LWS_PRE, expectedSize, LWS_WRITE_TEXT);

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
    std::cout << "LwsCallback reason: " << reasonToString(reason) << std:: endl;
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
            std::make_shared<SessionInfoExternal>(sessionId, getSessionPath(wsInstance));
        eventHandler->onConnect(sessionInfo);
        break;
    }
    case LWS_CALLBACK_SERVER_WRITEABLE:
    {
        auto sessions = callbackContext.getSessions();
        if (auto session = sessions->get(sessionId))
        {
            auto& messages = session->getMessages();
            if (!messages.empty() && !callbackContext.isStopping())
            {
                auto& message = messages.front();
                if (message.size() > MAX_MESSAGE_SIZE)
                {
                    eventHandler->onError(sessionId, "Message exceeds the maximum alowed size");
                }

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
        eventHandler->onMessageReceive(sessionId, std::string{reinterpret_cast<const char *>(in), len});
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
