#include <inference_engine.hpp>
#include <cnn_layer.h>

#include "cnn_network.h"
#include "input_info.h"
#include "data.h"

Napi::Object InferenceEngineJS::CNNNetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "CNNNetwork", {
            InstanceMethod("setBatchSize", &CNNNetwork::setBatchSize),
            InstanceMethod("getBatchSize", &CNNNetwork::getBatchSize),
            InstanceMethod("getName", &CNNNetwork::getName),
            InstanceMethod("layerCount", &CNNNetwork::layerCount),
            InstanceMethod("size", &CNNNetwork::layerCount),
            InstanceMethod("getInputsInfo", &CNNNetwork::getInputsInfo),
            InstanceMethod("getOutputsInfo", &CNNNetwork::getOutputsInfo),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("CNNNetwork", func);
    return exports;
}

InferenceEngineJS::CNNNetwork::CNNNetwork(const Napi::CallbackInfo &info) : Napi::ObjectWrap<CNNNetwork>(info) {

    auto model = std::string(info[0].ToString());
    auto weights = std::string(info[1].ToString());

    auto corePtr = info[2].As<Napi::External<InferenceEngine::Core>>().Data();

    this->_ieNetwork = corePtr->ReadNetwork(model, weights);
}

Napi::FunctionReference InferenceEngineJS::CNNNetwork::constructor;

void InferenceEngineJS::CNNNetwork::setBatchSize(const Napi::CallbackInfo &info) {
    auto batchSize = info[0].ToNumber().Int32Value();
    this->_ieNetwork.setBatchSize(batchSize);
}

Napi::Value InferenceEngineJS::CNNNetwork::getBatchSize(const Napi::CallbackInfo &info) {
    auto value = this->_ieNetwork.getBatchSize();
    return Napi::Number::New(info.Env(), value);
}

Napi::Value InferenceEngineJS::CNNNetwork::getName(const Napi::CallbackInfo &info) {
    return Napi::String::New(info.Env(), this->_ieNetwork.getName());
}

Napi::Value InferenceEngineJS::CNNNetwork::layerCount(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), this->_ieNetwork.layerCount());
}

Napi::Value InferenceEngineJS::CNNNetwork::getInputsInfo(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto inputsDataMap = this->_ieNetwork.getInputsInfo();

    Napi::Array result = Napi::Array::New(env, inputsDataMap.size());
    std::size_t i = 0;

    auto extIENetwork = Napi::External<InferenceEngine::CNNNetwork>::New(env, &(this->_ieNetwork));

    for (const auto &inputInfo: inputsDataMap) {

        auto ieInputInfo = InputInfo::constructor.New({extIENetwork, Napi::String::New(env, inputInfo.first)});

        auto obj = Napi::Object::New(env);
        obj.Set("name", inputInfo.first);
        obj.Set("value", ieInputInfo);

        result[i++] = obj;
    }

    return result;
}

Napi::Value InferenceEngineJS::CNNNetwork::getOutputsInfo(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto outputsDataMap = this->_ieNetwork.getOutputsInfo();
    Napi::Array result = Napi::Array::New(env, outputsDataMap.size());
    std::size_t i = 0;
    auto extIENetwork = Napi::External<InferenceEngine::CNNNetwork>::New(env, &(this->_ieNetwork));
    for (const auto &outputInfo: outputsDataMap) {

        auto ieDataPtr = Data::constructor.New({extIENetwork, Napi::String::New(env, outputInfo.first)});

        auto obj = Napi::Object::New(env);
        obj.Set("name", outputInfo.first);
        obj.Set("value", ieDataPtr);

        result[i++] = obj;
    }

    return result;
}

InferenceEngine::CNNNetwork *InferenceEngineJS::CNNNetwork::getCNNNetworkPtr() {
    return &(this->_ieNetwork);
}