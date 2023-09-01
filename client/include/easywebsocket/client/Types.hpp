/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

#include <string>

namespace ews::cli
{

// TODO: consider strong types using
using Address = std::string;
using Port = int;
using Path = std::string;

enum class MessageType
{
    Text,
    Binary
};

using Message = std::pair<MessageType, std::string>;

} // namespace ews::cli
