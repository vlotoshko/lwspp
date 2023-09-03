/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "easywebsocket/client/IDataSenderAcceptor.hpp"
#include "easywebsocket/client/IEventHandler.hpp"

namespace ews::cli
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
    void onDisconnect() noexcept override;

    void onBinaryDataReceive(const std::vector<char>& data, size_t bytesRemains) noexcept override;
    void onTextDataReceive(const std::string& message, size_t bytesRemains) noexcept override;
    void onError(const std::string& errorMessage) noexcept override;
    void onWarning(const std::string& warningMessage) noexcept override;

    void acceptDataSender(IDataSenderPtr) noexcept override;

protected:
    IDataSenderPtr _dataSender;
};

} // namespace ews::cli
