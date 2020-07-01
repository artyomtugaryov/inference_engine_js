#ifndef INFERENCE_ENGINE_JS_DATA_H
#define INFERENCE_ENGINE_JS_DATA_H

#include <napi.h>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class Data : public Napi::ObjectWrap<Data> {
    public:
        static void NewInstanceAsync(Napi::Env env,
                                     const std::shared_ptr<InferenceEngine::CNNNetwork> &networkPtr,
                                     Napi::Promise::Deferred &deferred);

        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        explicit Data(const Napi::CallbackInfo &info);

        static Napi::FunctionReference constructor;

        void setDataPtr(InferenceEngine::DataPtr ieDataPtr);

    private:
        static const char *classUTF8Name;

        InferenceEngine::DataPtr _ieDataPtr;
    };

    class OutputsDataFactoryAsyncWorker : public Napi::AsyncWorker {
    public:
        OutputsDataFactoryAsyncWorker(Napi::Env &env,
                                      const std::shared_ptr<InferenceEngine::CNNNetwork> &networkPtr,
                                      Napi::Promise::Deferred &deferred);

        ~OutputsDataFactoryAsyncWorker() {}

        void Execute();

        void OnOK();

        void OnError(Napi::Error const &error);

    private:
        const std::shared_ptr<InferenceEngine::CNNNetwork> &_cnnNetworkPtr;
        InferenceEngine::OutputsDataMap _outputInfoMap;

        Napi::Env _env;
        Napi::Promise::Deferred _deferred;
    };
}

#endif //INFERENCE_ENGINE_JS_DATA_H
