/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "easywebsocket/client/CallbackVersions.hpp"
#include "easywebsocket/client/Types.hpp"
#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

class ClientContext;

/**
 * @brief The ClientBuilder class is responsible for constructing client instances.
 * Some options are mandatory and must be set; otherwise, the builder will throw an exception.
 */
class ClientBuilder
{
public:
    ClientBuilder();
    ~ClientBuilder();

    ClientBuilder(ClientBuilder&&) noexcept;
    auto operator=(ClientBuilder&&) noexcept -> ClientBuilder&;

    ClientBuilder(const ClientBuilder&) = delete;
    auto operator=(const ClientBuilder&) -> ClientBuilder& = delete;

public:
    auto build() const -> IClientPtr;

    // Mandatory options
    auto setCallbackVersion(CallbackVersion) -> ClientBuilder&;
    auto setAddress(Address) -> ClientBuilder&;
    auto setPort(Port) -> ClientBuilder&;
    auto setEventHandler(IEventHandlerPtr) -> ClientBuilder&;
    auto setMessageSenderAcceptor(IMessageSenderAcceptorPtr) -> ClientBuilder&;

    // Non mandatory options
    auto setProtocolName(std::string) -> ClientBuilder&;
    auto setPath(Path) -> ClientBuilder&;
    auto setSslSettings(SslSettingsPtr) -> ClientBuilder&;
    auto setKeepAliveTimeout(int) -> ClientBuilder&;
    auto setKeepAliveProbes(int) -> ClientBuilder&;
    auto setKeepAliveProbesInterval(int) -> ClientBuilder&;
    auto setLwsLogLevel(int) -> ClientBuilder&;

private:
    std::unique_ptr<ClientContext> _context;

    friend class TestClientBuilder;
};

} // namespace ews::cli
