/*
 * lwspp - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <libwebsockets.h>
#include <stdexcept>

#include "lwspp/client/IClientControlAcceptor.hpp" // IWYU pragma: keep

#include "ClientContext.hpp"
#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsClient.hpp"
#include "LwsAdapter/LwsClientControl.hpp"
#include "LwsAdapter/LwsContextDeleter.hpp"
#include "LwsAdapter/LwsDataHolder.hpp"
#include "SslSettings.hpp" // IWYU pragma: keep

namespace lwspp
{
namespace cli
{
namespace
{

void setupSslSettings(lws_context_creation_info& lwsContextInfo, const SslSettingsPtr& ssl)
{
    if (ssl != nullptr)
    {
        const auto options = static_cast<uint64_t>(LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT);

        if (ssl->privateKeyPath != UNDEFINED_FILE_PATH)
        {
            lwsContextInfo.client_ssl_private_key_filepath = ssl->privateKeyPath.c_str();
        }

        if (ssl->certPath != UNDEFINED_FILE_PATH)
        {
            lwsContextInfo.client_ssl_cert_filepath = ssl->certPath.c_str();
        }

        if (ssl->caCertPath != UNDEFINED_FILE_PATH)
        {
            lwsContextInfo.client_ssl_ca_filepath = ssl->caCertPath.c_str();
        }

        if (!ssl->privateKeyPassword.empty())
        {
            lwsContextInfo.client_ssl_private_key_password= ssl->privateKeyPassword.c_str();
        }

        if (!ssl->ciphersList.empty())
        {
            lwsContextInfo.ssl_cipher_list = ssl->ciphersList.c_str();
        }

        if (!ssl->ciphersListTls13.empty())
        {
            lwsContextInfo.tls1_3_plus_cipher_list = ssl->ciphersListTls13.c_str();
        }

        lwsContextInfo.options = lwsContextInfo.options | options;
    }
}

auto setupSslConnectionFlags(const SslSettingsPtr& ssl) -> int
{
    int sslConnectionFlags = 0;

    if (ssl != nullptr)
    {
        sslConnectionFlags = sslConnectionFlags | LCCSCF_USE_SSL;

        if (ssl->allowSelfSignedServerCert)
        {
            sslConnectionFlags = sslConnectionFlags | LCCSCF_ALLOW_SELFSIGNED;
        }

        if (ssl->allowExpiredServerCert)
        {
            sslConnectionFlags = sslConnectionFlags | LCCSCF_ALLOW_EXPIRED;
        }

        if (ssl->skipServerCertHostnameCheck)
        {
            sslConnectionFlags = sslConnectionFlags | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
        }

        if (ssl->ignoreServerCaSert)
        {
            sslConnectionFlags = sslConnectionFlags | LCCSCF_ALLOW_INSECURE;
        }
    }
    return sslConnectionFlags;
}

} // namespace

LwsClient::LwsClient(const ClientContext& context)
    : _lwsConnectionInfo()
{
    auto clientControl = std::make_shared<LwsClientControl>();
    context.clientControlAcceptor->acceptClientControl(clientControl);

    _callbackContext = std::make_shared<LwsCallbackContext>(context.clientLogic, clientControl);
    _dataHolder = std::make_shared<LwsDataHolder>(context);

    setupLowLevelContext_();
    setupConnectionInfo_();
}

LwsClient::~LwsClient()
{
    disconnect();
    waitForClientStopping_();
}

void LwsClient::connect()
{
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        if (_state == State::Initial)
        {
            _wsInstance = lws_client_connect_via_info(&_lwsConnectionInfo);
           _state = State::Started;
        }
        else
        {
            return;
        }
    }

    int res = 0;
    while (res >= 0 && _state != State::Stopping)
    {
        res = lws_service(_lowLevelContext.get(), 0);
    }

    if (res < 0)
    {
        throw std::runtime_error{
            std::string{"lws_service stopped with the error code: "}.append(std::to_string(res))};
    }

    _state = State::Stopped;
    _isStoppedCV.notify_one();
}

void LwsClient::disconnect()
{
    const std::lock_guard<std::mutex> guard(_mutex);
    if (_state == State::Initial)
    {
        _state = State::Stopped;
    }
    else if (_state == State::Started)
    {
        _state = State::Stopping;
        _callbackContext->setStopping();
        lws_cancel_service(_lowLevelContext.get());
    }
}

void LwsClient::setupLowLevelContext_()
{
    auto lwsContextInfo = lws_context_creation_info{};
    lwsContextInfo.protocols = _dataHolder->protocols.data();
    lwsContextInfo.user = _callbackContext.get();
    lwsContextInfo.port = CONTEXT_PORT_NO_LISTEN;
    lwsContextInfo.ka_time = _dataHolder->keepAliveTimeout;
    lwsContextInfo.ka_interval = _dataHolder->keepAliveProbesInterval;
    lwsContextInfo.ka_probes = _dataHolder->keepAliveProbes;

    if (_dataHolder->lwsLogLevel != DEFAULT_LWS_LOG_LEVEL)
    {
        lws_set_log_level(_dataHolder->lwsLogLevel, nullptr);
    }

    setupSslSettings(lwsContextInfo, _dataHolder->ssl);

    _lowLevelContext = LowLevelContextPtr{lws_create_context(&lwsContextInfo), LwsContextDeleter{}};
    if (_lowLevelContext == nullptr)
    {
        throw std::runtime_error{"lws_context initialization failed"};
    }
}

void LwsClient::setupConnectionInfo_()
{
    _lwsConnectionInfo.context = _lowLevelContext.get();
    _lwsConnectionInfo.pwsi = &_wsInstance;

    _lwsConnectionInfo.address = _dataHolder->address.c_str();
    _lwsConnectionInfo.port = _dataHolder->port;
    _lwsConnectionInfo.path = _dataHolder->path.c_str();
    _lwsConnectionInfo.host = _lwsConnectionInfo.address;
    _lwsConnectionInfo.origin = _lwsConnectionInfo.address;
    _lwsConnectionInfo.ssl_connection = setupSslConnectionFlags(_dataHolder->ssl);

    if (_dataHolder->protocolName != DEFAULT_PROTOCOL_NAME)
    {
        _lwsConnectionInfo.protocol = _dataHolder->protocolName.c_str();
    }
}

void LwsClient::waitForClientStopping_()
{
    std::unique_lock<std::mutex> guard(_mutex);
    _isStoppedCV.wait(guard, [this]{ return _state == State::Stopped || _state == State::Initial; });
}

} // namespace cli
} // namespace lwspp
