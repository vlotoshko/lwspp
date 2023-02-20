/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <map>
#include <mutex>

#include "websocketpp/server/Types.hpp"
#include "ISessions.hpp"

namespace wspp::srv
{

class Sessions : public ISessions
{

public:
    void add(SessionId) override;
    void remove(SessionId) override;
    auto get(SessionId) -> ISessionPtr override;
    auto getAllSessions() -> std::vector<ISessionPtr> override;

private:
    std::map<SessionId, ISessionPtr> _sessions;
    std::mutex _mutex;
    std::vector<ISessionPtr> _cachedSessions;
    bool _cacheExpired = true;
};

} // namespace wspp::srv
