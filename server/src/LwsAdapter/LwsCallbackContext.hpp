/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "LwsAdapter/ILwsCallbackContext.hpp"

namespace ews::srv
{

class LwsCallbackContext : public ILwsCallbackContext
{
public:
    LwsCallbackContext(IEventHandlerPtr, ILwsSessionsPtr);

    void setStopping() override;
    auto isStopping() const -> bool override;
    auto getSessions() -> ILwsSessionsPtr override;

    auto getEventHandler() -> IEventHandlerPtr override;

private:
    IEventHandlerPtr _eventHandler;
    ILwsSessionsPtr _sessions;

    bool _isStopping = false;
};

} // namespace ews::srv
