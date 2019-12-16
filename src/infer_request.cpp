#include <napi-thread-safe-callback.hpp>

#include "blob.h"
#include "infer_request.h"

Napi::Object InferenceEngineJS::InferRequest::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "InferRequest", {
            InstanceMethod("getBlob", &InferRequest::getBlob),
            InstanceMethod("infer", &InferRequest::infer),
            InstanceMethod("setCompletionCallback", &InferRequest::setCompletionCallback),
            InstanceMethod("startAsync", &InferRequest::startAsync),
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
        Napi::Error::New(info.Env(), "Set name of blob to InferenceEngineJS::InferRequest::getBlob")
                         .ThrowAsJavaScriptException();
        return env.Undefined();
    }
    auto blobName = info[0].As<Napi::String>();
    auto blobObj = InferenceEngineJS::Blob::constructor.New({
        Napi::External<InferenceEngine::InferRequest>::New(env,  this->_inferRequestPtr.get()),
        blobName
    });
    return blobObj;
}

void InferenceEngineJS::InferRequest::infer(const Napi::CallbackInfo &info) {
    this->_inferRequestPtr->Infer();
}

void InferenceEngineJS::InferRequest::setCompletionCallback(const Napi::CallbackInfo& info) {
    auto callback = std::make_shared<ThreadSafeCallback>(info[0].As<Napi::Function>());
    this->_inferRequestPtr->SetCompletionCallback([callback] {
                                                      try {
                                                          callback->call();
                                                      } catch (std::exception &e) {
                                                          std::cout<<"Error"<<std::endl;
                                                          callback->callError(e.what());
                                                      }
                                                  });
}

void InferenceEngineJS::InferRequest::startAsync(const Napi::CallbackInfo& info) {
    this->_inferRequestPtr->StartAsync();
}
