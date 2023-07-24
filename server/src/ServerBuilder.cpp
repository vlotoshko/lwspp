/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <stdexcept>

#include "easywebsocket/server/ServerBuilder.hpp"
#include "Server.hpp"
#include "ServerContext.hpp"
#include "SslSettings.hpp"

namespace ews::srv
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

void checkContext(const ServerContext& context)
{
    if (context.serverVersion == UNDEFINED_SERVER_VERSION)
    {
        throw UndefinedRequiredParameterException{"server version"};
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

    if (context.keepAliveTimeout != UNDEFINED_UNSET)
    {
        if (context.keepAliveProbes == UNDEFINED_UNSET)
        {
            throw UndefinedRequiredParameterException{"keep alive probes"};
        }

        if (context.keepAliveProbesInterval == UNDEFINED_UNSET)
        {
            throw UndefinedRequiredParameterException{"keep alive probes interval"};
        }
    }
}

} // namespace


ServerBuilder::ServerBuilder() : _context(new ServerContext{})
{}

ServerBuilder::~ServerBuilder() = default;

ServerBuilder::ServerBuilder(ServerBuilder&& that) noexcept
    : _context(std::move(that._context))
{}

auto ServerBuilder::operator=(ServerBuilder&& that) noexcept -> ServerBuilder&
{
    if (this != &that)
    {
        _context = std::move(that._context);
    }
    return *this;
}

auto ServerBuilder::build() const -> IServerPtr
{
    const auto& context = *_context;
    checkContext(context);
    return std::make_shared<Server>(context);
}

auto ServerBuilder::setVersion(ServerVersion version) -> ServerBuilder&
{
    _context->serverVersion = version;
    return *this;
}

auto ServerBuilder::setPort(Port port) -> ServerBuilder&
{
    _context->port = port;
    return *this;
}

auto ServerBuilder::setEventHandler(IEventHandlerPtr e) -> ServerBuilder&
{
    _context->eventHandler = std::move(e);
    return *this;
}

auto ServerBuilder::setMessageSenderAcceptor(IMessageSenderAcceptorPtr a) -> ServerBuilder&
{
    _context->messageSenderAcceptor = std::move(a);
    return *this;
}

auto ServerBuilder::setProtocolName(std::string protocolName) -> ServerBuilder&
{
    _context->protocolName = std::move(protocolName);
    return *this;
}

auto ServerBuilder::setSslSettings(SslSettingsPtr ssl) -> ServerBuilder&
{
    _context->ssl = std::move(ssl);
    return *this;
}

auto ServerBuilder::setKeepAliveTimeout(int timeout) -> ServerBuilder&
{
    _context->keepAliveTimeout = timeout;
    return *this;
}

auto ServerBuilder::setKeepAliveProbes(int probes) -> ServerBuilder&
{
    _context->keepAliveProbes = probes;
    return *this;
}

auto ServerBuilder::setKeepAliveProbesInterval(int interval) -> ServerBuilder&
{
    _context->keepAliveProbesInterval = interval;
    return *this;
}

} // namespace ews::srv
