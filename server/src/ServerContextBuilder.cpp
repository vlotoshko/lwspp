/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <stdexcept>

#include "easywebsocket/server/ServerContextBuilder.hpp"
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
}

} // namespace


ServerContextBuilder::ServerContextBuilder() : _serverContext(new ServerContext{})
{}

ServerContextBuilder::~ServerContextBuilder() = default;

auto ServerContextBuilder::build() const -> IServerContextPtr
{
    const auto& context = *_serverContext;
    checkContext(context);
    return std::make_shared<ServerContext>(context);
}

auto ServerContextBuilder::setVersion(ServerVersion version) -> ServerContextBuilder&
{
    _serverContext->serverVersion = version;
    return *this;
}

auto ServerContextBuilder::setPort(Port port) -> ServerContextBuilder&
{
    _serverContext->port = port;
    return *this;
}

auto ServerContextBuilder::setEventHandler(IEventHandlerPtr e) -> ServerContextBuilder&
{
    _serverContext->eventHandler = std::move(e);
    return *this;
}

auto ServerContextBuilder::setPingPongInterval(int) -> ServerContextBuilder&
{
    return *this;
}

} // namespace ews::srv
