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

#include "lwspp/server/IServerControl.hpp" // IWYU pragma: keep

#include "ChatMessageSender.hpp"

namespace lwspp
{
namespace chat
{

auto isPrivateMessage(const User& user, const Message& message) -> bool
{
    return message.from.userName == user.userName ||  message.to.userName == user.userName;
}

auto isPublicMessage(const Message& message) -> bool
{
    return message.to.connectionId == srv::ALL_CONNECTIONS;
}

ChatMessageSender::ChatMessageSender(srv::IServerControlPtr s)
    : _serverControl(std::move(s))
{}

void ChatMessageSender::sendUserMessage(const Message& message)
{
    // Message format: "MSG:<From>:<To>:MessageText"
    if (_serverControl != nullptr)
    {
        std::string messageToSend{"MSG:<"};
        messageToSend
                .append(message.from.userName)
                .append(">:<")
                .append(message.to.userName).append(">:")
                .append(message.text)
                ;
        if (message.to.connectionId == srv::ALL_CONNECTIONS)
        {
            _serverControl->sendTextData(messageToSend);
        }
        else
        {
            _serverControl->sendTextData(message.to.connectionId, messageToSend);
            if (message.to.connectionId != message.from.connectionId &&
                message.from.connectionId != srv::UNDEFINED_CONNECTION_ID)
            {
                _serverControl->sendTextData(message.from.connectionId, messageToSend);
            }
        }
    }
}

void ChatMessageSender::sendChatHistory(const User& user, const std::vector<Message>& history)
{
    // Message format: "HIST:<From>:<To>:MessageText"
    if (_serverControl != nullptr)
    {
        for (const auto& message : history)
        {
            if (isPublicMessage(message) || isPrivateMessage(user, message))
            {
                _serverControl->sendTextData(user.connectionId, std::string{"HIST:<"}
                                            .append(message.from.userName).append(">:<")
                                            .append(message.to.userName).append(">:")
                                            .append(message.text));
            }
        }
    }
}

void ChatMessageSender::updateUsers(const std::map<srv::ConnectionId, User>& users)
{
    // Message format: "USRUPD:User1,User2,User3,"
    if (_serverControl != nullptr)
    {
        std::string message{"USRUPD:"};
        for (const auto& entry : users)
        {
            message.append(entry.second.userName).append(",");
        }
        _serverControl->sendTextData(message);
    }
}

} // namespace chat
} // namespace lwspp
