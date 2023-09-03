/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "easywebsocket/server/TypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief Accepts an IMessageSender instance from the server builder.
 *
 * The server builder provides an IMessageSender instance when constructing the server.
 * To utilize the IMessageSender, set your instance of IMessageSenderAcceptor to the server builder
 * and obtain the IMessageSender to transfer the messages from the server to the client.
 * Users of the library must implement this interface themselves.
 */
class IMessageSenderAcceptor
{
public:
    IMessageSenderAcceptor() = default;
    virtual ~IMessageSenderAcceptor() = default;

    IMessageSenderAcceptor(const IMessageSenderAcceptor&) = default;
    auto operator=(const IMessageSenderAcceptor&) noexcept -> IMessageSenderAcceptor& = default;

    IMessageSenderAcceptor(IMessageSenderAcceptor&&) = default;
    auto operator=(IMessageSenderAcceptor&&) noexcept -> IMessageSenderAcceptor& = default;

public:
    virtual void acceptMessageSender(IMessageSenderPtr) noexcept = 0;
};

} // namespace ews::srv
