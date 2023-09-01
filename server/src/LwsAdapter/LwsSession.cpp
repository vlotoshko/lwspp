/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "LwsAdapter/LwsSession.hpp"

namespace ews::srv
{

namespace
{

// NOTE: Additional space with the size of LWS_PRE should be added in the front of the data
// For more information please read lws_write description
template <typename Container>
auto addPrefixToMessage(const Container& message) -> std::string
{
    std::string result;
    // NOTE: resize can throw bad alloc if message is too large
    result.resize(LWS_PRE + message.size());
    std::copy(message.cbegin(), message.cend(), result.data() + LWS_PRE);
    return result;
}

} // namespace

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

void LwsSession::addBinaryData(const std::vector<char>& binaryData)
{
    std::string payload = addPrefixToMessage(binaryData);

    const std::lock_guard<std::mutex> guard(_mutex);
    _messages.emplace(MessageType::Binary, std::move(payload));
}

void LwsSession::addTextData(const std::string& textData)
{
    std::string payload = addPrefixToMessage(textData);

    const std::lock_guard<std::mutex> guard(_mutex);
    _messages.emplace(MessageType::Text, std::move(payload));
}

auto LwsSession::getMessages() -> std::queue<Message>&
{
    if (_messagesToSend.empty())
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        _messagesToSend.swap(_messages);
    }
    return _messagesToSend;
}

} // namespace ews::srv
