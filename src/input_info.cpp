#include "common.h"
#include "input_info.h"


Napi::Object InferenceEngineJS::InputInfo::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "InputInfo", {
            InstanceMethod("getDims", &InputInfo::getDims),
            InstanceMethod("setPrecision", &InputInfo::setPrecision),
            InstanceMethod("setLayout", &InputInfo::setLayout),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("InputInfo", func);
    return exports;
}

InferenceEngineJS::InputInfo::InputInfo(const Napi::CallbackInfo &info) : Napi::ObjectWrap<InputInfo>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set pointer to CNNNetwork to InferenceEngineJS::InputInfo constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    if (info[1].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set name of Input to InferenceEngineJS::InputInfo constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto cnnNetwork = info[0].As<Napi::External<InferenceEngine::CNNNetwork>>().Data();
    auto inputInfoName = std::string(info[1].As<Napi::String>());

    for(const auto &inputInfo: cnnNetwork->getInputsInfo()){
        if (inputInfo.first == inputInfoName){
            this->_ieInputInfo = inputInfo.second;
            return;
        }
    }
}

Napi::FunctionReference InferenceEngineJS::InputInfo::constructor;

Napi::Value InferenceEngineJS::InputInfo::getDims(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto dims = this->_ieInputInfo->getInputData()->getDims();

    return vectorToNapiArray<std::size_t, Napi::Number>(env, dims);
}

void InferenceEngineJS::InputInfo::setPrecision(const Napi::CallbackInfo &info) {
    auto precision = InferenceEngine::Precision::FromStr(std::string(info[0].ToString()));
    this->_ieInputInfo->setPrecision(precision);
}
void InferenceEngineJS::InputInfo::setLayout(const Napi::CallbackInfo &info) {
    auto layout = layoutFromString(std::string(info[0].ToString()));
    this->_ieInputInfo->setLayout(layout);
}

