/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/server/TypesFwd.hpp"

namespace wspp::srv
{

/**
 * @brief createServer creates the server instance using the server context
 * @param IServerContext - opaque context with the set of options for the server
 * @return server
 */
auto createServer(const IServerContext& c) -> IServerPtr;

} // namespace wspp::srv
