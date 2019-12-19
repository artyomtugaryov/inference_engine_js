#include "common.h"
#include "data.h"


Napi::Object InferenceEngineJS::Data::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Data", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Data", func);
    return exports;
}

InferenceEngineJS::Data::Data(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Data>(info) {
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set pointer to CNNNetwork to InferenceEngineJS::InputInfo constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    if (info[1].IsUndefined()) {
        Napi::Error::New(info.Env(),"Set name of Input to InferenceEngineJS::InputInfo constructor for initialize").ThrowAsJavaScriptException();
        return;
    }
    auto cnnNetwork = info[0].As<Napi::External<InferenceEngine::CNNNetwork>>().Data();
    auto outputName = std::string(info[1].As<Napi::String>());

    for(const auto &outputInfo: cnnNetwork->getOutputsInfo()){
        if (outputInfo.first == outputName){
            this->_ieDataPtr = outputInfo.second;
            return;
        }
    }
}

Napi::FunctionReference InferenceEngineJS::Data::constructor;
