/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

//#include "LwsAdapter/LwsSessions.hpp"
#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsSessions.hpp"

namespace wspp::srv
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

} // namespace wspp::srv
