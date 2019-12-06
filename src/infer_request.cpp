#include "blob.h"
#include "infer_request.h"

Napi::Object InferenceEngineJS::InferRequest::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "InferRequest", {
            InstanceMethod("getBlob", &InferRequest::getBlob),
            InstanceMethod("infer", &InferRequest::infer),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("InferRequest", func);
    return exports;
}

InferenceEngineJS::InferRequest::InferRequest(const Napi::CallbackInfo &info) : Napi::ObjectWrap<InferRequest>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(),
                         "Set pointer to InferRequest to InferenceEngineJS::InferRequest constructor for initialize")
                         .ThrowAsJavaScriptException();
        return;
    }
    auto executableNetworkPtr = info[0].As<Napi::External<InferenceEngine::ExecutableNetwork>>().Data();
    this->_inferRequestPtr = executableNetworkPtr->CreateInferRequestPtr();
}

Napi::FunctionReference InferenceEngineJS::InferRequest::constructor;

Napi::Value InferenceEngineJS::InferRequest::getBlob(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set name of blob to InferRequest::getBlob").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    auto blobName = std::string(info[0].As<Napi::String>());
    auto blobPtr = this->_inferRequestPtr->GetBlob(blobName);
    auto blobObj = InferenceEngineJS::Blob::constructor.New({
        Napi::External<InferenceEngine::Blob>::New(env, blobPtr.get())
    });
    return blobObj;
}

void InferenceEngineJS::InferRequest::infer(const Napi::CallbackInfo &info){
    this->_inferRequestPtr->Infer();
}
