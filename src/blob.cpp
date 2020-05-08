#include <iomanip>
#include "blob.h"
#include "common.h"

Napi::Object InferenceEngineJS::Blob::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Blob", {
            InstanceMethod("fillWithU8", &Blob::fillWithU8),
            InstanceMethod("getDims", &Blob::getDims),
            InstanceMethod("data", &Blob::data),
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
    auto inferRequestPtr = info[0].As<Napi::External<InferenceEngine::InferRequest>>().Data();
    auto blobName = std::string(info[1].As<Napi::String>());
    this->_ieBlobPtr = inferRequestPtr->GetBlob(blobName);
}

Napi::FunctionReference InferenceEngineJS::Blob::constructor;

Napi::Value InferenceEngineJS::Blob::getDims(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto dims = this->_ieBlobPtr->getTensorDesc().getDims();
    return vectorToNapiArray<std::size_t, Napi::Number>(env, dims);
}

void InferenceEngineJS::Blob::fillWithU8(const Napi::CallbackInfo &info) {
    auto inputData = info[0].As<Napi::Array>();
    using myBlobType = InferenceEngine::PrecisionTrait<InferenceEngine::Precision::U8>::value_type *;
    auto blobData = this->_ieBlobPtr->buffer().as<myBlobType>();
    for (unsigned int i = 0; i < inputData.Length(); i++) {
        blobData[i] = static_cast<unsigned int>((inputData.Get(i).As<Napi::Number>()));
    }
}

Napi::Value InferenceEngineJS::Blob::data(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto dims = this->_ieBlobPtr->getTensorDesc().getDims();
    using blobType = InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type;
    auto blob = dynamic_cast<InferenceEngine::TBlob<blobType> &>(*(this->_ieBlobPtr)).data();

    Napi::Array output = Napi::Array::New(env, dims[0]);

    for (size_t i=0; i< this->_ieBlobPtr->size(); i++){
        output[i] = Napi::Number::New(env, blob[i]);
    }
    return output;
}
