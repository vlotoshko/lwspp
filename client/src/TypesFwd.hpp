/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace ews::cli
{

class ClientContext;

class LwsClient;
using LwsClientPtr = std::shared_ptr<LwsClient>;

class SslSettings;
using SslSettingsPtr = std::shared_ptr<SslSettings>;

} // namespace ews::cli
