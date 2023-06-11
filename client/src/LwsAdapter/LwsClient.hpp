/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include <condition_variable>
#include <mutex>

#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace ews::cli
{

class LwsClient
{
public:
    explicit LwsClient(const ClientContext&);
    ~LwsClient();

    LwsClient(LwsClient&&) = delete;
    auto operator=(LwsClient&&) noexcept -> LwsClient& = delete;

    LwsClient(const LwsClient&) = delete;
    auto operator=(const LwsClient&) noexcept -> LwsClient& = delete;

    // NOTE: the 'connect' method blocks the thread
    void connect();
    void disconnect();

private:
    void setupLowLevelContext_();
    void setupConnectionInfo_();
    void waitForClientStopping_();

private:
    enum class State
    {
        Initial,
        Started,
        Stopping,
        Stopped,
    };

    ILwsCallbackContextPtr _callbackContext;
    LwsDataHolderPtr _dataHolder;

    LowLevelContextPtr _lowLevelContext;
    LwsInstanceRawPtr _wsInstance = nullptr;
    LwsConnectInfo _lwsConnectionInfo;

    std::condition_variable _isStoppedCV;
    State _state = State::Initial;
    std::mutex _mutex;
};

} // namespace ews::cli
