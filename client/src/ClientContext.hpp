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

class ClientContext
{
public:
    ClientVersion clientVersion = UNDEFINED_CLIENT_VERSION;
    IEventHandlerPtr eventHandler;
    IMessageSenderAcceptorPtr messageSenderAcceptor;
    Address address = UNDEFINED_ADDRESS;
    Port port = UNDEFINED_PORT;

    Path path = DEFAULT_URI_PATH;
    std::string protocolName = DEFAULT_PROTOCOL_NAME;

    SslSettingsPtr ssl;
};

} // namespace ews::cli
