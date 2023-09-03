/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

namespace ews::srv
{

/**
 * @brief The IServer class defines the server interface for interacting with clients.
 *
 * Use the server builder to obtain an instance of the server implementation.
 *
 * @note The server starts listening upon construction and stops listening upon destruction.
 * @note The server operates within a separate thread to handle incoming connections.
 */
class IServer
{
public:
    IServer() = default;
    virtual ~IServer() = default;

    IServer(IServer&&) = default;
    auto operator=(IServer&&) noexcept -> IServer& = default;

    IServer(const IServer&) = delete;
    auto operator=(const IServer&) noexcept -> IServer& = delete;
};

} // namespace ews::srv
