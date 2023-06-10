/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

namespace ews::cli
{
/**
 * @brief The IClient class defines the client interface. Use the client factory to get the
 * instance of the client implementation
 * @note The client connects on construction and disconnects on destruction
 * @note The client spawns a separate thread to connect to a server
 */
class IClient
{
public:
    IClient() = default;
    virtual ~IClient() = default;

    IClient(IClient&&) = default;
    auto operator=(IClient&&) noexcept -> IClient& = default;

    IClient(const IClient&) = delete;
    auto operator=(const IClient&) noexcept -> IClient& = delete;
};

} // namespace ews::cli
