#include "ie_layer.h"


Napi::Object InferenceEngineJS::IELayer::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IELayer", {
            InstanceMethod("getParamAsString", &IELayer::getParamAsString),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IELayer", func);
    return exports;
}

InferenceEngineJS::IELayer::IELayer(const Napi::CallbackInfo &info) : Napi::ObjectWrap<IELayer>(info) {
    if (info[0].IsUndefined()) {
        throw Napi::Error::New(info.Env(),
                               "Set pointer to CNNLayer to InferenceEngineJS::IELayer constructor for initialize");
    }
    auto layerPtr = info[0].As<Napi::External<InferenceEngine::CNNLayer>>().Data();
    this->_ieCNNLayer = std::shared_ptr<InferenceEngine::CNNLayer>(layerPtr);
}

Napi::FunctionReference InferenceEngineJS::IELayer::constructor;

Napi::Value InferenceEngineJS::IELayer::getParamAsString(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto paramName = std::string(info[0].ToString());
    std::string value;
    if (!info[1].IsUndefined()) {
        auto defaultValue = std::string(info[1].ToString());
        value = this->_ieCNNLayer->GetParamAsString(paramName.c_str(), defaultValue.c_str());
    } else {
        value =  this->_ieCNNLayer->GetParamAsString(paramName.c_str());
    }
    return Napi::String::New(env, value);
}