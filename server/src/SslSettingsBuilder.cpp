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

#include "easywebsocket/server/SslSettingsBuilder.hpp"

#include "SslSettings.hpp"

namespace ews::srv
{

SslSettingsBuilder::SslSettingsBuilder() : _settings(new SslSettings{})
{}

SslSettingsBuilder::~SslSettingsBuilder() = default;

SslSettingsBuilder::SslSettingsBuilder(SslSettingsBuilder&& that) noexcept
    : _settings(std::move(that._settings))
{}

auto SslSettingsBuilder::operator=(SslSettingsBuilder&& that) noexcept -> SslSettingsBuilder&
{
    if (this != &that)
    {
        _settings = std::move(that._settings);
    }
    return *this;
}

auto SslSettingsBuilder::build() const -> SslSettingsPtr
{
    return std::make_shared<SslSettings>(*_settings);
}

auto SslSettingsBuilder::setPrivateKeyFilepath(std::string path) -> SslSettingsBuilder&
{
    _settings->privateKeyPath = std::move(path);
    return *this;
}

auto SslSettingsBuilder::setCertFilepath(std::string path) -> SslSettingsBuilder&
{
    _settings->certPath = std::move(path);
    return *this;
}

auto SslSettingsBuilder::setCaCertFilepath(std::string path) -> SslSettingsBuilder&
{
    _settings->caCertPath = std::move(path);
    return *this;
}

auto SslSettingsBuilder::setPrivateKeyPassword(std::string password) -> SslSettingsBuilder&
{
    _settings->privateKeyPassword = std::move(password);
    return *this;
}

auto SslSettingsBuilder::requireValidClientCert() -> SslSettingsBuilder&
{
    _settings->requireValidClientCert = true;
    return *this;
}

auto SslSettingsBuilder::setCiphersList(std::string ciphersList) -> SslSettingsBuilder&
{
    _settings->ciphersList = std::move(ciphersList);
    return *this;
}

auto SslSettingsBuilder::setCiphersListTls13(std::string ciphersList) -> SslSettingsBuilder&
{
    _settings->ciphersListTls13 = std::move(ciphersList);
    return *this;
}

} // namespace ews::srv
