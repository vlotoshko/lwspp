/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date March, 2023
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "easywebsocket/server/TypesFwd.hpp"

#include "Types.hpp"

namespace ews::chat
{

class ChatMessageSender
{
public:
    explicit ChatMessageSender(srv::IMessageSenderPtr);

    void sendUserMessage(const Message&);
    void sendChatHistory(const User&, const std::vector<Message>&);

    void updateUsers(const std::map<srv::SessionId, User>& users);

private:
    srv::IMessageSenderPtr _messageSender;
};

} // namespace ews::chat
