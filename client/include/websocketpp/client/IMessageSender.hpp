/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

#include <string>

#include "websocketpp/client/Types.hpp"

namespace wspp::cli
{

/**
 * @brief The IMessageSender class sends messages to the server
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
    virtual void sendMessage(SessionId, const std::string&) = 0;
    // Sends a message to all clients
    virtual void sendMessage(const std::string&) = 0;
};

} // namespace wspp::cli
