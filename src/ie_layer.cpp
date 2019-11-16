#include "ie_layer.h"


Napi::Object InferenceEngineJS::IELayer::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IELayer", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IELayer", func);
    return exports;
}

InferenceEngineJS::IELayer::IELayer(const Napi::CallbackInfo &info) : Napi::ObjectWrap<IELayer>(info) {
    this->_ieCNNLayer = std::shared_ptr<InferenceEngine::CNNLayer>(info[0].As<Napi::External<InferenceEngine::CNNLayer>>().Data());
}

Napi::FunctionReference InferenceEngineJS::IELayer::constructor;
