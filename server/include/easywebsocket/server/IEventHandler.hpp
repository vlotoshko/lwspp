/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/Types.hpp"
#include "easywebsocket/server/TypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The IEventHandler class defines interface for the server behavior implementation.
 * The library user implements this interface on its own.
 */
class IEventHandler
{
public:
    IEventHandler() = default;
    virtual ~IEventHandler() = default;

    IEventHandler(IEventHandler&&) = default;
    auto operator=(IEventHandler&&) noexcept -> IEventHandler& = default;

    IEventHandler(const IEventHandler&) = delete;
    auto operator=(const IEventHandler&) noexcept -> IEventHandler& = delete;

public:
    virtual void onConnect(ISessionInfoPtr) noexcept = 0;
    virtual void onDisconnect(SessionId) noexcept = 0;

    virtual void onMessageReceive(SessionId, const std::string& message) noexcept = 0;
    virtual void onError(SessionId, const std::string& errorMessage) noexcept = 0;
    virtual void onWarning(SessionId, const std::string& errorMessage) noexcept = 0;

    // The setMessageSender method is used by server factory to set the message sender
    // for the IEventHandler. The user's implementations of the IEventHandler can use the
    // IMessageSenderPtr to send messages to the clients.
    virtual void setMessageSender(IMessageSenderPtr) noexcept = 0;
};

} // namespace ews::srv
