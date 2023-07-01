/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/client/IEventHandler.hpp"

namespace ews::cli
{

class EventHandlerBase : public IEventHandler
{
public:
    void onConnect(ISessionInfoPtr) noexcept override;
    void onDisconnect() noexcept override;

    void onMessageReceive(const std::string& message) noexcept override;
    void onError(const std::string& errorMessage) noexcept override;
    void onWarning(const std::string& errorMessage) noexcept override;

    void setMessageSender(IMessageSenderPtr) noexcept override;

protected:
    IMessageSenderPtr _messageSender;
};

} // namespace ews::cli
