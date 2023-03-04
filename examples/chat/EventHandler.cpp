/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "websocketpp/server/Consts.hpp"
//#include "websocketpp/server/IMessageSender.hpp"

#include "EventHandler.hpp"

namespace wspp::chat
{

namespace
{

const User ALL_USERS = User{srv::ALL_SESSIONS, "ALL"};

auto getRecipient(const std::map<srv::SessionId, User>& users, const std::string& message)
-> User
{
    auto pos = message.find_first_of(':');

    if (pos != std::string::npos)
    {
        auto userName = message.substr(0, pos);
        for (const auto& entry : users)
        {
            if (entry.second.userName == userName)
            {
                return entry.second;
                break;
            }
        }
    }
    return ALL_USERS;
}

} // namespace

EventHandler::EventHandler() : _chatMessageSender(srv::IMessageSenderPtr{})
{}

void EventHandler::onConnect(wspp::srv::SessionId sessionId) noexcept
{
    User user = {sessionId, std::string{"user#"}.append(std::to_string(sessionId))};
    _users[sessionId] = user;

    _chatMessageSender.updateUsers(_users);
    _chatMessageSender.sendChatHistory(user, _chatMessages);
}

void EventHandler::onDisconnect(wspp::srv::SessionId sessionId) noexcept
{
    _users.erase(sessionId);
    _chatMessageSender.updateUsers(_users);
}

void EventHandler::onMessageReceive(srv::SessionId sessionId, const std::string& messageText) noexcept
{
    auto recipient = getRecipient(_users, messageText);
    auto message = Message{_users[sessionId], recipient, messageText};
    _chatMessageSender.sendUserMessage(message);
    _chatMessages.emplace_back(std::move(message));
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
