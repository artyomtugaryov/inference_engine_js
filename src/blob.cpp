#include <iomanip>
#include "blob.h"
#include "common.h"

Napi::Object InferenceEngineJS::Blob::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Blob", {
            InstanceMethod("getDims", &Blob::getDims),
            InstanceMethod("fillImageAsU8", &Blob::fillImageAsU8),
            InstanceMethod("getTopClassificationResults", &Blob::getTopClassificationResults),
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

void InferenceEngineJS::Blob::fillImageAsU8(const Napi::CallbackInfo &info) {
    auto inputData = info[0].As<Napi::Array>();
    using myBlobType = InferenceEngine::PrecisionTrait<InferenceEngine::Precision::U8>::value_type *;
    auto blobData = this->_ieBlobPtr->buffer().as<myBlobType>();
    for (unsigned int i = 0; i < inputData.Length(); i++) {
        blobData[i] = static_cast<unsigned int>((inputData.Get(i).As<Napi::Number>()));
    }
}

Napi::Value InferenceEngineJS::Blob::getTopClassificationResults(const Napi::CallbackInfo &info) {
    auto numResults = static_cast<int>(info[0].As<Napi::Number>());
    using myBlobType = InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type;
    std::vector<unsigned> results;
    InferenceEngine::TBlob<myBlobType>& tblob = dynamic_cast<InferenceEngine::TBlob<myBlobType>&>(*(this->_ieBlobPtr));
    InferenceEngine::TopResults(numResults, tblob, results);
    auto image_id=0;
    for (size_t id = image_id * numResults, cnt = 0; id < (image_id + 1) * numResults; ++cnt, ++id) {
        std::cout.precision(7);
        /** Getting probability for resulting class **/
        const auto result = this->_ieBlobPtr->buffer().as<myBlobType*>()[results[id] + image_id * (this->_ieBlobPtr->size() / 1)];

        std::cout << std::setw(static_cast<int>(5)) << std::left << results[id] << " ";
        std::cout << std::left << std::setw(static_cast<int>(5)) << std::fixed << result;
        std::cout << std::endl;
    }
}
