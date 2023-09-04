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

#pragma once

#include <string>

#include "easywebsockets/client/TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The SslSettingsBuilder class constructs SSL settings that can be utilized by the
 * client builder to build a client supporting SSL connections.
 */
class SslSettingsBuilder
{
public:
    SslSettingsBuilder();
    ~SslSettingsBuilder();

    SslSettingsBuilder(SslSettingsBuilder&&) noexcept;
    auto operator=(SslSettingsBuilder&&) noexcept -> SslSettingsBuilder&;

    SslSettingsBuilder(const SslSettingsBuilder&) = delete;
    auto operator=(const SslSettingsBuilder&) -> SslSettingsBuilder& = delete;

public:
    auto build() const -> SslSettingsPtr;

    auto setPrivateKeyFilepath(std::string) -> SslSettingsBuilder&;
    auto setCertFilepath(std::string) -> SslSettingsBuilder&;
    auto setCaCertFilepath(std::string) -> SslSettingsBuilder&;
    auto setPrivateKeyPassword(std::string) -> SslSettingsBuilder&;

    // For more information please look for the struct lws_context_creation_info description
    // List of valid ciphers to use ON TLS1.2 AND LOWER ONLY
    auto setCiphersList(std::string) -> SslSettingsBuilder&;
    // List of valid ciphers to use ON TLS1.3 AND ABOVE
    auto setCiphersListTls13(std::string) -> SslSettingsBuilder&;

    auto allowSelfSignedServerCert() -> SslSettingsBuilder&;
    auto allowExpiredServerCert() -> SslSettingsBuilder&;
    auto skipServerCertHostnameCheck() -> SslSettingsBuilder&;
    auto ignoreServerCaSert() -> SslSettingsBuilder&;

private:
    std::unique_ptr<SslSettings> _settings;
};

} // namespace ews::cli
