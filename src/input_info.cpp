#include "common.h"
#include "input_info.h"


Napi::Object InferenceEngineJS::InputInfo::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "InputInfo", {
            InstanceMethod("getDims", &InputInfo::getDims),
            InstanceMethod("setPrecision", &InputInfo::setPrecision),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("InputInfo", func);
    return exports;
}

InferenceEngineJS::InputInfo::InputInfo(const Napi::CallbackInfo &info) : Napi::ObjectWrap<InputInfo>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set pointer to InputInfo to InferenceEngineJS::InputInfo constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto layerPtr = info[0].As<Napi::External<InferenceEngine::InputInfo>>().Data();
    this->_ieInputInfo = std::shared_ptr<InferenceEngine::InputInfo>(layerPtr);
}

Napi::FunctionReference InferenceEngineJS::InputInfo::constructor;

Napi::Value InferenceEngineJS::InputInfo::getDims(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto dims = this->_ieInputInfo->getInputData()->getDims();

    return vectorToNapiArray<std::size_t, Napi::Number>(env, dims);
}

void InferenceEngineJS::InputInfo::setPrecision(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto precision = InferenceEngine::Precision::FromStr(std::string(info[0].ToString()));
    this->_ieInputInfo->setPrecision(precision);
}
void InferenceEngineJS::InputInfo::setLayout(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto layout = layoutFromString(std::string(info[0].ToString()));
    this->_ieInputInfo->setLayout(layout);
}

