#include <ie_layer.h>
#include <inference_engine.hpp>

#include "ie_network.h"
#include "ie_input_info.h"

using namespace InferenceEngine;
using namespace Napi;

Napi::Object InferenceEngineJS::IENetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IENetwork", {
            InstanceMethod("setBatchSize", &IENetwork::setBatchSize),
            InstanceMethod("getBatchSize", &IENetwork::getBatchSize),
            InstanceMethod("getPrecision", &IENetwork::getPrecision),
            InstanceMethod("getName", &IENetwork::getName),
            InstanceMethod("layerCount", &IENetwork::layerCount),
            InstanceMethod("size", &IENetwork::layerCount),
            InstanceMethod("getLayerByName", &IENetwork::getLayerByName),
            InstanceMethod("getInputsInfo", &IENetwork::getInputsInfo),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IENetwork", func);
    return exports;
}

InferenceEngineJS::IENetwork::IENetwork(const Napi::CallbackInfo &info) : Napi::ObjectWrap<IENetwork>(info) {

    auto model = std::string(info[0].ToString());
    auto weights = std::string(info[1].ToString());

    CNNNetReader net_reader;
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
    auto ieLayer = IELayer::constructor.New({Napi::External<CNNLayer>::New(info.Env(), layerPtr.get())});
    return ieLayer;
}

Napi::Value InferenceEngineJS::IENetwork::getInputsInfo(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto inputsDataMap = this->_ieNetwork.getInputsInfo();
    Napi::Array result = Napi::Array::New(env, inputsDataMap.size());
    std::size_t i = 0;
    for (const auto& inputInfo: inputsDataMap) {
        auto inputInfoPtr = inputInfo.second;
        auto ieInputInfo = IEInputInfo::constructor.New({Napi::External<InputInfo>::New(env, inputInfoPtr.get())});
        auto obj = Napi::Object::New(env);
        obj.Set(inputInfo.first, ieInputInfo);
        result[i++] = obj;
    }

    return result;
}

InferenceEngine::CNNNetwork InferenceEngineJS::IENetwork::getCNNNetwork(){
    return this->_ieNetwork;
}