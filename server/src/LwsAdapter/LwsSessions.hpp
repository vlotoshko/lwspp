/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <map>
#include <mutex>

#include "websocketpp/server/Types.hpp"
#include "LwsAdapter/ILwsSessions.hpp"

namespace wspp::srv
{

class LwsSessions : public ILwsSessions
{
public:
    void add(ILwsSessionPtr) override;
    void remove(SessionId) override;
    auto get(SessionId) -> ILwsSessionPtr override;
    auto getAllSessions() -> std::vector<ILwsSessionPtr> override;

private:
    std::map<SessionId, ILwsSessionPtr> _sessions;
    std::mutex _mutex;
    std::vector<ILwsSessionPtr> _cachedSessions;
    bool _cacheExpired = true;
};

} // namespace wspp::srv
