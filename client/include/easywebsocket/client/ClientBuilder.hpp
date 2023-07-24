/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "easywebsocket/client/ClientVersions.hpp"
#include "easywebsocket/client/Types.hpp"
#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

class ClientContext;

/**
 * @brief The ClientBuilder class builds the client instance. Some options are mandatory to set.
 *  Builder throws an exception if any of the mandatory options are not set.
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
    auto setVersion(ClientVersion) -> ClientBuilder&;
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

private:
    std::unique_ptr<ClientContext> _context;

    friend class TestClientBuilder;
};

} // namespace ews::cli
