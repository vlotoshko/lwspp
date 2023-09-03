/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

namespace ews::cli
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
    v1_Amsterdam,

    // Reserved for future versions
//    v2_Barcelona,
//    v3_Chicago,
//    v4_Dublin,
//    v5_Eindhoven,

    Undefined,
};

} // namespace ews::cli
