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

#include "lwspp/client/EventHandlerBase.hpp"

namespace lwspp
{
namespace cli
{

void EventHandlerBase::onConnect(IConnectionInfoPtr) noexcept
{}

void EventHandlerBase::onDisconnect() noexcept
{}

void EventHandlerBase::onFirstDataPacket(size_t) noexcept
{}

void EventHandlerBase::onBinaryDataReceive(const DataPacket&) noexcept
{}

void EventHandlerBase::onTextDataReceive(const DataPacket&) noexcept
{}

void EventHandlerBase::onError(const std::string& /*errorMessage*/) noexcept
{}

void EventHandlerBase::onWarning(const std::string& /*warningMessage*/) noexcept
{}

void EventHandlerBase::acceptActor(IActorPtr actor) noexcept
{
    _actor = std::move(actor);
}

} // namespace cli
} // namespace lwspp
