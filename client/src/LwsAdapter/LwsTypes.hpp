/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Sep, 2023
 */

#pragma once

#include <string>

namespace ews::cli
{

enum class DataType
{
    Text,
    Binary
};

using Message = std::pair<DataType, std::string>;

} // namespace ews::cli
