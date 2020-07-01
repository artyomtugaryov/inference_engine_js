#ifndef INFERENCE_ENGINE_JS_INPUT_INFO_H
#define INFERENCE_ENGINE_JS_INPUT_INFO_H

#include <napi.h>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class InputInfo : public Napi::ObjectWrap<InputInfo> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static void NewInstanceAsync(Napi::Env env,
                                     const std::shared_ptr<InferenceEngine::CNNNetwork> &networkPtr,
                                     Napi::Promise::Deferred &deferred);

        explicit InputInfo(const Napi::CallbackInfo &info);

        static Napi::FunctionReference constructor;

        Napi::Value getDims(const Napi::CallbackInfo &info);

        void setPrecision(const Napi::CallbackInfo &info);

        void setLayout(const Napi::CallbackInfo &info);

        void setInputInfo(InferenceEngine::InputInfo::Ptr ieInputInfo);

    private:
        static const char *classUTF8Name;

        InferenceEngine::InputInfo::Ptr _ieInputInfo;
    };

    class InputInfoFactoryAsyncWorker : public Napi::AsyncWorker {
    public:
        InputInfoFactoryAsyncWorker(Napi::Env &env,
                                    const std::shared_ptr<InferenceEngine::CNNNetwork> &networkPtr,
                                    Napi::Promise::Deferred &deferred);

        ~InputInfoFactoryAsyncWorker() {}

        void Execute();

        void OnOK();

        void OnError(Napi::Error const &error);

    private:
        const std::shared_ptr<InferenceEngine::CNNNetwork> &_cnnNetworkPtr;
        InferenceEngine::InputsDataMap _inputInfoMap;

        Napi::Env _env;
        Napi::Promise::Deferred _deferred;
    };
}

#endif //INFERENCE_ENGINE_JS_INPUT_INFO_H
