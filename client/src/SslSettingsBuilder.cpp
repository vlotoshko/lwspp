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

// namespace


} // namespace ews::cli
