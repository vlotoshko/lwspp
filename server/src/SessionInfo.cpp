/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jun, 2023
 */

#include "SessionInfo.hpp"

namespace ews::srv
{

SessionInfo::SessionInfo(SessionId sessionId, Path path)
    : _sessionId(sessionId)
    , _path(std::move(path))
{}

auto SessionInfo::getSessionId() -> SessionId
{
    return _sessionId;
}

auto SessionInfo::getPath() -> const Path&
{
    return _path;
}



} // namespace ews::srv
