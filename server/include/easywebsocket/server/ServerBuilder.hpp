/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "easywebsocket/server/CallbackVersions.hpp"
#include "easywebsocket/server/Types.hpp"
#include "easywebsocket/server/TypesFwd.hpp"

namespace ews::srv
{

class ServerContext;

/**
 * @brief The ServerBuilder class builds the server instance. Some options are mandatory to set.
 *  Builder throws an exception if any of the mandatory options are not set.
 */
class ServerBuilder
{
public:
    ServerBuilder();
    ~ServerBuilder();

    ServerBuilder(ServerBuilder&&) noexcept;
    auto operator=(ServerBuilder&&) noexcept -> ServerBuilder&;

    ServerBuilder(const ServerBuilder&) = delete;
    auto operator=(const ServerBuilder&) -> ServerBuilder& = delete;

public:
    auto build() const -> IServerPtr;

    // Mandatory options
    auto setCallbackVersion(CallbackVersion) -> ServerBuilder&;
    auto setPort(Port) -> ServerBuilder&;
    auto setEventHandler(IEventHandlerPtr) -> ServerBuilder&;
    auto setMessageSenderAcceptor(IMessageSenderAcceptorPtr) -> ServerBuilder&;

    // Non mandatory options
    auto setProtocolName(std::string) -> ServerBuilder&;
    auto setKeepAliveTimeout(int) -> ServerBuilder&;
    auto setKeepAliveProbes(int) -> ServerBuilder&;
    auto setKeepAliveProbesInterval(int) -> ServerBuilder&;
    auto setSslSettings(SslSettingsPtr) -> ServerBuilder&;
    auto setVhostName(std::string) -> ServerBuilder&;
    auto setServerString(std::string) -> ServerBuilder&;
    auto setLwsLogLevel(int) -> ServerBuilder&;


private:
    std::unique_ptr<ServerContext> _context;

    friend class TestServerBuilder;
};

} // namespace ews::srv
