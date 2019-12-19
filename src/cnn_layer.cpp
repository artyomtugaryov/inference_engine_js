#include "cnn_layer.h"


Napi::Object InferenceEngineJS::CNNLayer::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "CNNLayer", {
            InstanceMethod("getParamAsString", &CNNLayer::getParamAsString),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("CNNLayer", func);
    return exports;
}

InferenceEngineJS::CNNLayer::CNNLayer(const Napi::CallbackInfo &info) : Napi::ObjectWrap<CNNLayer>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set pointer to CNNNetwork to InferenceEngineJS::CNNLayer constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    if (info[1].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set name of layer to InferenceEngineJS::CNNLayer constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto cnnNetwork = info[0].As<Napi::External<InferenceEngine::CNNNetwork>>().Data();
    auto layerName = std::string(info[1].As<Napi::String>());

    this->_ieCNNLayer = cnnNetwork->getLayerByName(layerName.c_str());
}

Napi::FunctionReference InferenceEngineJS::CNNLayer::constructor;

Napi::Value InferenceEngineJS::CNNLayer::getParamAsString(const Napi::CallbackInfo &info) {
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