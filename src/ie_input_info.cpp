#include "common.h"
#include "ie_input_info.h"


Napi::Object InferenceEngineJS::IEInputInfo::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IEInputInfo", {
            InstanceMethod("getDims", &IEInputInfo::getDims),
            InstanceMethod("setPrecision", &IEInputInfo::setPrecision),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IEInputInfo", func);
    return exports;
}

InferenceEngineJS::IEInputInfo::IEInputInfo(const Napi::CallbackInfo &info) : Napi::ObjectWrap<IEInputInfo>(info) {
    if (info[0].IsUndefined()) {
        throw Napi::Error::New(info.Env(),
                               "Set pointer to InputInfo to InferenceEngineJS::IEInputInfo constructor for initialize");
    }
    auto layerPtr = info[0].As<Napi::External<InferenceEngine::InputInfo>>().Data();
    this->_ieInputInfo = std::shared_ptr<InferenceEngine::InputInfo>(layerPtr);
}

Napi::FunctionReference InferenceEngineJS::IEInputInfo::constructor;

Napi::Value InferenceEngineJS::IEInputInfo::getDims(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto dims = this->_ieInputInfo->getInputData()->getDims();

    return vectorToNapiArray<std::size_t, Napi::Number>(env, dims);
}

void InferenceEngineJS::IEInputInfo::setPrecision(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto precision = InferenceEngine::Precision::FromStr(std::string(info[0].ToString()));
    this->_ieInputInfo->setPrecision(precision);
}
void InferenceEngineJS::IEInputInfo::setLayout(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto layout = layoutFromString(std::string(info[0].ToString()));
    this->_ieInputInfo->setLayout(layout);
}

