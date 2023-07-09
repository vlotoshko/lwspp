/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#include "easywebsocket/client/EventHandlerBase.hpp"

namespace ews::cli
{

void EventHandlerBase::onConnect(ISessionInfoPtr) noexcept
{}

void EventHandlerBase::onDisconnect() noexcept
{}

void EventHandlerBase::onMessageReceive(const std::string& /*message*/) noexcept
{}

void EventHandlerBase::onError(const std::string& /*errorMessage*/) noexcept
{}

void EventHandlerBase::onWarning(const std::string& /*warningMessage*/) noexcept
{}

void EventHandlerBase::acceptMessageSender(IMessageSenderPtr messageSender) noexcept
{
    _messageSender = std::move(messageSender);
}

} // namespace ews::cli
