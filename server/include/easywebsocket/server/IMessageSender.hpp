/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/Types.hpp"

namespace ews::srv
{

/**
 * @brief The IMessageSender class sends messages to the server's client
 */
class IMessageSender
{
public:
    IMessageSender() = default;
    virtual ~IMessageSender() = default;

    IMessageSender(const IMessageSender&) = default;
    auto operator=(const IMessageSender&) -> IMessageSender& = default;

    IMessageSender(IMessageSender&&) noexcept = default;
    auto operator=(IMessageSender&&) noexcept -> IMessageSender& = default;

public:
    // Sends a message to a specific client
    virtual void sendMessage(SessionId, const std::string&) = 0;
    // Sends a message to all clients
    virtual void sendMessage(const std::string&) = 0;
};

} // namespace ews::srv
