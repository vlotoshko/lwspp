/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#include <iostream>
#include <vector>

#include "easywebsocket/client/IEventHandler.hpp"
#include "LwsAdapter/ILwsCallbackContext.hpp"
#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsSession.hpp"
#include "SessionInfo.hpp"

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

auto sendMessage(lws* wsInstance, const Message& message) -> bool
{
    // C-style cast to convert from const char* to unsigned char*
    auto* messageBegin = (unsigned char*)(message.second.data() + LWS_PRE);
    const int expectedSize = static_cast<int>(message.second.size() - LWS_PRE);

    auto writeProtocol = message.first == MessageType::Text ? LWS_WRITE_TEXT : LWS_WRITE_BINARY;
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
            auto& messages = session->getMessages();
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
            eventHandler->onDataReceive(std::vector<char>{inAsChar, inAsChar + len}, bytesRemains);
        }
        else
        {
            eventHandler->onMessageReceive(std::string{inAsChar, len}, bytesRemains);
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

} // namespace ews::cli
