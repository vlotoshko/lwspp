/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

#include <string>
#include <vector>

namespace ews::cli
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
    // Sends a message to a server
    virtual void sendMessage(const std::string&) = 0;
    // Sends a message to a server
    virtual void sendData(const std::vector<char>&) = 0;
};

} // namespace ews::cli
