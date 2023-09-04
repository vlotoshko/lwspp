/*
 * EasyWebsockets - C++ wrapper for the libwebsockets library
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
    if (context.callbackVersion == UNDEFINED_CALLBACK_VERSION)
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

auto ClientBuilder::setDataSenderAcceptor(IDataSenderAcceptorPtr a) -> ClientBuilder&
{
    _context->dataSenderAcceptor = std::move(a);
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

auto ClientBuilder::setCallbackVersion(CallbackVersion version) -> ClientBuilder&
{
    _context->callbackVersion = version;
    return *this;
}

auto ClientBuilder::setSslSettings(SslSettingsPtr ssl) -> ClientBuilder&
{
    _context->ssl = std::move(ssl);
    return *this;
}

auto ClientBuilder::setKeepAliveTimeout(int timeout) -> ClientBuilder&
{
    _context->keepAliveTimeout = timeout;
    return *this;
}

auto ClientBuilder::setKeepAliveProbes(int probes) -> ClientBuilder&
{
    _context->keepAliveProbes = probes;
    return *this;
}

auto ClientBuilder::setKeepAliveProbesInterval(int interval) -> ClientBuilder&
{
    _context->keepAliveProbesInterval = interval;
    return *this;
}

auto ClientBuilder::setLwsLogLevel(int logLevel) -> ClientBuilder&
{
    _context->lwsLogLevel = logLevel;
    return *this;
}

} // namespace ews::cli
