/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace wspp::srv
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
    ILwsSessionsPtr _sessions;
    ILwsCallbackContextPtr _callbackContext;

    LwsDataHolderPtr _dataHolder;
    LowLevelContextPtr _lowLevelContext;

    bool _stopListening = false;
};

} // namespace wspp::srv
