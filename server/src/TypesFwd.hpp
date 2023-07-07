/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace ews::srv
{

class ServerContext;

class LwsServer;
using LwsServerPtr = std::shared_ptr<LwsServer>;

class SslSettings;
using SslSettingsPtr = std::shared_ptr<SslSettings>;

} // namespace ews::srv
