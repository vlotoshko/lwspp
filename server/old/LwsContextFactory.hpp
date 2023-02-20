/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "LwsAdapter/LwsContext.hpp"
#include "TypesFwd.hpp"

namespace wspp::srv
{

auto createLwsContext(const ServerContext&, CallbackContext&) -> LwsContext;

} // namespace wspp::srv
