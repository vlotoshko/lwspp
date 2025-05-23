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

#pragma once

#include <map>
#include <vector>

#include "lwspp/server/ServerLogicBase.hpp"

#include "ChatMessageSender.hpp"
#include "Types.hpp"

namespace lwspp
{
namespace chat
{

class ServerLogic : public srv::ServerLogicBase
{
public:
    ServerLogic();
    
    void onConnect(srv::IConnectionInfoPtr) noexcept override;
    void onDisconnect(srv::ConnectionId) noexcept override;
    void onTextDataReceive(srv::ConnectionId, const srv::DataPacket&) noexcept override;
    
    void acceptServerControl(srv::IServerControlPtr) noexcept override;

private:
    void processHelloMessage_(srv::ConnectionId, const std::string& message);
    void processUserMessage_(srv::ConnectionId, const std::string& message);

private:
    ChatMessageSender _chatMessageSender;
    std::vector<Message> _history;
    
    std::map<srv::ConnectionId, User> _users;
};

} // namespace chat
} // namespace lwspp
