/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/client/TypesFwd.hpp"
#include "Consts.hpp"

namespace ews::cli
{

/**
 * @brief The ClientContext class is the 'high-level' context to create the
 * 'low-level' lws_context of the libwebsocket
 */
class ClientContext
{
public:
    CallbackVersion callbackVersion = UNDEFINED_CALLBACK_VERSION;
    IEventHandlerPtr eventHandler;
    IDataSenderAcceptorPtr dataSenderAcceptor;
    Address address = UNDEFINED_ADDRESS;
    Port port = UNDEFINED_PORT;

    Path path = DEFAULT_URI_PATH;
    std::string protocolName = DEFAULT_PROTOCOL_NAME;
    int keepAliveTimeout = UNDEFINED_UNSET;
    int keepAliveProbes = UNDEFINED_UNSET;
    int keepAliveProbesInterval = UNDEFINED_UNSET;
    int lwsLogLevel = DEFAULT_LWS_LOG_LEVEL;
    SslSettingsPtr ssl;
};

} // namespace ews::cli
