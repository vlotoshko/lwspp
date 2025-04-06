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

#include "lwspp/client/CallbackVersions.hpp"
#include "lwspp/client/Types.hpp"
#include "lwspp/client/TypesFwd.hpp"

namespace lwspp
{
namespace cli
{

class ClientContext;

/**
 * @brief The ClientBuilder class is responsible for constructing client instances.
 * Some options are mandatory and must be set; otherwise, the builder will throw an exception.
 */
class ClientBuilder
{
public:
    ClientBuilder();
    ~ClientBuilder();

    ClientBuilder(ClientBuilder&&) noexcept;
    auto operator=(ClientBuilder&&) noexcept -> ClientBuilder&;

    ClientBuilder(const ClientBuilder&) = delete;
    auto operator=(const ClientBuilder&) -> ClientBuilder& = delete;

public:
    auto build() const -> IClientPtr;

    // Mandatory options
    auto setCallbackVersion(CallbackVersion) -> ClientBuilder&;
    auto setAddress(Address) -> ClientBuilder&;
    auto setPort(Port) -> ClientBuilder&;
    auto setClientLogic(contract::IClientLogicPtr) -> ClientBuilder&;
    auto setClientControlAcceptor(contract::IClientControlAcceptorPtr) -> ClientBuilder&;

    // Non mandatory options
    auto setProtocolName(std::string) -> ClientBuilder&;
    auto setPath(Path) -> ClientBuilder&;
    auto setSslSettings(SslSettingsPtr) -> ClientBuilder&;
    auto setKeepAliveTimeout(int) -> ClientBuilder&;
    auto setKeepAliveProbes(int) -> ClientBuilder&;
    auto setKeepAliveProbesInterval(int) -> ClientBuilder&;
    auto setLwsLogLevel(int) -> ClientBuilder&;

private:
    std::unique_ptr<ClientContext> _context;

    friend class TestClientBuilder;
};

} // namespace cli
} // namespace lwspp
