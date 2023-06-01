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

    void connect();
    void disconnect();

private:
    void setupLowLeverContext();
    void setupConnectionInfo();

private:
    ILwsCallbackContextPtr _callbackContext;
    LwsDataHolderPtr _dataHolder;

    LowLevelContextPtr _lowLevelContext;
    LwsInstanceRawPtr _wsInstance = nullptr;
    LwsConnectInfo _lwsConnectionInfo;

    bool _isStopping = false;
};

} // namespace wspp::cli
