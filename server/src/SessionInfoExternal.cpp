/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jun, 2023
 */

#include "SessionInfoExternal.hpp"

namespace ews::srv
{

SessionInfoExternal::SessionInfoExternal(SessionId sessionId, Path path)
    : _sessionId(sessionId)
    , _path(std::move(path))
{}

auto SessionInfoExternal::getSessionId() -> SessionId
{
    return _sessionId;
}

auto SessionInfoExternal::getPath() -> const Path&
{
    return _path;
}



} // namespace ews::srv
