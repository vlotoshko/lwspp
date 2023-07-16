/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "Consts.hpp"

namespace ews::cli
{

class SslSettings
{
public:
    std::string privateKeyPath = UNDEFINED_FILE_PATH;
    std::string certPath = UNDEFINED_FILE_PATH;
    std::string caCertPath = UNDEFINED_FILE_PATH;
    std::string privateKeyPassword;
    std::string ciphersList;
    std::string ciphersListTls13;

    bool allowSelfSignedServerCert = false;
    bool allowExpiredServerCert = false;
    bool skipServerCertHostnameCheck = false;
    bool ignoreServerCaSert = false;
};

} // namespace ews::cli
