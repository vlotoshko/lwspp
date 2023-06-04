/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

namespace wspp::srv
{

/**
 * @brief The IServer class defines the server interface. Use the server factory to get the
 * instance of this interface implementation
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

public:
    // NOTE: the 'start' method can be implemented as blocking thread method
    virtual void start() = 0;
};

} // namespace wspp::srv
