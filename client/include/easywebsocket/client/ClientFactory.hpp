/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief createClient creates the client instance using the client context
 * @param IClientContext - opaque context with the set of options for the client
 * @return client
 */
auto createClient(const IClientContext&) -> IClientPtr;

} // namespace ews::cli
