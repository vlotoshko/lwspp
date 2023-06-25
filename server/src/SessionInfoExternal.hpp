/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jun, 2023
 */

#pragma once

#include <future>

#include "easywebsocket/server/ISessionInfo.hpp"

namespace ews::srv
{

class SessionInfoExternal : public ISessionInfo
{
public:
    SessionInfoExternal(SessionId, Path);

public:
    auto getSessionId() -> SessionId override;
    auto getPath() -> const Path& override;

private:
    SessionId _sessionId;
    Path _path;
};

} // namespace ews::srv
