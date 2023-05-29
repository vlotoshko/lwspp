/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <stdexcept>

#include "websocketpp/client/ClientContextBuilder.hpp"
#include "ClientContext.hpp"

namespace wspp::cli
{

namespace
{

class UndefinedRequiredParameterException : public std::runtime_error
{
public:
    explicit UndefinedRequiredParameterException(const std::string& parameter)
        : std::runtime_error("Required parameter is undefined: " + parameter)
    {}
};

void checkContext(const ClientContext& context)
{
    if (context.clientVersion == UNDEFINED_CLIENT_VERSION)
    {
        throw UndefinedRequiredParameterException{"client version"};
    }

    if (context.port == UNDEFINED_PORT)
    {
        throw UndefinedRequiredParameterException{"port"};
    }

    if (context.eventHandler == nullptr)
    {
        throw UndefinedRequiredParameterException{"event handler"};
    }
}

} // namespace


ClientContextBuilder::ClientContextBuilder() : _clientContext(new ClientContext{})
{}

ClientContextBuilder::~ClientContextBuilder() = default;

auto ClientContextBuilder::build() const -> IClientContextPtr
{

    const auto& context = *_clientContext;
    checkContext(context);
    return std::make_shared<ClientContext>(*_clientContext);
}

auto ClientContextBuilder::setEventHandler(IEventHandlerPtr e) -> ClientContextBuilder&
{
    _clientContext->eventHandler = std::move(e);
    return *this;
}

auto ClientContextBuilder::setPath(Path path) -> ClientContextBuilder&
{
    _clientContext->path = std::move(path);
    return *this;
}

auto ClientContextBuilder::setPort(Port port) -> ClientContextBuilder&
{
    _clientContext->port = port;
    return *this;
}

auto ClientContextBuilder::setAddress(Address address) -> ClientContextBuilder&
{
    _clientContext->address = std::move(address);
    return *this;
}

auto ClientContextBuilder::setVersion(ClientVersion version) -> ClientContextBuilder&
{
    _clientContext->clientVersion = version;
    return *this;
}

} // namespace wspp::cli
