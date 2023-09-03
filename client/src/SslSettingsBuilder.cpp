/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#include "easywebsocket/client/SslSettingsBuilder.hpp"

#include "SslSettings.hpp"

namespace ews::cli
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

auto SslSettingsBuilder::allowSelfSignedServerCert() -> SslSettingsBuilder&
{
    _settings->allowSelfSignedServerCert = true;
    return *this;
}

auto SslSettingsBuilder::allowExpiredServerCert() -> SslSettingsBuilder&
{
    _settings->allowExpiredServerCert = true;
    return *this;
}

auto SslSettingsBuilder::skipServerCertHostnameCheck() -> SslSettingsBuilder&
{
    _settings->skipServerCertHostnameCheck = true;
    return *this;
}

auto SslSettingsBuilder::ignoreServerCaSert() -> SslSettingsBuilder&
{
    _settings->ignoreServerCaSert = true;
    return *this;
}

} // namespace ews::cli
