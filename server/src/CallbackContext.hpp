/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "ICallbackContext.hpp"

namespace wspp::srv
{

class CallbackContext : public ICallbackContext
{
public:
    CallbackContext(IEventHandlerPtr, ISessionsPtr);

    void setServerStopped() override;
    auto isServerStopped() const -> bool override;
    auto getEventHandler() -> IEventHandlerPtr override;
    auto getSessions() -> ISessionsPtr override;

private:
    bool _isStopped = false;
    IEventHandlerPtr _eventHandler;
    ISessionsPtr _sessions;
};

} // namespace wspp::srv
