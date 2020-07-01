#include <inference_engine.hpp>
#include <infer_request.h>

#include "executable_network.h"

using namespace InferenceEngine;
using namespace Napi;

Napi::Object InferenceEngineJS::ExecutableNetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "ExecutableNetwork", {
            InstanceMethod("createInferRequest", &ExecutableNetwork::createInferRequest),

    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("ExecutableNetwork", func);
    return exports;
}

void InferenceEngineJS::ExecutableNetwork::NewInstanceAsync(Napi::Env &env,
                                                            const std::shared_ptr<InferenceEngine::Core> &core,
                                                            const std::shared_ptr<InferenceEngine::CNNNetwork> &network,
                                                            const Napi::String &device,
                                                            Napi::Promise::Deferred &deferred) {
    auto load_network_worker = new InferenceEngineJS::LoadNetworkAsyncWorker(env, core, network, device, deferred);
    load_network_worker->Queue();
}

InferenceEngineJS::ExecutableNetwork::ExecutableNetwork(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ExecutableNetwork>(info) {}

Napi::FunctionReference InferenceEngineJS::ExecutableNetwork::constructor;

Napi::Value InferenceEngineJS::ExecutableNetwork::createInferRequest(const Napi::CallbackInfo &info){
    auto env = info.Env();
    auto inferRequest = InferenceEngineJS::InferRequest::constructor.New({
        Napi::External<InferenceEngine::ExecutableNetwork>::New(env, &(this->_ieExecNetwork))
    });
    return inferRequest;
}

void InferenceEngineJS::ExecutableNetwork::setExecutableNetwork(InferenceEngine::ExecutableNetwork &execNetwork) {
    this->_ieExecNetwork = execNetwork;
}

InferenceEngineJS::LoadNetworkAsyncWorker::LoadNetworkAsyncWorker(Napi::Env &env,
                                                                  const std::shared_ptr<InferenceEngine::Core> &corePtr,
                                                                  const std::shared_ptr<InferenceEngine::CNNNetwork> &cnnNetworkPtr,
                                                                  const Napi::String &device,
                                                                  Napi::Promise::Deferred &deferred)
        : Napi::AsyncWorker(env),
          _corePtr(corePtr),
          _cnnNetworkPtr(cnnNetworkPtr),
          _device(device.As<Napi::String>()),
          _env(env),
          _deferred(deferred) {}


void InferenceEngineJS::LoadNetworkAsyncWorker::Execute() {
    try {
        this->_ieExecutableNetwork = _corePtr->LoadNetwork(*(this->_cnnNetworkPtr), this->_device);
    } catch (const std::exception &error) {
        Napi::AsyncWorker::SetError(error.what());
    }
}

void InferenceEngineJS::LoadNetworkAsyncWorker::OnOK() {
    Napi::EscapableHandleScope scope(this->_env);
    auto executableNetworkObj = InferenceEngineJS::ExecutableNetwork::constructor.New({});
    auto executableNetwork = Napi::ObjectWrap<InferenceEngineJS::ExecutableNetwork>::Unwrap(executableNetworkObj);
    executableNetwork->setExecutableNetwork(this->_ieExecutableNetwork);
    this->_deferred.Resolve(scope.Escape(napi_value(executableNetworkObj)).ToObject());
}

void InferenceEngineJS::LoadNetworkAsyncWorker::OnError(Napi::Error const &error) {
    this->_deferred.Reject(error.Value());
}

