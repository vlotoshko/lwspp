/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace wspp::cli
{

class LwsContext
{
public:
    explicit LwsContext(const ClientContext&);

private:
    ILwsCallbackContextPtr _callbackContext;
    LwsDataHolderPtr _dataHolder;
    LowLevelContextPtr _lowLevelContext;
//    LwsInstanceRawPtr _lwsInstance;
};

} // namespace wspp::cli
