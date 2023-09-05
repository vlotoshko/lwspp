/*
 * lwspp - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "LwsAdapter/LwsCallbackContext.hpp"

namespace ews::srv
{

LwsCallbackContext::LwsCallbackContext(IEventHandlerPtr e, ILwsSessionsPtr s)
    : _eventHandler(std::move(e))
    , _sessions(std::move(s))
{}

void LwsCallbackContext::setStopping()
{
    _isStopping = true;
}

auto LwsCallbackContext::isStopping() const -> bool
{
    return _isStopping;
}

auto LwsCallbackContext::getSessions() -> ILwsSessionsPtr
{
    return _sessions;
}

auto LwsCallbackContext::getEventHandler() -> IEventHandlerPtr
{
    return _eventHandler;
}


} // namespace ews::srv
