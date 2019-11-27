#include "infer_request.h"

Napi::Object InferenceEngineJS::InferRequest::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "InferRequest", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("InferRequest", func);
    return exports;
}

InferenceEngineJS::InferRequest::InferRequest(const Napi::CallbackInfo &info) : Napi::ObjectWrap<InferRequest>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set pointer to InferRequest to InferenceEngineJS::InferRequest constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto inferRequestPtr = info[0].As<Napi::External<InferenceEngine::InferRequest>>().Data();
    this->_inferRequestPtr = std::shared_ptr<InferenceEngine::InferRequest>(inferRequestPtr);
}

Napi::FunctionReference InferenceEngineJS::InferRequest::constructor;