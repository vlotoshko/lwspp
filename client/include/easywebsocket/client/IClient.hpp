/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

namespace ews::cli
{

/**
 * @brief The IClient class defines the client interface for interacting with a server.
 *
 * Use the client builder to obtain an instance of the client implementation.
 *
 * @note The client automatically connects upon construction and disconnects upon destruction.
 * @note The client operates within a separate thread to establish and maintain the connection
 * with the server.
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
