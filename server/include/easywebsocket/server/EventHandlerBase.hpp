/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IMessageSenderAcceptor.hpp"

namespace ews::srv
{

class EventHandlerBase : public IEventHandler, public IMessageSenderAcceptor
{
public:
    void onConnect(ISessionInfoPtr) noexcept override;
    void onDisconnect(SessionId) noexcept override;

    void onMessageReceive(SessionId, const std::string& message) noexcept override;
    void onError(SessionId, const std::string& errorMessage) noexcept override;
    void onWarning(SessionId, const std::string& errorMessage) noexcept override;

    void acceptMessageSender(IMessageSenderPtr) noexcept override;

protected:
    IMessageSenderPtr _messageSender;
};

} // namespace ews::srv
