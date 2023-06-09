/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <functional>
#include <string>

#include "easywebsocket/server/ServerVersions.hpp"
#include "easywebsocket/server/TypesFwd.hpp"
#include "Consts.hpp"
#include "IServerContext.hpp"

namespace ews::srv
{

/**
 * @brief The ServerContext class is the 'high-level' context to create the
 * 'low-level' lws_context of the libwebsocket
 */
class ServerContext : public IServerContext
{
public:
    void accept(IContextVisitor&) const override;

    IEventHandlerPtr eventHandler;
    ServerVersion serverVersion = UNDEFINED_SERVER_VERSION;
    Port port = UNDEFINED_PORT;

    std::string protocolName = DEFAULT_PROTOCOL_NAME;
};

} // namespace ews::srv
