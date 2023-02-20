/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "Sessions.hpp"
#include "Session.hpp"

namespace wspp::srv
{

void Sessions::add(SessionId sessionId)
{
    const std::lock_guard<std::mutex> guard(_mutex);
    _sessions.insert({sessionId, std::make_shared<Session>()});
    _cacheExpired = true;
}

void Sessions::remove(SessionId sessionId)
{
    const std::lock_guard<std::mutex> guard(_mutex);
    _sessions.erase(sessionId);
    _cacheExpired = true;
}

auto Sessions::get(SessionId sessionId) -> ISessionPtr
{
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        auto it = _sessions.find(sessionId);
        if (it != _sessions.end())
        {
            return it->second;
        }
    }
    return ISessionPtr{};
}

auto Sessions::getAllSessions() -> std::vector<ISessionPtr>
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

} // namespace wspp::srv
