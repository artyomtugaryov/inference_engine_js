#include "common.h"
#include "data.h"


Napi::Object InferenceEngineJS::Data::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Data", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Data", func);
    return exports;
}

InferenceEngineJS::Data::Data(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Data>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set pointer to Data to InferenceEngineJS::Data constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto dataPtr = info[0].As<Napi::External<InferenceEngine::Data>>().Data();
    this->_ieDataPtr = std::shared_ptr<InferenceEngine::Data>(dataPtr);
}

Napi::FunctionReference InferenceEngineJS::Data::constructor;
