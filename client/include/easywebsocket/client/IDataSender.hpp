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
 * @brief The IDataSender class sends data to the server.
 * The instance of IDataSender can be obtained using IDataSenderAcceptor when building the client.
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
    // Sends text data to the server. The provided text data should be valid UTF-8 text.
    virtual void sendTextData(const std::string&) = 0;

    // Sends binary data to the server
    virtual void sendBinaryData(const std::vector<char>&) = 0;
};

} // namespace ews::cli
