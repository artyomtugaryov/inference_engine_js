#include "ie_network.h"

Napi::Object IENetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IENetwork", {
            InstanceAccessor("batch", &IENetwork::getBatchSize, nullptr),
            InstanceAccessor("precision", &IENetwork::getPrecision, nullptr)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IENetwork", func);

    return exports;
}

IENetwork::IENetwork(const Napi::CallbackInfo &info): Napi::ObjectWrap<IENetwork>(info) {

    auto model = std::string(info[0].ToString());
    auto weights = std::string(info[1].ToString());

    InferenceEngine::CNNNetReader net_reader;
    net_reader.ReadNetwork(model);
    net_reader.ReadWeights(weights);

    this->_ie_network = net_reader.getNetwork();
}

Napi::FunctionReference IENetwork::constructor;

Napi::Value IENetwork::getBatchSize(const Napi::CallbackInfo &info) {
    auto value = this->_ie_network.getBatchSize();
    return Napi::Number::New(info.Env(), value);
}

Napi::Value IENetwork::getPrecision(const Napi::CallbackInfo &info) {
    auto value = this->_ie_network.getPrecision().name();
    return Napi::String::New(info.Env(), value);
}
