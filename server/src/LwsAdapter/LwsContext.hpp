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


enum class State
{
    Initial,
    Started,
    Stopping,
    Stopped,
};

class LwsContext
{
public:
    explicit LwsContext(const ServerContext&);
    ~LwsContext();

    // NOTE: the 'startListening' method blocks the thread
    void startListening();

private:
    void waitForServerStopped_();


private:
    ILwsCallbackContextPtr _callbackContext;
    LwsDataHolderPtr _dataHolder;
    LowLevelContextPtr _lowLevelContext;

    std::condition_variable _isStoppedCV;
    State _state = State::Initial;
    std::mutex _mutex;
};

} // namespace ews::srv
