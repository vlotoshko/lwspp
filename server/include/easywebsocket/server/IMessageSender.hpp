/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <string>
#include <vector>

#include "easywebsocket/server/Types.hpp"

namespace ews::srv
{

/**
 * @brief The IMessageSender class sends data to the client.
 * The instance of IMessageSender can be obtained using IMessageSenderAcceptor when building the server.
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
    // Sends text data to the specified client identified by SessionId.
    // The provided text data should be valid UTF-8 text.
    virtual void sendMessage(SessionId, const std::string&) = 0;

    // Sends binary data to the specified client identified by SessionId.
    virtual void sendData(SessionId, const std::vector<char>&) = 0;

    // Sends text data to all connected clients.
    //The provided text data should be valid UTF-8 text.
    virtual void sendMessage(const std::string&) = 0;

    // Sends binary data to all connected clients.
    virtual void sendData(const std::vector<char>&) = 0;
};

} // namespace ews::srv
