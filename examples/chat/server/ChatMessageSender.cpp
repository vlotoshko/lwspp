/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date March, 2023
 */

#include "easywebsocket/server/IDataSender.hpp"

#include "ChatMessageSender.hpp"

namespace ews::chat
{

auto isPrivateMessage(const User& user, const Message& message) -> bool
{
    return message.from.userName == user.userName ||  message.to.userName == user.userName;
}

auto isCommonMessage(const Message& message) -> bool
{
    return message.to.sessionId == srv::ALL_SESSIONS;
}

ChatMessageSender::ChatMessageSender(srv::IDataSenderPtr s)
    : _dataSender(std::move(s))
{}

void ChatMessageSender::sendUserMessage(const Message& message)
{
    // Message format: "MSG:<From>:<To>:MessageText"
    if (_dataSender != nullptr)
    {
        std::string messageToSend{"MSG:<"};
        messageToSend
                .append(message.from.userName)
                .append(">:<")
                .append(message.to.userName).append(">:")
                .append(message.text)
                ;
        if (message.to.sessionId == srv::ALL_SESSIONS)
        {
            _dataSender->sendTextData(messageToSend);
        }
        else
        {
            _dataSender->sendTextData(message.to.sessionId, messageToSend);
            if (message.to.sessionId != message.from.sessionId &&
                message.from.sessionId != srv::UNDEFINED_SESSION_ID)
            {
                _dataSender->sendTextData(message.from.sessionId, messageToSend);
            }
        }
    }
}

void ChatMessageSender::sendChatHistory(const User& user, const std::vector<Message>& history)
{
    // Message format: "HIST:<From>:<To>:MessageText"
    if (_dataSender != nullptr)
    {
        for (const auto& message : history)
        {
            if (isCommonMessage(message) || isPrivateMessage(user, message))
            {
                _dataSender->sendTextData(user.sessionId, std::string{"HIST:<"}
                                            .append(message.from.userName).append(">:<")
                                            .append(message.to.userName).append(">:")
                                            .append(message.text));
            }
        }
    }
}

void ews::chat::ChatMessageSender::updateUsers(const std::map<srv::SessionId, User>& users)
{
    // Message format: "USRUPD:User1,User2,User3,"
    if (_dataSender != nullptr)
    {
        std::string message{"USRUPD:"};
        for (const auto& entry : users)
        {
            message.append(entry.second.userName).append(",");
        }
        _dataSender->sendTextData(message);
    }
}

} // namespace ews::chat
