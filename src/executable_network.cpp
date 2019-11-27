#include <cnn_layer.h>
#include <inference_engine.hpp>

#include "executable_network.h"

using namespace InferenceEngine;
using namespace Napi;

Napi::Object InferenceEngineJS::ExecutableNetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "ExecutableNetwork", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("ExecutableNetwork", func);
    return exports;
}

InferenceEngineJS::ExecutableNetwork::ExecutableNetwork(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ExecutableNetwork>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set pointer to ExecNetworkPtr to InferenceEngineJS::IEEexecNetwork constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto execNetworkPtr = info[0].As<Napi::External<InferenceEngine::ExecutableNetwork>>().Data();
    this->_ieExecNetworkPtr = std::shared_ptr<InferenceEngine::ExecutableNetwork>(execNetworkPtr);
}

Napi::FunctionReference InferenceEngineJS::ExecutableNetwork::constructor;
