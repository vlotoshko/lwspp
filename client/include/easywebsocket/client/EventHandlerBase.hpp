/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "easywebsocket/client/IEventHandler.hpp"
#include "easywebsocket/client/IMessageSenderAcceptor.hpp"

namespace ews::cli
{

/**
 * @brief The EventHandlerBase class serves as a convenient base class for implementing the IEventHandler interface.
 * It provides stubs for all overridden methods in the IEventHandler interface and also implements
 * the IMessageSenderAcceptor to obtain the IMessageSender.
 */
class EventHandlerBase : public IEventHandler, public IMessageSenderAcceptor
{
public:
    void onConnect(ISessionInfoPtr) noexcept override;
    void onDisconnect() noexcept override;

    void onDataReceive(const std::vector<char>& data, size_t bytesRemains) noexcept override;
    void onMessageReceive(const std::string& message, size_t bytesRemains) noexcept override;
    void onError(const std::string& errorMessage) noexcept override;
    void onWarning(const std::string& warningMessage) noexcept override;

    void acceptMessageSender(IMessageSenderPtr) noexcept override;

protected:
    IMessageSenderPtr _messageSender;
};

} // namespace ews::cli
