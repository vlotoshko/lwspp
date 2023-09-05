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

#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsSessions.hpp"

namespace ews::srv
{

void LwsSessions::add(ILwsSessionPtr session)
{
    const std::lock_guard<std::mutex> guard(_mutex);
    _sessions.insert({session->getSessionId(), session});
    _cacheExpired = true;
}

void LwsSessions::remove(SessionId sessionId)
{
    const std::lock_guard<std::mutex> guard(_mutex);
    _sessions.erase(sessionId);
    _cacheExpired = true;
}

auto LwsSessions::get(SessionId sessionId) -> ILwsSessionPtr
{
    const std::lock_guard<std::mutex> guard(_mutex);
    auto it = _sessions.find(sessionId);
    if (it != _sessions.end())
    {
        return it->second;
    }
    return ILwsSessionPtr{};
}

auto LwsSessions::getAllSessions() -> std::vector<ILwsSessionPtr>
{
    if (_cacheExpired)
    {
        _cachedSessions.clear();
        _cachedSessions.reserve(_sessions.size());
        const std::lock_guard<std::mutex> guard(_mutex);
        if (_cacheExpired)
        {
            for(auto& entry : _sessions)
            {
                _cachedSessions.push_back(entry.second);
            }
            _cacheExpired = false;
        }
    }

    return _cachedSessions;
}

} // namespace ews::srv
