/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "LwsAdapter/LwsContext.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace wspp::srv
{

auto createLwsContext(const ServerContext&, ILwsCallbackContext&) -> LwsContext;

} // namespace wspp::srv
