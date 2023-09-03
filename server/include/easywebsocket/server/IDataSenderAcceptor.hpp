/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "easywebsocket/server/TypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief Accepts an IDataSender instance from the server builder.
 *
 * The server builder provides an IDataSender instance when constructing the server.
 * To utilize the IDataSender, set your instance of IDataSenderAcceptor to the server builder
 * and obtain the IDataSender to transfer the data from the server to the client.
 * Users of the library must implement this interface themselves.
 */
class IDataSenderAcceptor
{
public:
    IDataSenderAcceptor() = default;
    virtual ~IDataSenderAcceptor() = default;

    IDataSenderAcceptor(const IDataSenderAcceptor&) = default;
    auto operator=(const IDataSenderAcceptor&) noexcept -> IDataSenderAcceptor& = default;

    IDataSenderAcceptor(IDataSenderAcceptor&&) = default;
    auto operator=(IDataSenderAcceptor&&) noexcept -> IDataSenderAcceptor& = default;

public:
    virtual void acceptDataSender(IDataSenderPtr) noexcept = 0;
};

} // namespace ews::srv
