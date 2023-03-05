/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "websocketpp/server/Consts.hpp"

#include "EventHandler.hpp"

namespace wspp::chat
{

namespace
{

const User ALL_USERS = User{srv::ALL_SESSIONS, "ALL"};

auto trim(const std::string& str) -> std::string
{
    const size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
    {
        return std::string{};
    }
    const size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

auto getUserByName(const std::map<srv::SessionId, User>& users, const std::string& userName)
-> User
{
    for (const auto& entry : users)
    {
        if (entry.second.userName == userName)
        {
            return entry.second;
            break;
        }
    }
    return ALL_USERS;
}

// Trying to retrieve recipient name if message is private
auto parseIncomingMessage(const std::map<srv::SessionId, User>& users, const std::string& message)
-> std::tuple<User, std::string>
{
    auto pos = message.find_first_of(">:");

    if (pos != std::string::npos && message.front() == '<')
    {
        auto userName = message.substr(1, pos-1);
        auto rawMessage = message.substr(pos+2, message.length()-1);
        return std::make_pair(getUserByName(users, userName), trim(rawMessage));
    }

    return std::make_pair(ALL_USERS, trim(message));
}

} // namespace

EventHandler::EventHandler() : _chatMessageSender(srv::IMessageSenderPtr{})
{}

void EventHandler::onConnect(wspp::srv::SessionId sessionId) noexcept
{
    const User user = {sessionId, std::string{"user#"}.append(std::to_string(sessionId))};
    _users[sessionId] = user;

    _chatMessageSender.sendWellcome(user);
    _chatMessageSender.updateUsers(_users);
    _chatMessageSender.sendChatHistory(user, _history);
}

void EventHandler::onDisconnect(wspp::srv::SessionId sessionId) noexcept
{
    _users.erase(sessionId);
    _chatMessageSender.updateUsers(_users);
}

void EventHandler::onMessageReceive(srv::SessionId sessionId, const std::string& messageText) noexcept
{
    // Expected common message format: "MessageText"
    // Expected private message format: "<To>: MessageText"
    User recipient;
    std::string rawMessage;
    std::tie(recipient, rawMessage) = parseIncomingMessage(_users, messageText);

    auto message = Message{_users[sessionId], recipient, rawMessage};
    _chatMessageSender.sendUserMessage(message);
    _history.emplace_back(std::move(message));
}

void EventHandler::onError(srv::SessionId, const std::string& /*errorMessage*/) noexcept
{}

void EventHandler::onWarning(srv::SessionId, const std::string& /*warningMessage*/) noexcept
{}

void EventHandler::setMessageSender(srv::IMessageSenderPtr messageSender)
{
    _chatMessageSender = ChatMessageSender{messageSender};
}

} // namespace wspp::chat
