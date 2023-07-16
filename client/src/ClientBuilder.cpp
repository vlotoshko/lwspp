/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <stdexcept>

#include "easywebsocket/client/ClientBuilder.hpp"
#include "Client.hpp"
#include "ClientContext.hpp"
#include "SslSettings.hpp"

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

    if (context.ssl != nullptr &&
        context.ssl->privateKeyPath != UNDEFINED_FILE_PATH &&
        context.ssl->certPath == UNDEFINED_FILE_PATH)
    {
        throw UndefinedRequiredParameterException{"ssl certificate path"};
    }
}

} // namespace


ClientBuilder::ClientBuilder() : _context(new ClientContext{})
{}

ClientBuilder::~ClientBuilder() = default;

ClientBuilder::ClientBuilder(ClientBuilder&& that) noexcept
    : _context(std::move(that._context))
{}

auto ClientBuilder::operator=(ClientBuilder&& that) noexcept -> ClientBuilder&
{
    if (this != &that)
    {
        _context = std::move(that._context);
    }
    return *this;
}

auto ClientBuilder::build() const -> IClientPtr
{
    const auto& context = *_context;
    checkContext(context);
    return std::make_shared<Client>(context);
}

auto ClientBuilder::setEventHandler(IEventHandlerPtr e) -> ClientBuilder&
{
    _context->eventHandler = std::move(e);
    return *this;
}

auto ClientBuilder::setMessageSenderAcceptor(IMessageSenderAcceptorPtr a) -> ClientBuilder&
{
    _context->messageSenderAcceptor = std::move(a);
    return *this;
}

auto ClientBuilder::setProtocolName(std::string protocolName) -> ClientBuilder&
{
    _context->protocolName = std::move(protocolName);
    return *this;
}

auto ClientBuilder::setPath(Path path) -> ClientBuilder&
{
    _context->path = std::move(path);
    return *this;
}

auto ClientBuilder::setPort(Port port) -> ClientBuilder&
{
    _context->port = port;
    return *this;
}

auto ClientBuilder::setAddress(Address address) -> ClientBuilder&
{
    _context->address = std::move(address);
    return *this;
}

auto ClientBuilder::setVersion(ClientVersion version) -> ClientBuilder&
{
    _context->clientVersion = version;
    return *this;
}

auto ClientBuilder::setSslSettings(SslSettingsPtr ssl) -> ClientBuilder&
{
    _context->ssl = std::move(ssl);
    return *this;
}

} // namespace ews::cli
