/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "LwsAdapter/ILwsCallbackContext.hpp"

namespace ews::cli
{

class LwsCallbackContext : public ILwsCallbackContext
{
public:
    LwsCallbackContext(IEventHandlerPtr, IDataSenderAcceptorPtr);

    void setStopping() override;
    auto isStopping() const -> bool override;

    auto getSession() -> ILwsSessionPtr override;
    void setSession(ILwsSessionPtr) override;
    void resetSession() override;

    auto getEventHandler() -> IEventHandlerPtr override;

private:
    IEventHandlerPtr _eventHandler;
    IDataSenderAcceptorPtr _dataSenderAcceptor;
    ILwsSessionPtr _session;

    bool _isStopping = false;
};

} // namespace ews::cli
