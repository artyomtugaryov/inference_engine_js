#ifndef INFERENCE_ENGINE_JS_CNN_NETWORK_H
#define INFERENCE_ENGINE_JS_CNN_NETWORK_H

#include <napi.h>

#include <ie_core.hpp>
#include <inference_engine.hpp>

using namespace InferenceEngine;
using namespace Napi;

namespace InferenceEngineJS {
    class CNNNetwork : public Napi::ObjectWrap<CNNNetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static void NewInstanceAsync(Napi::Env env,
                                     const Napi::Value &model,
                                     const Napi::Value &weights,
                                     const InferenceEngine::Core *core,
                                     Napi::Promise::Deferred &deferred);

        static Napi::FunctionReference constructor;

        explicit CNNNetwork(const Napi::CallbackInfo &info);

        void setCNNNetwork(InferenceEngine::CNNNetwork& cnnNetwork);

        void setBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getName(const Napi::CallbackInfo &info);

        Napi::Value layerCount(const Napi::CallbackInfo &info);

        Napi::Value getInputsInfo(const Napi::CallbackInfo &info);

        Napi::Value getOutputsInfo(const Napi::CallbackInfo &info);

        InferenceEngine::CNNNetwork *getCNNNetworkPtr();

    private:
        InferenceEngine::CNNNetwork _ieNetwork;
    };

    class ReadNetworkAsyncWorker : public Napi::AsyncWorker {
    public:
        ReadNetworkAsyncWorker(Napi::Env &env,
                               const Napi::Value &model,
                               const Napi::Value &weights,
                               const InferenceEngine::Core* core,
                               Napi::Promise::Deferred &deferred);

        ~ReadNetworkAsyncWorker() {}

        void Execute();

        void OnOK();

        void OnError(Napi::Error const &error);

    private:
        InferenceEngine::CNNNetwork _ieNetwork;
        const InferenceEngine::Core* _core;
        const std::string _model_path;
        const std::string _weights_path;
        Napi::Env _env;
        Napi::Promise::Deferred _deferred;
    };
}
#endif //INFERENCE_ENGINE_JS_CNN_NETWORK_H
