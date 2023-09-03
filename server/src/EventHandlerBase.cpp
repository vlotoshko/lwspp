/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#include "easywebsocket/server/EventHandlerBase.hpp"

namespace ews::srv
{

void EventHandlerBase::onConnect(ISessionInfoPtr) noexcept
{}

void EventHandlerBase::onDisconnect(SessionId) noexcept
{}

void EventHandlerBase::onBinaryDataReceive(SessionId, const std::vector<char>& /*data*/, size_t /*bytesRemains*/) noexcept
{}

void EventHandlerBase::onTextDataReceive(SessionId, const std::string& /*message*/, size_t /*bytesRemains*/) noexcept
{}

void EventHandlerBase::onError(SessionId, const std::string& /*errorMessage*/) noexcept
{}

void EventHandlerBase::onWarning(SessionId, const std::string& /*warningMessage*/) noexcept
{}

void EventHandlerBase::acceptDataSender(IDataSenderPtr dataSender) noexcept
{
    _dataSender = std::move(dataSender);
}

} // namespace ews::srv
