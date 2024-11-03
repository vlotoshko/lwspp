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

#include "lwspp/server/Consts.hpp"
#include "lwspp/server/IConnectionInfo.hpp" // IWYU pragma: keep

#include "EventHandler.hpp"

namespace lwspp
{
namespace chat
{

namespace
{

// Incoming message types
enum class DataType : std::uint8_t
{
    HELLO,
    MSG,
    UNKNOWN,
};

const std::string UNKNOWN_USERN_NAME = "UNKNOWN_USER_NAME";
const User UNKNOWN_USER = User{srv::UNDEFINED_CONNECTION_ID, "UNKNOWN"};
const User SYSTEM = User{srv::UNDEFINED_CONNECTION_ID, "SYS"};
const User ALL_USERS = User{srv::ALL_CONNECTIONS, "ALL"};

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

auto getDataType(const std::string& message) -> DataType
{
    auto pos = message.find_first_of(':');

    if (pos != std::string::npos)
    {
        auto type = message.substr(0, pos);
        if (type == "HELLO")
        {
            return DataType::HELLO;
        }

        if (type == "MSG")
        {
            return DataType::MSG;
        }
    }

    return DataType::UNKNOWN;
}

auto getUserByName(const std::map<srv::ConnectionId, User>& users, const std::string& userName)
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
auto parseIncomingMessage(const std::map<srv::ConnectionId, User>& users, const std::string& message)
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
        if(user.connectionId != UNKNOWN_USER.connectionId)
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

EventHandler::EventHandler() : _chatMessageSender(srv::IActorPtr{})
{}

void EventHandler::onConnect(lwspp::srv::IConnectionInfoPtr connectionInfo) noexcept
{
    const User user = {connectionInfo->getConnectionId(), UNKNOWN_USERN_NAME};
    _users[connectionInfo->getConnectionId()] = user;
}

void EventHandler::onDisconnect(lwspp::srv::ConnectionId connectionId) noexcept
{
    _users.erase(connectionId);
    _chatMessageSender.updateUsers(_users);
}

void EventHandler::onTextDataReceive(srv::ConnectionId connectionId, const srv::DataPacket& dataPacket) noexcept
{
    // Expected messages:
    //   hello message, format: "HELLO:<Nickname>"
    //   user message public, format: "MSG:MessageText"
    //   user message private, format: "MSG:<To>: MessageText"

    auto message = std::string{dataPacket.data, dataPacket.length};

    switch (getDataType(message))
    {
    case DataType::HELLO:
    {
        processHelloMessage_(connectionId, message);
        break;
    }
    case DataType::MSG:
    {
        processUserMessage_(connectionId, message);
        break;
    }
    default:
        // TODO: unknown message type warning
        break;
    }
}

void EventHandler::acceptActor(srv::IActorPtr messageSender) noexcept
{
    srv::EventHandlerBase::acceptActor(messageSender);
    _chatMessageSender = ChatMessageSender{_actor};
}

void EventHandler::processHelloMessage_(srv::ConnectionId connectionId, const std::string& messageText)
{
    auto userName = getUserNameFromHello(messageText);
    if (!userName.empty())
    {
        auto userWithThisName = getUserByName(_users, userName);
        if(userWithThisName.connectionId == UNKNOWN_USER.connectionId)
        {
            auto& user = _users[connectionId];
            user.userName = userName;
            _chatMessageSender.updateUsers(_users);
            _chatMessageSender.sendChatHistory(user, _history);
        }
        else
        {
            auto sendText =
                    std::string{"nickname '"}.append(userName).append("' is already in use");
            auto message = Message{SYSTEM, _users[connectionId], sendText};
            _chatMessageSender.sendUserMessage(message);
        }
    }
    else
    {
        _chatMessageSender.sendUserMessage(Message{SYSTEM, _users[connectionId],
                                                   "error: empty nickname"});
    }
}

void EventHandler::processUserMessage_(srv::ConnectionId connectionId, const std::string& messageText)
{
    User recipient;
    std::string rawMessage;
    std::tie(recipient, rawMessage) = parseIncomingMessage(_users, messageText);

    auto message = Message{_users[connectionId], recipient, rawMessage};
    _chatMessageSender.sendUserMessage(message);
    _history.emplace_back(std::move(message));
}

} // namespace chat
} // namespace lwspp
