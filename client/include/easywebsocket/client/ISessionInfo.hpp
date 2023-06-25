/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jun, 2023
 */

#pragma once

namespace ews::cli
{

class ISessionInfo
{
public:
    ISessionInfo() = default;
    virtual ~ISessionInfo() = default;

    ISessionInfo(ISessionInfo&&) = default;
    auto operator=(ISessionInfo&&) noexcept -> ISessionInfo& = default;

    ISessionInfo(const ISessionInfo&) = delete;
    auto operator=(const ISessionInfo&) noexcept -> ISessionInfo& = delete;
};

} // namespace ews::cli
