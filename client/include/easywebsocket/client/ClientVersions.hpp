/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

namespace ews::cli
{

/**
 * @brief The ClientVersion enum is used for the backward compatibility.
 * The new client version is released when the existing core behavior of the client is changed.
 * The behavior extension, like adding a new feature, without changing the existing behavior
 * does not affect the new version release.
 */
enum class ClientVersion
{
    v1_Amsterdam,

    // Reserved for future purposes
//    v2_Barcelona,
//    v3_Chicago,
//    v4_Dublin,
//    v5_Eindhoven,
    Undefined,
};

} // namespace ews::cli
