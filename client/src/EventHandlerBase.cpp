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

void EventHandlerBase::onDataReceive(const std::vector<char>& /*data*/, size_t /*bytesRemains*/) noexcept
{}

void EventHandlerBase::onMessageReceive(const std::string& /*message*/, size_t /*bytesRemains*/) noexcept
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
