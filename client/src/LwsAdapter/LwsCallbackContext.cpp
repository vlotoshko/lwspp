/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/LwsCallbackContext.hpp"

namespace wspp::cli
{

LwsCallbackContext::LwsCallbackContext(IEventHandlerPtr e)
    : _eventHandler(std::move(e))
{}

auto LwsCallbackContext::getEventHandler() -> IEventHandlerPtr
{
    return _eventHandler;
}

} // namespace wspp::cli
