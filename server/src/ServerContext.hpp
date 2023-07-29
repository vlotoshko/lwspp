/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/TypesFwd.hpp"
#include "Consts.hpp"

namespace ews::srv
{

/**
 * @brief The ServerContext class is the 'high-level' context to create the
 * 'low-level' lws_context of the libwebsocket
 */
class ServerContext
{
public:
    IEventHandlerPtr eventHandler;
    IMessageSenderAcceptorPtr messageSenderAcceptor;
    ServerVersion serverVersion = UNDEFINED_SERVER_VERSION;
    Port port = UNDEFINED_PORT;

    std::string protocolName = DEFAULT_PROTOCOL_NAME;
    int keepAliveTimeout = UNDEFINED_UNSET;
    int keepAliveProbes = UNDEFINED_UNSET;
    int keepAliveProbesInterval = UNDEFINED_UNSET;
    std::string vhostName = UNDEFINED_NAME;
    std::string serverString = UNDEFINED_NAME;
    int lwsLogLevel = UNDEFINED_UNSET;
    SslSettingsPtr ssl;
};

} // namespace ews::srv
