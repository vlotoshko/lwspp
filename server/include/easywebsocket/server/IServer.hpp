/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

namespace ews::srv
{

/**
 * @brief The IServer class defines the server interface. Use the server factory to get the
 * instance of the server implementation
 * @note The server starts listening on construction and stops listening on destruction
 * @note The server spawns a separate thread for the listening
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
