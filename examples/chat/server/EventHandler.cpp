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

// Incoming message types
enum class MessageType
{
    HELLO,
    MSG,
    UNKNOWN,
};

const std::string UNKNOWN_USERN_NAME = "UNKNOWN_USERN_NAME";
const User UNKNOWN_USER = User{srv::UNDEFINED_SESSION_ID, "UNKNOWN"};
const User SYSTEM = User{srv::UNDEFINED_SESSION_ID, "SYS"};
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

auto getMessageType(const std::string& message) -> MessageType
{
    auto pos = message.find_first_of(':');

    if (pos != std::string::npos)
    {
        auto type = message.substr(0, pos);
        if (type == "HELLO")
        {
            return MessageType::HELLO;
        }

        if (type == "MSG")
        {
            return MessageType::MSG;
        }
    }

    return MessageType::UNKNOWN;
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
    return UNKNOWN_USER;
}

// Trying to retrieve recipient name if message is private
auto parseIncomingMessage(const std::map<srv::SessionId, User>& users, const std::string& message)
-> std::tuple<User, std::string>
{
    const std::string messagePrefix = "MSG:<";
    const auto prefixLength = messagePrefix.size();

    auto nameBegin = message.find(messagePrefix);
    auto nameEnd = message.find(">:");

    if (nameBegin < nameEnd && nameBegin != std::string::npos && nameEnd != std::string::npos)
    {
        auto userName = message.substr(nameBegin + prefixLength, nameEnd - prefixLength);
        auto rawMessage = message.substr(nameEnd + 2, message.length() - 1);

        auto user = getUserByName(users, userName);
        if(user.sessionId != UNKNOWN_USER.sessionId)
        {
            return std::make_pair(user, trim(rawMessage));
        }
    }

    auto pos = message.find(':');
    if(pos != std::string::npos)
    {
        auto rawMessage = message.substr(pos + 1, message.length() - 1);
        return std::make_pair(ALL_USERS, trim(rawMessage));
    }

    return std::make_pair(ALL_USERS, trim(message));
}

auto getUserNameFromHello(const std::string& message) -> std::string
{
    auto pos = message.find(':');
    if (pos != std::string::npos)
    {
        auto userName = message.substr(pos + 1, message.length() - 1);
        return userName;
    }

    return std::string{};
}

} // namespace

EventHandler::EventHandler() : _chatMessageSender(srv::IMessageSenderPtr{})
{}

void EventHandler::onConnect(wspp::srv::SessionId sessionId) noexcept
{
    const User user = {sessionId, UNKNOWN_USERN_NAME};
    _users[sessionId] = user;
}

void EventHandler::onDisconnect(wspp::srv::SessionId sessionId) noexcept
{
    _users.erase(sessionId);
    _chatMessageSender.updateUsers(_users);
}

void EventHandler::onMessageReceive(srv::SessionId sessionId, const std::string& messageText) noexcept
{
    // Expected messages:
    //   hello message, format: "HELLO:<Nikcname>"
    //   user message common, format: "MSG:MessageText"
    //   user message private, format: "MSG:<To>: MessageText"

    switch (getMessageType(messageText))
    {
    case MessageType::HELLO:
    {
        processHelloMessage_(sessionId, messageText);
        break;
    }
    case MessageType::MSG:
    {
        processUserMessage_(sessionId, messageText);
        break;
    }

    default:
        break;
    }
}

void EventHandler::onError(srv::SessionId, const std::string& /*errorMessage*/) noexcept
{}

void EventHandler::onWarning(srv::SessionId, const std::string& /*warningMessage*/) noexcept
{}

void EventHandler::setMessageSender(srv::IMessageSenderPtr messageSender)
{
    _chatMessageSender = ChatMessageSender{messageSender};
}

void EventHandler::processHelloMessage_(srv::SessionId sessionId, const std::string& messageText)
{
    auto userName = getUserNameFromHello(messageText);
    if (!userName.empty())
    {
        auto userWithThisName = getUserByName(_users, userName);
        if(userWithThisName.sessionId == UNKNOWN_USER.sessionId)
        {
            auto& user = _users[sessionId];
            user.userName = userName;
            _chatMessageSender.updateUsers(_users);
            _chatMessageSender.sendChatHistory(user, _history);
        }
        else
        {
            auto sendText =
                    std::string{"nickname '"}.append(userName).append("' is already in use");
            auto message = Message{SYSTEM, _users[sessionId], sendText};
            _chatMessageSender.sendUserMessage(message);
        }
    }
    else
    {
        _chatMessageSender.sendUserMessage(Message{SYSTEM, _users[sessionId],
                                                   "error: empty nickname"});
    }
}

void EventHandler::processUserMessage_(srv::SessionId sessionId, const std::string& messageText)
{
    User recipient;
    std::string rawMessage;
    std::tie(recipient, rawMessage) = parseIncomingMessage(_users, messageText);

    auto message = Message{_users[sessionId], recipient, rawMessage};
    _chatMessageSender.sendUserMessage(message);
    _history.emplace_back(std::move(message));
}

} // namespace wspp::chat
