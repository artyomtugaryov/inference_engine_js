#include <iomanip>
#include "blob.h"
#include "common.h"

Napi::Object InferenceEngineJS::Blob::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Blob", {
            InstanceMethod("fillWithU8", &Blob::fillWithU8),
            InstanceMethod("getDims", &Blob::getDims),
            InstanceMethod("getClassificationResult", &Blob::getClassificationResult),
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

InferenceEngineJS::Blob::~Blob(){
    std::cout<<"DESTRUCTOR"<<std::endl;
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

Napi::Value InferenceEngineJS::Blob::getClassificationResult(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto dims = this->_ieBlobPtr->getTensorDesc().getDims();

    Napi::Array output = Napi::Array::New(env, dims[0]);

    size_t rank = dims.size();
    if (!rank || !dims[0]) {
        Napi::Error::New(env, "Cannot parse argument").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    size_t batchSize = dims[0];
    std::vector<unsigned> indexes(this->_ieBlobPtr->size() / batchSize);
    using blobType = InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type;
    auto blob = dynamic_cast<InferenceEngine::TBlob<blobType> &>(*(this->_ieBlobPtr));

    for (size_t i = 0; i < batchSize; i++) {

        auto bachArray = Napi::Array::New(env, dims[1]);

        size_t offset = i * (this->_ieBlobPtr->size() / batchSize);
        blobType *batchData = blob.data();
        batchData += offset;

        for (std::size_t j = 0; j < dims[1]; j++) {
            auto value = batchData[j];
            bachArray[j] = Napi::Number::New(env, value);
        }

        output[i] = bachArray;
    }
    return output;
}