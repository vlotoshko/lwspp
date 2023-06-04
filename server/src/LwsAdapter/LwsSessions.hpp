/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <map>
#include <mutex>

#include "easywebsocket/server/Types.hpp"
#include "LwsAdapter/ILwsSessions.hpp"

namespace ews::srv
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

} // namespace ews::srv
