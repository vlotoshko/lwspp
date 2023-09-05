/*
 * lwspp - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdexcept>

#include "lwspp/server/ServerBuilder.hpp"
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
    if (context.callbackVersion == UNDEFINED_CALLBACK_VERSION)
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

    if (context.dataSenderAcceptor == nullptr)
    {
        throw UndefinedRequiredParameterException{"data sender acceptor"};
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

auto ServerBuilder::setCallbackVersion(CallbackVersion version) -> ServerBuilder&
{
    _context->callbackVersion = version;
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

auto ServerBuilder::setDataSenderAcceptor(IDataSenderAcceptorPtr a) -> ServerBuilder&
{
    _context->dataSenderAcceptor = std::move(a);
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

auto ServerBuilder::setVhostName(std::string vhostName) -> ServerBuilder&
{
    _context->vhostName = std::move(vhostName);
    return *this;
}

auto ServerBuilder::setServerString(std::string serverString) -> ServerBuilder&
{
    _context->serverString = std::move(serverString);
    return *this;
}

auto ServerBuilder::setLwsLogLevel(int logLevel) -> ServerBuilder&
{
    _context->lwsLogLevel = logLevel;
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
