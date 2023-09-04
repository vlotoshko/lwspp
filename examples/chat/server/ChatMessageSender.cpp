/*
 * EasyWebsockets - C++ wrapper for the libwebsockets library
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
