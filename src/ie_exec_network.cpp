#include <ie_layer.h>
#include <inference_engine.hpp>

#include "ie_exec_network.h"

using namespace InferenceEngine;
using namespace Napi;

Napi::Object InferenceEngineJS::IEExecNetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IEExecNetwork", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IEExecNetwork", func);
    return exports;
}

InferenceEngineJS::IEExecNetwork::IEExecNetwork(const Napi::CallbackInfo &info) : Napi::ObjectWrap<IEExecNetwork>(info) {
    if (info[0].IsUndefined()) {
        throw Napi::Error::New(info.Env(),
                               "Set pointer to ExecNetworkPtr to InferenceEngineJS::IEEexecNetwork constructor for initialize");
    }
    auto execNetworkPtr = info[0].As<Napi::External<InferenceEngine::ExecutableNetwork>>().Data();
    this->_ieExecNetworkPtr = std::shared_ptr<InferenceEngine::ExecutableNetwork>(execNetworkPtr);
}

Napi::FunctionReference InferenceEngineJS::IEExecNetwork::constructor;
