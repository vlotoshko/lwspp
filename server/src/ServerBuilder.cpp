/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <stdexcept>

#include "easywebsocket/server/ServerBuilder.hpp"
#include "Server.hpp"
#include "ServerContext.hpp"

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
}

} // namespace


ServerBuilder::ServerBuilder() : _serverContext(new ServerContext{})
{}

ServerBuilder::~ServerBuilder() = default;

auto ServerBuilder::build() const -> IServerPtr
{
    const auto& context = *_serverContext;
    checkContext(context);
    return std::make_shared<Server>(context);
}

auto ServerBuilder::setVersion(ServerVersion version) -> ServerBuilder&
{
    _serverContext->serverVersion = version;
    return *this;
}

auto ServerBuilder::setPort(Port port) -> ServerBuilder&
{
    _serverContext->port = port;
    return *this;
}

auto ServerBuilder::setEventHandler(IEventHandlerPtr e) -> ServerBuilder&
{
    _serverContext->eventHandler = std::move(e);
    return *this;
}

auto ServerBuilder::setMessageSenderAcceptor(IMessageSenderAcceptorPtr a) -> ServerBuilder&
{
    _serverContext->messageSenderAcceptor = std::move(a);
    return *this;
}

auto ServerBuilder::setProtocolName(std::string protocolName) -> ServerBuilder&
{
    _serverContext->protocolName = std::move(protocolName);
    return *this;
}

auto ServerBuilder::setPingPongInterval(int) -> ServerBuilder&
{
    return *this;
}

} // namespace ews::srv
