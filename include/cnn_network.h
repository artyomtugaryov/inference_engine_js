#ifndef INFERENCE_ENGINE_JS_CNN_NETWORK_H
#define INFERENCE_ENGINE_JS_CNN_NETWORK_H

#include <napi.h>

#include <ie_core.hpp>
#include <inference_engine.hpp>


namespace InferenceEngineJS {
    class CNNNetwork : public Napi::ObjectWrap<CNNNetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static void NewInstanceAsync(Napi::Env env,
                                     const Napi::String &modelPath,
                                     const Napi::String &weightsPath,
                                     const std::shared_ptr<InferenceEngine::Core> &ieCore,
                                     Napi::Promise::Deferred &deferred);

        static Napi::FunctionReference constructor;

        explicit CNNNetwork(const Napi::CallbackInfo &info);

        void setBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getName(const Napi::CallbackInfo &info);

        Napi::Value layerCount(const Napi::CallbackInfo &info);

        Napi::Value getInputsInfo(const Napi::CallbackInfo &info);

        Napi::Value getOutputsInfo(const Napi::CallbackInfo &info);

        const std::shared_ptr<InferenceEngine::CNNNetwork> getCNNNetworkPtr();

        void fromNGraphFunction(const std::shared_ptr< const ngraph::Function > ngraphFunction);

    private:
        static const char *classUTF8Name;

        std::shared_ptr<InferenceEngine::CNNNetwork> _ieNetwork;
    };

    class ReadNetworkAsyncWorker : public Napi::AsyncWorker {
    public:
        ReadNetworkAsyncWorker(Napi::Env &env,
                               const Napi::Value &modelPath,
                               const Napi::Value &weightsPath,
                               const std::shared_ptr<InferenceEngine::Core> &ieCore,
                               Napi::Promise::Deferred &deferred);

        ~ReadNetworkAsyncWorker() {}

        void Execute();

        void OnOK();

        void OnError(Napi::Error const &error);

    private:
        InferenceEngine::CNNNetwork _ieNetwork;
        const std::shared_ptr<InferenceEngine::Core> _ieCore;
        const std::string _modelPath;
        const std::string _weightsPath;
        Napi::Env _env;
        Napi::Promise::Deferred _deferred;
    };
}
#endif //INFERENCE_ENGINE_JS_CNN_NETWORK_H
