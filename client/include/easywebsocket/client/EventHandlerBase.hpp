/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/client/IEventHandler.hpp"
#include "easywebsocket/client/IMessageSenderAcceptor.hpp"

namespace ews::cli
{

class EventHandlerBase : public IEventHandler, public IMessageSenderAcceptor
{
public:
    void onConnect(ISessionInfoPtr) noexcept override;
    void onDisconnect() noexcept override;

    void onMessageReceive(const std::string& message) noexcept override;
    void onError(const std::string& errorMessage) noexcept override;
    void onWarning(const std::string& warningMessage) noexcept override;

    void acceptMessageSender(IMessageSenderPtr) noexcept override;

protected:
    IMessageSenderPtr _messageSender;
};

} // namespace ews::cli
