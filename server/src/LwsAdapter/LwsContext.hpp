/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <mutex>

#include "easywebsocket/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace ews::srv
{

class LwsContext
{
public:
    explicit LwsContext(const ServerContext&);

    // NOTE: the 'startListening' method blocks the thread
    void startListening();
    void stopListening();

    auto getPort() const -> Port;

private:
    ILwsCallbackContextPtr _callbackContext;
    LwsDataHolderPtr _dataHolder;
    LowLevelContextPtr _lowLevelContext;

    bool _isStarted = false;
    bool _stopListening = false;
    std::mutex _mutex;
};

} // namespace ews::srv
