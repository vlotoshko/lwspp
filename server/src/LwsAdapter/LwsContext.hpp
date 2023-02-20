/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "LwsAdapter/LwsDataHolder.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace wspp::srv
{

struct LwsContext
{
    explicit LwsContext(const ServerContext&, ICallbackContext&);

    LwsDataHolder dataHolder;
    LowLevelContext lowLeveContext;
};

} // namespace wspp::srv
