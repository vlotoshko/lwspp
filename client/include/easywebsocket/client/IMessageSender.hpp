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
 * @brief The IMessageSender class sends data to the server.
 * The instance of IMessageSender can be obtained using IMessageSenderAcceptor when building the client.
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
    // Sends text data to the server. The provided text data should be valid UTF-8 text.
    virtual void sendMessage(const std::string&) = 0;

    // Sends binary data to the server
    virtual void sendData(const std::vector<char>&) = 0;
};

} // namespace ews::cli
