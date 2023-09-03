/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IDataSenderAcceptor.hpp"

namespace ews::srv
{
/**
 * @brief The EventHandlerBase class serves as a convenient base class for implementing the IEventHandler interface.
 * It provides stubs for all overridden methods in the IEventHandler interface and also implements
 * the IDataSenderAcceptor to obtain the IDataSender.
 */
class EventHandlerBase : public IEventHandler, public IDataSenderAcceptor
{
public:
    void onConnect(ISessionInfoPtr) noexcept override;
    void onDisconnect(SessionId) noexcept override;

    void onBinaryDataReceive(SessionId, const std::vector<char>& data, size_t bytesRemains) noexcept override;
    void onTextDataReceive(SessionId, const std::string& message, size_t bytesRemains) noexcept override;
    void onError(SessionId, const std::string& errorMessage) noexcept override;
    void onWarning(SessionId, const std::string& warningMessage) noexcept override;

    void acceptDataSender(IDataSenderPtr) noexcept override;

protected:
    IDataSenderPtr _dataSender;
};

} // namespace ews::srv
