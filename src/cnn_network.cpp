#include <inference_engine.hpp>

#include "cnn_network.h"
#include "input_info.h"
#include "data.h"

Napi::Object InferenceEngineJS::CNNNetwork::Init(Napi::Env env, Napi::Object exports) {
    const auto classUTF8Name = "CNNNetwork";
    auto func = DefineClass(env, classUTF8Name, {
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
    exports.Set(classUTF8Name, func);
    return exports;
}

void InferenceEngineJS::CNNNetwork::NewInstanceAsync(Napi::Env env,
                                                     const Napi::Value &model_path,
                                                     const Napi::Value &weights_path,
                                                     const InferenceEngine::Core* core,
                                                     Napi::Promise::Deferred &deferred) {
    auto read_network_worker = new InferenceEngineJS::ReadNetworkAsyncWorker(env, model_path, weights_path, core, deferred);
    read_network_worker->Queue();
}

Napi::FunctionReference InferenceEngineJS::CNNNetwork::constructor;

InferenceEngineJS::CNNNetwork::CNNNetwork(const Napi::CallbackInfo &info) : Napi::ObjectWrap<CNNNetwork>(info) {}

void InferenceEngineJS::CNNNetwork::setCNNNetwork(InferenceEngine::CNNNetwork &cnnNetwork) {
    this->_ieNetwork = cnnNetwork;
}

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

InferenceEngineJS::ReadNetworkAsyncWorker::ReadNetworkAsyncWorker(Napi::Env &env,
                                                                  const Napi::Value &model,
                                                                  const Napi::Value &weights,
                                                                  const InferenceEngine::Core* core,
                                                                  Napi::Promise::Deferred &deferred)
        : Napi::AsyncWorker(env),
          _model_path(model.As<Napi::String>()),
          _weights_path(weights.As<Napi::String>()),
          _core(core),
          _env(env),
          _deferred(deferred) {}


void InferenceEngineJS::ReadNetworkAsyncWorker::Execute() {
    try {
        this->_ieNetwork = _core->ReadNetwork(this->_model_path, this->_weights_path);
    } catch (const std::exception &error) {
        Napi::AsyncWorker::SetError(error.what());
    }
}

void InferenceEngineJS::ReadNetworkAsyncWorker::OnOK() {
    Napi::EscapableHandleScope scope(this->_env);
    auto cnnNetworkObj = InferenceEngineJS::CNNNetwork::constructor.New({});
    auto cnnNetwork = Napi::ObjectWrap<InferenceEngineJS::CNNNetwork>::Unwrap(cnnNetworkObj);
    cnnNetwork->setCNNNetwork(this->_ieNetwork);
    this->_deferred.Resolve(scope.Escape(napi_value(cnnNetworkObj)).ToObject());
}

void InferenceEngineJS::ReadNetworkAsyncWorker::OnError(Napi::Error const &error) {
    this->_deferred.Reject(error.Value());
}
