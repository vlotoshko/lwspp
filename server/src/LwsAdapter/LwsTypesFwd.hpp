/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <libwebsockets.h>
#include <memory>
#include <vector>

#include "LwsAdapter/LwsContextDeleter.hpp"

namespace wspp::srv
{

using LowLevelContext = std::unique_ptr<lws_context, LwsContextDeleter>;
using LwsProtocols = std::vector<lws_protocols>;
using LwsCallback = lws_callback_function;

} // namespace wspp::srv
