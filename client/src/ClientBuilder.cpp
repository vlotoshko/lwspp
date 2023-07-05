/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <stdexcept>

#include "easywebsocket/client/ClientBuilder.hpp"
#include "Client.hpp"
#include "ClientContext.hpp"

namespace ews::cli
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

    if (context.address == UNDEFINED_ADDRESS)
    {
        throw UndefinedRequiredParameterException{"address"};
    }

    if (context.port == UNDEFINED_PORT)
    {
        throw UndefinedRequiredParameterException{"port"};
    }

    if (context.eventHandler == nullptr)
    {
        throw UndefinedRequiredParameterException{"event handler"};
    }

    if (context.messageSenderAcceptor == nullptr)
    {
        throw UndefinedRequiredParameterException{"message sender acceptor"};
    }
}

} // namespace


ClientBuilder::ClientBuilder() : _clientContext(new ClientContext{})
{}

ClientBuilder::~ClientBuilder() = default;

auto ClientBuilder::build() const -> IClientPtr
{
    const auto& context = *_clientContext;
    checkContext(context);
    return std::make_shared<Client>(context);
}

auto ClientBuilder::setEventHandler(IEventHandlerPtr e) -> ClientBuilder&
{
    _clientContext->eventHandler = std::move(e);
    return *this;
}

auto ClientBuilder::setMessageSenderAcceptor(IMessageSenderAcceptorPtr a) -> ClientBuilder&
{
    _clientContext->messageSenderAcceptor = std::move(a);
    return *this;
}

auto ClientBuilder::setProtocolName(std::string protocolName) -> ClientBuilder&
{
    _clientContext->protocolName = std::move(protocolName);
    return *this;
}

auto ClientBuilder::setPath(Path path) -> ClientBuilder&
{
    _clientContext->path = std::move(path);
    return *this;
}

auto ClientBuilder::enableSsl() -> ClientBuilder&
{
    _clientContext->enableSsl = true;
    return *this;
}

auto ClientBuilder::setPort(Port port) -> ClientBuilder&
{
    _clientContext->port = port;
    return *this;
}

auto ClientBuilder::setAddress(Address address) -> ClientBuilder&
{
    _clientContext->address = std::move(address);
    return *this;
}

auto ClientBuilder::setVersion(ClientVersion version) -> ClientBuilder&
{
    _clientContext->clientVersion = version;
    return *this;
}

} // namespace ews::cli
