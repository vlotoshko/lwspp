/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The IMessageSenderAcceptor class
 */
class IMessageSenderAcceptor
{
public:
    IMessageSenderAcceptor() = default;
    virtual ~IMessageSenderAcceptor() = default;

    IMessageSenderAcceptor(const IMessageSenderAcceptor&) = default;
    IMessageSenderAcceptor(IMessageSenderAcceptor&&) = default;

    IMessageSenderAcceptor& operator=(const IMessageSenderAcceptor&) = default;
    IMessageSenderAcceptor& operator=(IMessageSenderAcceptor&&) = default;

public:
    virtual void acceptMessageSender(IMessageSenderPtr) noexcept = 0;
};


} // namespace ews::cli
