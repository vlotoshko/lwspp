/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

namespace ews::srv
{

/**
 * @brief Enumerates the versions of the lws_callback_function service callback.
 *
 * This enumeration is used for maintaining backward compatibility. When changes
 * to the callback behavior are introduced, a new version of the callback is released.
 * Users of the library can continue to use the previous callback to maintain the
 * same behavior as before.
 */
enum class CallbackVersion
{
    v1_Andromeda,

    // Reserved for future versions
//    v2_BlackHole,
//    v3_Chaos,
//    v4_DarkEnergy,
//    v5_Entropy,

    Undefined,
};

} // namespace ews::srv
