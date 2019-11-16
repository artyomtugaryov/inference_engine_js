#include <ie_layer.h>
#include "ie_network.h"

Napi::Object InferenceEngineJS::IENetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IENetwork", {
            InstanceMethod("setBatchSize", &IENetwork::getBatchSize),
            InstanceMethod("getBatchSize", &IENetwork::getBatchSize),
            InstanceMethod("getPrecision", &IENetwork::getPrecision),
            InstanceMethod("getName", &IENetwork::getName),
            InstanceMethod("layerCount", &IENetwork::layerCount),
            InstanceMethod("size", &IENetwork::layerCount),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IENetwork", func);
    return exports;
}

InferenceEngineJS::IENetwork::IENetwork(const Napi::CallbackInfo &info): Napi::ObjectWrap<IENetwork>(info) {

    auto model = std::string(info[0].ToString());
    auto weights = std::string(info[1].ToString());

    InferenceEngine::CNNNetReader net_reader;
    net_reader.ReadNetwork(model);
    net_reader.ReadWeights(weights);

    this->_ieNetwork = net_reader.getNetwork();
}

Napi::FunctionReference InferenceEngineJS::IENetwork::constructor;

void InferenceEngineJS::IENetwork::setBatchSize(const Napi::CallbackInfo &info) {
    auto batchSize = info[0].ToNumber().Int32Value();
    this->_ieNetwork.setBatchSize(batchSize);
}

Napi::Value InferenceEngineJS::IENetwork::getBatchSize(const Napi::CallbackInfo &info) {
    auto value = this->_ieNetwork.getBatchSize();
    return Napi::Number::New(info.Env(), value);
}

Napi::Value InferenceEngineJS::IENetwork::getPrecision(const Napi::CallbackInfo &info) {
    auto value = this->_ieNetwork.getPrecision().name();
    return Napi::String::New(info.Env(), value);
}

Napi::Value InferenceEngineJS::IENetwork::getName(const Napi::CallbackInfo &info) {
    return Napi::String::New(info.Env(), this->_ieNetwork.getName());
}

Napi::Value InferenceEngineJS::IENetwork::layerCount(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), this->_ieNetwork.layerCount());
}

Napi::Value InferenceEngineJS::IENetwork::getLayerByName(const Napi::CallbackInfo &info) {
    auto layerName = std::string(info[0].ToString());
    auto layerPtr = this->_ieNetwork.getLayerByName(layerName.c_str());
    auto ieLayer  = InferenceEngineJS::IELayer::New(info, layerPtr);
    return ieLayer.Value();
}