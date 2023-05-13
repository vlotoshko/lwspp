/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/client/TypesFwd.hpp"

namespace wspp::cli
{

/**
 * @brief createClient creates the client instance using the client context
 * @param IClientContext - opaque context with the set of options for the client
 * @return client
 */
auto createClient(const IClientContext&) -> IClientPtr;

} // namespace wspp::cli
