/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <condition_variable>
#include <mutex>

#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace ews::srv
{

class LwsServer
{
public:
    explicit LwsServer(const ServerContext&);
    ~LwsServer();

    LwsServer(LwsServer&&) noexcept = delete;
    auto operator=(LwsServer&&) noexcept -> LwsServer& = delete;

    LwsServer(const LwsServer&) = delete;
    auto operator=(const LwsServer&) -> LwsServer& = delete;

    // NOTE: the 'startListening' method blocks the thread
    void startListening();

private:
    void waitForServerStopped_();

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

    std::condition_variable _isStoppedCV;
    State _state = State::Initial;
    std::mutex _mutex;
};

} // namespace ews::srv
