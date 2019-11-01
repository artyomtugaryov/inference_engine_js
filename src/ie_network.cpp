#include "ie_network.h"

Napi::Object IENetwork::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IENetwork", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IENetwork", func);

    return exports;
}
IENetwork::IENetwork(const Napi::CallbackInfo &info): Napi::ObjectWrap<IENetwork>(info) {

    std::string model = std::string(info[0].ToString());
    std::string weights = std::string(info[1].ToString());

    InferenceEngine::CNNNetReader net_reader;
    net_reader.ReadNetwork(model);
    net_reader.ReadWeights(weights);

    this->network = net_reader.getNetwork();
}

Napi::FunctionReference IENetwork::constructor;