/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "types/TypesFwd.hpp"

namespace wspp::cli
{

auto createClient(const IClientContext&) -> IClientPtr;

} // namespace wspp::cli
