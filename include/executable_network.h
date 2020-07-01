#ifndef INFERENCE_ENGINE_JS_IE_EXECUTABLE_NETWORK_H
#define INFERENCE_ENGINE_JS_IE_EXECUTABLE_NETWORK_H

#include <napi.h>

#include <inference_engine.hpp>


namespace InferenceEngineJS {
    class ExecutableNetwork : public Napi::ObjectWrap<ExecutableNetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static Napi::FunctionReference constructor;
        static void NewInstanceAsync(Napi::Env& env,
                                     const std::shared_ptr<InferenceEngine::Core> &core,
                                     const std::shared_ptr<InferenceEngine::CNNNetwork> &network,
                                     const Napi::String &device,
                                     Napi::Promise::Deferred &deferred);

        explicit ExecutableNetwork(const Napi::CallbackInfo &info);

        Napi::Value createInferRequest(const Napi::CallbackInfo &info);

        void setExecutableNetwork(InferenceEngine::ExecutableNetwork &cnnNetwork);
    private:

        InferenceEngine::ExecutableNetwork _ieExecNetwork;

    };

    class LoadNetworkAsyncWorker : public Napi::AsyncWorker {
    public:
        LoadNetworkAsyncWorker(Napi::Env &env,
                               const std::shared_ptr<InferenceEngine::Core> &corePtr,
                               const std::shared_ptr<InferenceEngine::CNNNetwork> &networkPtr,
                               const Napi::String &device,
                               Napi::Promise::Deferred &deferred);

        ~LoadNetworkAsyncWorker() {}

        void Execute();

        void OnOK();

        void OnError(Napi::Error const &error);

    private:
        InferenceEngine::ExecutableNetwork _ieExecutableNetwork;
        const std::shared_ptr<InferenceEngine::Core> _corePtr;
        const std::shared_ptr<InferenceEngine::CNNNetwork> _cnnNetworkPtr;
        std::string _device;
        Napi::Env _env;
        Napi::Promise::Deferred _deferred;
    };
}

#endif //INFERENCE_ENGINE_JS_IE_EXECUTABLE_NETWORK_H