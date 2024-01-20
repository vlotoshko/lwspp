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

#include <stdexcept>

#include "lwspp/client/IActorAcceptor.hpp"

#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsActor.hpp"

namespace lwspp
{
namespace cli
{

LwsCallbackContext::LwsCallbackContext(IEventHandlerPtr e, IActorAcceptorPtr a)
    : _eventHandler(std::move(e))
    , _actorAcceptor(std::move(a))
{}

void LwsCallbackContext::setStopping()
{
    _isStopping = true;
}

auto LwsCallbackContext::isStopping() const -> bool
{
    return _isStopping;
}

auto LwsCallbackContext::getEventHandler() -> IEventHandlerPtr
{
    return _eventHandler;
}

auto LwsCallbackContext::getConnection() -> ILwsConnectionPtr
{
    return _connection;
}

void LwsCallbackContext::setConnection(ILwsConnectionPtr s)
{
    if (s == nullptr)
    {
        throw std::runtime_error{"unaintialized lws connection found"};
    }
    _connection = std::move(s);

    auto actor = std::make_shared<LwsActor>(_connection);
    _actorAcceptor->acceptActor(std::move(actor));
}

void LwsCallbackContext::resetConnection()
{
    _connection.reset();
}

} // namespace cli
} // namespace lwspp
