/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

namespace ews::srv
{

/**
 * @brief The ServerVersion enum is used for the backward compatibility.
 * The new server version is released when the existing core behavior of the server is changed.
 * The behavior extension, like adding a new feature, without changing the existing behavior
 * does not affect the new version release.
 */
enum class ServerVersion
{
    v1_Andromeda,
    // Reserved for future purposes
//    v2_BlackHole,
//    v3_Chaos,
//    v4_DarkEnergy,
//    v5_Entropy,
    Undefined,
};

} // namespace ews::srv
