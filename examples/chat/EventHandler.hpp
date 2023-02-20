/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/TypesFwd.hpp"

namespace wspp::chat
{

class EventHandler : public srv::IEventHandler
{
public:
    void onConnect(srv::SessionId) noexcept override;
    void onDisconnect(srv::SessionId) noexcept override;
    void onError(srv::SessionId, const std::string& errorMessage) noexcept override;
    void setMessageSender(srv::IMessageSenderPtr) override;

private:
    srv::IMessageSenderPtr _messageSender;
};

} // namespace wspp::chat
