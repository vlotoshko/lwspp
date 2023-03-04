/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/Types.hpp"

namespace wspp::srv
{

const SessionId UNDEFINED_SESSION_ID = static_cast<int>(0U - 1);
const SessionId ALL_SESSIONS = static_cast<int>(0U - 2);

} // namespace wspp::srv
