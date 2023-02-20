/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "websocketpp/client/ClientContextBuilder.hpp"
#include "ClientContext.hpp"

namespace wspp::cli
{

ClientContextBuilder::ClientContextBuilder(int port) : _clientContext(new ClientContext{})
{
    _clientContext->port = port;
}

ClientContextBuilder::~ClientContextBuilder() = default;

auto ClientContextBuilder::build() const -> IClientContextPtr
{
    return std::make_shared<ClientContext>(*_clientContext);
}

auto ClientContextBuilder::setBehavior(std::string b) -> ClientContextBuilder&
{
    _clientContext->behavior = std::move(b);
    return *this;
}

auto ClientContextBuilder::setLogging(int i) -> ClientContextBuilder&
{
    _clientContext->loglevel = i;
    return *this;
}

} // namespace wspp::cli
