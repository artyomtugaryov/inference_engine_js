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

    this->_ie_network = net_reader.getNetwork();
}

Napi::FunctionReference InferenceEngineJS::IENetwork::constructor;

void InferenceEngineJS::IENetwork::setBatchSize(const Napi::CallbackInfo &info) {
    auto batchSize = info[0].ToNumber();
    this->_ie_network->setBatchSize(batchSize);
}

Napi::Value InferenceEngineJS::IENetwork::getBatchSize(const Napi::CallbackInfo &info) {
    auto value = this->_ie_network->getBatchSize();
    return Napi::Number::New(info.Env(), value);
}

Napi::Value InferenceEngineJS::IENetwork::getPrecision(const Napi::CallbackInfo &info) {
    auto value = this->_ie_network->getPrecision().name();
    return Napi::String::New(info.Env(), value);
}

Napi::Value InferenceEngineJS::IENetwork::getName(const Napi::CallbackInfo &info) {
    return Napi::String::New(info.Env(), this->_ie_network->getName());
}

Napi::Value InferenceEngineJS::IENetwork::layerCount(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), this->_ie_network->layerCount());
}