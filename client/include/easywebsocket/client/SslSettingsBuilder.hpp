/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

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
    auto setCiphersList(std::string) -> SslSettingsBuilder&;
    auto setCiphersListTls13(std::string) -> SslSettingsBuilder&;

    auto allowSelfSignedServerCert() -> SslSettingsBuilder&;
    auto allowExpiredServerCert() -> SslSettingsBuilder&;
    auto skipServerCertHostnameCheck() -> SslSettingsBuilder&;
    auto ignoreServerCaSert() -> SslSettingsBuilder&;

private:
    std::unique_ptr<SslSettings> _settings;
};

} // namespace ews::cli
