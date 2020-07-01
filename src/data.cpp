#include "common.h"
#include "data.h"

const char *InferenceEngineJS::Data::classUTF8Name = "Data";

Napi::Object InferenceEngineJS::Data::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, InferenceEngineJS::Data::classUTF8Name, {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set(InferenceEngineJS::Data::classUTF8Name, func);
    return exports;
}

void InferenceEngineJS::Data::NewInstanceAsync(Napi::Env env,
                                               const std::shared_ptr<InferenceEngine::CNNNetwork> &networkPtr,
                                               Napi::Promise::Deferred &deferred) {
    auto createOutputInfoWorker = new InferenceEngineJS::OutputsDataFactoryAsyncWorker(env, networkPtr, deferred);
    createOutputInfoWorker->Queue();
}

InferenceEngineJS::Data::Data(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Data>(info) {}

Napi::FunctionReference InferenceEngineJS::Data::constructor;

void InferenceEngineJS::Data::setDataPtr(InferenceEngine::DataPtr ieDataPtr) {
    this->_ieDataPtr = ieDataPtr;
}

InferenceEngineJS::OutputsDataFactoryAsyncWorker::OutputsDataFactoryAsyncWorker(Napi::Env &env,
                                                                                const std::shared_ptr<InferenceEngine::CNNNetwork> &cnnNetworkPtr,
                                                                                Napi::Promise::Deferred &deferred)
        : Napi::AsyncWorker(env),
          _cnnNetworkPtr(cnnNetworkPtr),
          _env(env),
          _deferred(deferred) {}

void InferenceEngineJS::OutputsDataFactoryAsyncWorker::Execute() {
    try {
        this->_outputInfoMap = this->_cnnNetworkPtr->getOutputsInfo();
    } catch (const std::exception &error) {
        Napi::AsyncWorker::SetError(error.what());
    }
}

void InferenceEngineJS::OutputsDataFactoryAsyncWorker::OnOK() {
    Napi::EscapableHandleScope scope(this->_env);
    std::size_t i = 0;
    auto result = Napi::Array::New(this->_env, this->_outputInfoMap.size());
    for (const auto &outputInfo: this->_outputInfoMap) {
        auto ieDataObj = InferenceEngineJS::Data::constructor.New({});
        auto ieData = Napi::ObjectWrap<InferenceEngineJS::Data>::Unwrap(ieDataObj);

        ieData->setDataPtr(outputInfo.second);

        auto obj = Napi::Object::New(this->_env);
        obj.Set("name", Napi::String::New(this->_env, outputInfo.first));
        obj.Set("value", ieDataObj);

        result[i++] = obj;
    }

    this->_deferred.Resolve(scope.Escape(napi_value(result)).ToObject());
}

void InferenceEngineJS::OutputsDataFactoryAsyncWorker::OnError(Napi::Error const &error) {
    this->_deferred.Reject(error.Value());
}
