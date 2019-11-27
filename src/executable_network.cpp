#include <cnn_layer.h>
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

InferenceEngineJS::ExecutableNetwork::ExecutableNetwork(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ExecutableNetwork>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set pointer to CNNNetwork to InferenceEngineJS::ExecutableNetwork constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    if (info[1].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set pointer to Core to InferenceEngineJS::ExecutableNetwork constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    if (info[1].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set device to InferenceEngineJS::ExecutableNetwork constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto cnnNetworkPtr = info[0].As<Napi::External<InferenceEngine::CNNNetwork>>().Data();
    auto corePtr = info[1].As<Napi::External<InferenceEngine::Core>>().Data();
    auto device = std::string(info[2].As<Napi::String>());
    this->_ieExecNetwork = corePtr->LoadNetwork(*cnnNetworkPtr, device);
}

Napi::FunctionReference InferenceEngineJS::ExecutableNetwork::constructor;

Napi::Value InferenceEngineJS::ExecutableNetwork::createInferRequest(const Napi::CallbackInfo &info){
    auto env = info.Env();
    auto inferRequestPtr = this->_ieExecNetwork.CreateInferRequestPtr();
    auto inferRequestObj = InferenceEngineJS::InferRequest::constructor.New({
        Napi::External<InferenceEngine::InferRequest>::New(env, inferRequestPtr.get())
    });
    return inferRequestObj;
}
