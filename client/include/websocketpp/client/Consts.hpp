/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

#include "websocketpp/client/Types.hpp"

namespace wspp::cli
{

const SessionId UNDEFINED_SESSION_ID = static_cast<int>(0U - 1);
const SessionId ALL_SESSIONS = static_cast<int>(0U - 2);

} // namespace wspp::cli
