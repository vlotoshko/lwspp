/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/TypesFwd.hpp"

namespace ews::srv
{
/**
 * @brief The SslSettingsBuilder class constructs SSL settings that can be utilized by the
 * server builder to build a server supporting SSL connections.
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
    auto requireValidClientCert() -> SslSettingsBuilder&;

    // For more information please look for the struct lws_context_creation_info description
    // List of valid ciphers to use ON TLS1.2 AND LOWER ONLY
    auto setCiphersList(std::string) -> SslSettingsBuilder&;
    // List of valid ciphers to use ON TLS1.3 AND ABOVE
    auto setCiphersListTls13(std::string) -> SslSettingsBuilder&;

private:
    std::unique_ptr<SslSettings> _settings;
};

} // namespace ews::srv
