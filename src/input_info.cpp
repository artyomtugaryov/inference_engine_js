#include "common.h"
#include "input_info.h"

const char *InferenceEngineJS::InputInfo::classUTF8Name = "InputInfo";

Napi::Object InferenceEngineJS::InputInfo::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, InferenceEngineJS::InputInfo::classUTF8Name, {
            InstanceMethod("getDims", &InputInfo::getDims),
            InstanceMethod("setPrecision", &InputInfo::setPrecision),
            InstanceMethod("setLayout", &InputInfo::setLayout),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set(InferenceEngineJS::InputInfo::classUTF8Name, func);
    return exports;
}

void InferenceEngineJS::InputInfo::NewInstanceAsync(Napi::Env env,
                                                    const std::shared_ptr<InferenceEngine::CNNNetwork> &networkPtr,
                                                    Napi::Promise::Deferred &deferred) {
    auto createInputInfoWorker = new InferenceEngineJS::InputInfoFactoryAsyncWorker(env, networkPtr, deferred);
    createInputInfoWorker->Queue();
}

InferenceEngineJS::InputInfo::InputInfo(const Napi::CallbackInfo &info) : Napi::ObjectWrap<InputInfo>(info) {}

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

void InferenceEngineJS::InputInfo::setInputInfo(InferenceEngine::InputInfo::Ptr ieInputInfo) {
    this->_ieInputInfo = ieInputInfo;
}


InferenceEngineJS::InputInfoFactoryAsyncWorker::InputInfoFactoryAsyncWorker(Napi::Env &env,
                                                                            const std::shared_ptr<InferenceEngine::CNNNetwork> &cnnNetworkPtr,
                                                                            Napi::Promise::Deferred &deferred)
        : Napi::AsyncWorker(env),
          _cnnNetworkPtr(cnnNetworkPtr),
          _env(env),
          _deferred(deferred) {}

void InferenceEngineJS::InputInfoFactoryAsyncWorker::Execute() {
    try {
        this->_inputInfoMap = this->_cnnNetworkPtr->getInputsInfo();
    } catch (const std::exception &error) {
        Napi::AsyncWorker::SetError(error.what());
    }
}

void InferenceEngineJS::InputInfoFactoryAsyncWorker::OnOK() {
    Napi::EscapableHandleScope scope(this->_env);
    std::size_t i = 0;
    auto result = Napi::Array::New(this->_env, this->_inputInfoMap.size());
    for (const auto &inputInfo: this->_inputInfoMap) {
        auto ieInputInfoObj = InferenceEngineJS::InputInfo::constructor.New({});
        auto ieInputInfo = Napi::ObjectWrap<InferenceEngineJS::InputInfo>::Unwrap(ieInputInfoObj);
        ieInputInfo->setInputInfo(inputInfo.second);

        auto obj = Napi::Object::New(this->_env);
        obj.Set("name", Napi::String::New(this->_env, inputInfo.first));
        obj.Set("value", ieInputInfoObj);

        result[i++] = obj;
    }

    this->_deferred.Resolve(scope.Escape(napi_value(result)).ToObject());
}

void InferenceEngineJS::InputInfoFactoryAsyncWorker::OnError(Napi::Error const &error) {
    this->_deferred.Reject(error.Value());
}
