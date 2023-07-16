/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "Consts.hpp"

namespace ews::srv
{

class SslSettings
{
public:
    std::string privateKeyPath = UNDEFINED_FILE_PATH;
    std::string certPath = UNDEFINED_FILE_PATH;
    std::string caCertPath = UNDEFINED_FILE_PATH;
    bool requireValidClientCert = false;
    std::string privateKeyPassword;
    std::string ciphersList;
    std::string ciphersListTls13;
};

} // namespace ews::srv
