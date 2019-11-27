#include "blob.h"
#include "common.h"

Napi::Object InferenceEngineJS::Blob::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Blob", {
            InstanceMethod("getDims", &Blob::getDims),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Blob", func);
    return exports;
}

InferenceEngineJS::Blob::Blob(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Blob>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set pointer to Blob to InferenceEngineJS::Blob constructor for initialize");
    }
    auto blobPtr = info[0].As<Napi::External<InferenceEngine::Blob>>().Data();
    this->_ieBlobPtr = std::shared_ptr<InferenceEngine::Blob>(blobPtr);
}

Napi::FunctionReference InferenceEngineJS::Blob::constructor;

Napi::Value InferenceEngineJS::Blob::getDims(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto dims = this->_ieBlobPtr->getTensorDesc().getDims();
    return vectorToNapiArray<std::size_t, Napi::Number>(env, dims);
}