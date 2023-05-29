/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "LwsAdapter/ILwsCallbackContext.hpp"

namespace wspp::cli
{

class LwsCallbackContext : public ILwsCallbackContext
{
public:
    LwsCallbackContext(IEventHandlerPtr);

    auto getEventHandler() -> IEventHandlerPtr override;

private:
    IEventHandlerPtr _eventHandler;
};

} // namespace wspp::cli
