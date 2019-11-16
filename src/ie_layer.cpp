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
}

Napi::FunctionReference InferenceEngineJS::IELayer::constructor;

InferenceEngineJS::IELayer InferenceEngineJS::IELayer::New(const Napi::CallbackInfo &info,
                                                           const InferenceEngine::CNNLayerPtr &layerPtr) {
    IELayer layer(info);
    layer._ieCNNLayer = layerPtr;
    return layer;
}