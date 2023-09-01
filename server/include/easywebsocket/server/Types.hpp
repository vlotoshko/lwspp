/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>

namespace ews::srv
{

// TODO: consider strong types using
using Port = int;
using SessionId = int;
using Path = std::string;

enum class MessageType
{
    Text,
    Binary
};

using Message = std::pair<MessageType, std::string>;

} // namespace ews::srv
