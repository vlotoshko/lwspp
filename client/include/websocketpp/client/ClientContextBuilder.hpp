/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/client/ClientVersions.hpp"
#include "websocketpp/client/Types.hpp"
#include "websocketpp/client/TypesFwd.hpp"

namespace wspp::cli
{

class ClientContext;

/**
 * @brief The ClientContextBuilder class builds the context for the client factory to
 * create the client with desired options. Some options are mandatory to set. Builder
 * throws an exception if any of the mandatory options are not set.
 */
class ClientContextBuilder
{
public:
    ClientContextBuilder();
    ~ClientContextBuilder();

    ClientContextBuilder(ClientContextBuilder&&) noexcept = default;
    auto operator=(ClientContextBuilder&&) noexcept -> ClientContextBuilder& = default;

    ClientContextBuilder(const ClientContextBuilder&) = delete;
    auto operator=(const ClientContextBuilder&) -> ClientContextBuilder& = delete;

public:
    auto build() const -> IClientContextPtr;

    // Mandatory options
    auto setVersion(ClientVersion) -> ClientContextBuilder&;
    auto setAddress(Address) -> ClientContextBuilder&;
    auto setPort(Port) -> ClientContextBuilder&;
    auto setPath(Path) -> ClientContextBuilder&;
    auto setEventHandler(IEventHandlerPtr) -> ClientContextBuilder&;

    // Non mandatory options
    auto setPingPongInterval(int) -> ClientContextBuilder&;

private:
    std::unique_ptr<ClientContext> _clientContext;
};

} // namespace wspp::cli
