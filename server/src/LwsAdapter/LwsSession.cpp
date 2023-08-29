/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "LwsAdapter/LwsSession.hpp"

namespace ews::srv
{

LwsSession::LwsSession(SessionId sessionId, LwsInstanceRawPtr instance)
    : _sessionId(sessionId)
    , _wsInstance(instance)
{}

auto LwsSession::getSessionId() const -> SessionId
{
    return _sessionId;
}

auto LwsSession::getLwsInstance() -> LwsInstanceRawPtr
{
    return _wsInstance;
}

void LwsSession::addMessage(const std::string& message)
{
    // Additional space with the size of LWS_PRE should be added in the front of the message
    // Please, read lws_write description for more information
    std::string preparedMessage;

    // NOTE: resize can throw bad alloc if message is too large
    preparedMessage.resize(LWS_PRE + message.size());
    std::copy(message.cbegin(), message.cend(), preparedMessage.data() + LWS_PRE);
    _messages.emplace(std::move(preparedMessage));
}

auto LwsSession::getMessages() -> std::queue<std::string>&
{
    if (_messagesToSend.empty())
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        _messagesToSend.swap(_messages);
    }
    return _messagesToSend;
}

} // namespace ews::srv
