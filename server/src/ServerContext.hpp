/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <functional>
#include <string>

#include "websocketpp/server/ServerVersions.hpp"
#include "websocketpp/server/TypesFwd.hpp"
#include "Consts.hpp"
#include "IServerContext.hpp"

namespace wspp::srv
{

/**
 * @brief The ServerContext class is the 'high-level' context to create the
 * 'low-level' lws_context of the libwebsocket
 */
class ServerContext : public IServerContext
{
public:
    void accept(IContextVisitor&) const override;

    ServerVersion serverVersion = UNDEFINED_SERVER_VERSION;
    Port port = UNDEFINED_PORT;
    IEventHandlerPtr eventHandler;
};

} // namespace wspp::srv
