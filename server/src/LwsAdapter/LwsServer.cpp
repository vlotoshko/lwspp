/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <libwebsockets.h>
#include <thread>

#include "easywebsocket/server/IMessageSenderAcceptor.hpp"

#include "LwsAdapter/ILwsCallbackNotifier.hpp"
#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsContextDeleter.hpp"
#include "LwsAdapter/LwsDataHolder.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"
#include "LwsAdapter/LwsServer.hpp"
#include "LwsAdapter/LwsSessions.hpp"
#include "ServerContext.hpp"
#include "SslSettings.hpp"

namespace ews::srv
{

namespace
{

class LwsCallbackNotifier : public ILwsCallbackNotifier
{
public:
    LwsCallbackNotifier(const LwsDataHolderPtr& d, const LowLevelContextPtr& c)
        : _dataHolder(d)
        , _lowLevelContext(c)
    {}

    void sendSessionMessages(const ILwsSessionPtr& session) override
    {
        lws_callback_on_writable(session->getLwsInstance());
    }

    void sendPendingMessages() override
    {
        if (auto context = _lowLevelContext.lock())
        {
            if (auto dataHolder = _dataHolder.lock())
            {
                lws_callback_on_writable_all_protocol(
                    context.get(), dataHolder->protocols.data());
            }
        }
    }

private:
    LwsDataHolderWeak _dataHolder;
    LowLevelContextWeak _lowLevelContext;
};

void setupSslSettings(lws_context_creation_info& lwsContextInfo, const SslSettingsPtr& ssl)
{
    if (ssl != nullptr)
    {
        uint64_t options = 0;

        if (ssl->privateKeyPath != UNDEFINED_FILE_PATH)
        {
            options = options | LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
            lwsContextInfo.ssl_private_key_filepath = ssl->privateKeyPath.c_str();
        }

        if (ssl->certPath != UNDEFINED_FILE_PATH)
        {
            lwsContextInfo.ssl_cert_filepath = ssl->certPath.c_str();
        }

        if (ssl->caCertPath != UNDEFINED_FILE_PATH)
        {
            options = options | LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
            lwsContextInfo.ssl_ca_filepath = ssl->caCertPath.c_str();
        }

        if (!ssl->privateKeyPassword.empty())
        {
            lwsContextInfo.ssl_private_key_password= ssl->privateKeyPassword.c_str();
        }

        if (!ssl->ciphersList.empty())
        {
            lwsContextInfo.ssl_cipher_list = ssl->ciphersList.c_str();
        }

        if (!ssl->ciphersListTls13.empty())
        {
            lwsContextInfo.tls1_3_plus_cipher_list = ssl->ciphersListTls13.c_str();
        }

        if (ssl->requireValidClientCert)
        {
            options = options | LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT;
        }
        lwsContextInfo.options = lwsContextInfo.options | options;
    }
}

auto setupLowLeverContext(const ILwsCallbackContextPtr& callbackContext, const LwsDataHolderPtr& dataHolder)
    -> LowLevelContextPtr
{
    auto lwsContextInfo = lws_context_creation_info{};

    lwsContextInfo.user = callbackContext.get();
    lwsContextInfo.port = dataHolder->port;
    lwsContextInfo.protocols = dataHolder->protocols.data();

    setupSslSettings(lwsContextInfo, dataHolder->ssl);

    auto lowLevelContext = LowLevelContextPtr{lws_create_context(&lwsContextInfo), LwsContextDeleter{}};
    if (lowLevelContext == nullptr)
    {
        throw std::runtime_error{"lws_context initialization failed"};
    }
    return lowLevelContext;
}

} // namespace

LwsServer::LwsServer(const ServerContext& context)
{
    auto sessions = std::make_shared<LwsSessions>();
    _callbackContext = std::make_shared<LwsCallbackContext>(context.eventHandler, sessions);
    _dataHolder = std::make_shared<LwsDataHolder>(context);
    _lowLevelContext = setupLowLeverContext(_callbackContext, _dataHolder);

    auto notifier = std::make_shared<LwsCallbackNotifier>(_dataHolder, _lowLevelContext);
    auto sender = std::make_shared<LwsMessageSender>(sessions, std::move(notifier));
    context.messageSenderAcceptor->acceptMessageSender(std::move(sender));
}

LwsServer::~LwsServer()
{
    stopListening();
    waitForServerStopped_();
}

void LwsServer::startListening()
{
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        if (_state == State::Initial)
        {
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

void LwsServer::stopListening()
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
    }
}

void LwsServer::waitForServerStopped_()
{
    std::unique_lock<std::mutex> guard(_mutex);
    _isStoppedCV.wait(guard, [this]{ return _state == State::Stopped || _state == State::Initial; });
}

} // namespace ews::srv
