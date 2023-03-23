/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/server/ServerVersions.hpp"
#include "websocketpp/server/Types.hpp"
#include "websocketpp/server/TypesFwd.hpp"

namespace wspp::srv
{

class ServerContext;

/**
 * @brief The ServerContextBuilder class builds the context for the server factory to
 * create the server with desired options. Some options are mandatory to set. Builder
 * throws an exception if any of the mandatory options are not set.
 */
class ServerContextBuilder
{
public:
    ServerContextBuilder();
    ~ServerContextBuilder();

    ServerContextBuilder(ServerContextBuilder&&) noexcept = default;
    auto operator=(ServerContextBuilder&&) noexcept -> ServerContextBuilder& = default;

    ServerContextBuilder(const ServerContextBuilder&) = delete;
    auto operator=(const ServerContextBuilder&) -> ServerContextBuilder& = delete;

public:
    auto build() const -> IServerContextPtr;

    // Mandatory options
    auto setVersion(ServerVersion) -> ServerContextBuilder&;
    auto setPort(Port) -> ServerContextBuilder&;
    auto setEventHandler(IEventHandlerPtr) -> ServerContextBuilder&;

    // Non mandatory options
    auto setPingPongInterval(int) -> ServerContextBuilder&;

private:
    std::unique_ptr<ServerContext> _serverContext;
};

} // namespace wspp::srv
