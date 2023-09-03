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

void EventHandlerBase::onBinaryDataReceive(const std::vector<char>& /*data*/, size_t /*bytesRemains*/) noexcept
{}

void EventHandlerBase::onTextDataReceive(const std::string& /*message*/, size_t /*bytesRemains*/) noexcept
{}

void EventHandlerBase::onError(const std::string& /*errorMessage*/) noexcept
{}

void EventHandlerBase::onWarning(const std::string& /*warningMessage*/) noexcept
{}

void EventHandlerBase::acceptDataSender(IDataSenderPtr dataSender) noexcept
{
    _dataSender = std::move(dataSender);
}

} // namespace ews::cli
