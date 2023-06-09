/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <mutex>

#pragma once

#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace ews::cli
{

class LwsContext
{
public:
    explicit LwsContext(const ClientContext&);
    ~LwsContext();

    LwsContext(LwsContext&&) = delete;
    auto operator=(LwsContext&&) noexcept -> LwsContext& = delete;

    LwsContext(const LwsContext&) = delete;
    auto operator=(const LwsContext&) noexcept -> LwsContext& = delete;

    void connect();

private:
    void setupLowLevelContext();
    void setupConnectionInfo();

private:
    ILwsCallbackContextPtr _callbackContext;
    LwsDataHolderPtr _dataHolder;

    LowLevelContextPtr _lowLevelContext;
    LwsInstanceRawPtr _wsInstance = nullptr;
    LwsConnectInfo _lwsConnectionInfo;

    bool _isConnected = false;
    bool _isStopping = false;
    std::mutex _mutex;
};

} // namespace ews::cli
