/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#include <iostream>

#include "easywebsocket/client/IEventHandler.hpp"
#include "LwsAdapter/ILwsCallbackContext.hpp"
#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsSession.hpp"
#include "Consts.hpp"

namespace ews::cli
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
    std::cout << "cli LwsCallback reason: " << reasonToString(reason) << std:: endl;
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
        eventHandler->onConnect();
        break;
    }
    case LWS_CALLBACK_CLIENT_WRITEABLE:
    {
        if (auto session = callbackContext.getSession())
        {
            auto& messages = session->getMessages();
            if (!messages.empty() && !callbackContext.isStopping())
            {
                auto& message = messages.front();
                if (message.size() > MAX_MESSAGE_SIZE)
                {
                    eventHandler->onError("Message exceeds the maximum alowed size");
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
        eventHandler->onMessageReceive(std::string{reinterpret_cast<const char *>(in), len});
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

} // namespace ews::cli
