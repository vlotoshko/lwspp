/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief Accepts an IMessageSender instance from the client builder.
 *
 * The client builder provides an IMessageSender instance when constructing the client.
 * To utilize the IMessageSender, set your instance of IMessageSenderAcceptor to the client builder
 * and obtain the IMessageSender to transfer the messages from the client to the server.
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

} // namespace ews::cli
