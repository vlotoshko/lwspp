/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date March, 2023
 */

#include "websocketpp/server/IMessageSender.hpp"

#include "ChatMessageSender.hpp"

namespace wspp::chat
{

bool isPrivateMessage(const User& user, const Message& message)
{
    return message.from.sessionId == user.sessionId ||  message.to.sessionId == user.sessionId;
}

bool isCommonMessage(const Message& message)
{
    return message.to.sessionId == srv::ALL_SESSIONS;
}

ChatMessageSender::ChatMessageSender(srv::IMessageSenderPtr s)
    : _messageSender(std::move(s))
{}

void ChatMessageSender::sendUserMessage(const Message& message)
{
    // Message format: "MSG:<From>:<To>:<MessageText>"
    std::string messageToSend{"MSG:"};
    messageToSend.append(message.from.userName).append(":");
    if (message.to.sessionId == srv::ALL_SESSIONS)
    {
        messageToSend.append("ALL:").append(message.text);
        _messageSender->sendMessage(messageToSend);
    }
    else
    {
        messageToSend.append(message.text);

        _messageSender->sendMessage(message.to.sessionId, messageToSend);
        if (message.to.sessionId != message.from.sessionId)
        {
            _messageSender->sendMessage(message.from.sessionId, messageToSend);
        }
    }
}

void ChatMessageSender::sendChatHistory(const User& user, const std::vector<Message>& messages)
{
    // Message format: "HIST:<From>:<To>:<MessageText>"
    if (_messageSender != nullptr)
    {
        for (const auto& message : messages)
        {
            if (isCommonMessage(message) || isPrivateMessage(user, message))
            {
                _messageSender->sendMessage(user.sessionId, std::string{"HIST:"}
                                            .append(message.from.userName).append(":")
                                            .append(message.to.userName).append(":")
                                            .append(message.text));
            }
        }
    }
}

void wspp::chat::ChatMessageSender::updateUsers(const std::map<srv::SessionId, User>& users)
{
    // Message format: "USRUPD:<User1>,<User2>,<User3>,"
    if (_messageSender != nullptr)
    {
        std::string message{"USRUPD:"};
        for (const auto& entry : users)
        {
            message.append(entry.second.userName).append(",");
        }
        _messageSender->sendMessage(message);
    }
}

} // namespace wspp::chat
