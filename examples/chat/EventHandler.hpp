/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <map>
#include <vector>

#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/TypesFwd.hpp"

#include "ChatMessageSender.hpp"
#include "Types.hpp"

namespace wspp::chat
{

class EventHandler : public srv::IEventHandler
{
public:
    EventHandler();

    void onConnect(srv::SessionId) noexcept override;
    void onDisconnect(srv::SessionId) noexcept override;
    void onMessageReceive(srv::SessionId, const std::string& message) noexcept override;

    void onError(srv::SessionId, const std::string& errorMessage) noexcept override;
    void onWarning(srv::SessionId, const std::string& warningMessage) noexcept override;
    void setMessageSender(srv::IMessageSenderPtr) override;

private:
    ChatMessageSender _chatMessageSender;
    std::vector<Message> _chatMessages;
//    srv::IMessageSenderPtr _messageSender;

    std::map<srv::SessionId, User> _users;
};

} // namespace wspp::chat
