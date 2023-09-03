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
 * @brief The IDataSender class sends data to the client.
 * The instance of IDataSender can be obtained using IDataSenderAcceptor when building the server.
 */
class IDataSender
{
public:
    IDataSender() = default;
    virtual ~IDataSender() = default;

    IDataSender(const IDataSender&) = default;
    auto operator=(const IDataSender&) -> IDataSender& = default;

    IDataSender(IDataSender&&) noexcept = default;
    auto operator=(IDataSender&&) noexcept -> IDataSender& = default;

public:
    // Sends text data to the specified client identified by SessionId.
    // The provided text data should be valid UTF-8 text.
    virtual void sendTextData(SessionId, const std::string&) = 0;

    // Sends binary data to the specified client identified by SessionId.
    virtual void sendBinaryData(SessionId, const std::vector<char>&) = 0;

    // Sends text data to all connected clients.
    //The provided text data should be valid UTF-8 text.
    virtual void sendTextData(const std::string&) = 0;

    // Sends binary data to all connected clients.
    virtual void sendBinaryData(const std::vector<char>&) = 0;
};

} // namespace ews::srv
