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

#pragma once

#include "lwspp/server/CallbackVersions.hpp"
#include "lwspp/server/Types.hpp"
#include "lwspp/server/TypesFwd.hpp"

namespace lwspp
{
namespace srv
{

class ServerContext;

/**
 * @brief The ServerBuilder class is responsible for constructing server instances.
 * Some options are mandatory and must be set; otherwise, the builder will throw an exception.
 */
class ServerBuilder
{
public:
    ServerBuilder();
    ~ServerBuilder();

    ServerBuilder(ServerBuilder&&) noexcept;
    auto operator=(ServerBuilder&&) noexcept -> ServerBuilder&;

    ServerBuilder(const ServerBuilder&) = delete;
    auto operator=(const ServerBuilder&) -> ServerBuilder& = delete;

public:
    auto build() const -> IServerPtr;

    // Mandatory options
    auto setCallbackVersion(CallbackVersion) -> ServerBuilder&;
    auto setPort(Port) -> ServerBuilder&;
    auto setEventHandler(IEventHandlerPtr) -> ServerBuilder&;
    auto setDataSenderAcceptor(IDataSenderAcceptorPtr) -> ServerBuilder&;

    // Non mandatory options
    auto setProtocolName(std::string) -> ServerBuilder&;
    auto setKeepAliveTimeout(int) -> ServerBuilder&;
    auto setKeepAliveProbes(int) -> ServerBuilder&;
    auto setKeepAliveProbesInterval(int) -> ServerBuilder&;
    auto setSslSettings(SslSettingsPtr) -> ServerBuilder&;
    auto setVhostName(std::string) -> ServerBuilder&;
    auto setServerString(std::string) -> ServerBuilder&;
    auto setLwsLogLevel(int) -> ServerBuilder&;


private:
    std::unique_ptr<ServerContext> _context;

    friend class TestServerBuilder;
};

} // namespace srv
} // namespace lwspp
