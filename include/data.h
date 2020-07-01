#ifndef INFERENCE_ENGINE_JS_DATA_H
#define INFERENCE_ENGINE_JS_DATA_H

#include <napi.h>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class Data : public Napi::ObjectWrap<Data> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        explicit Data(const Napi::CallbackInfo &info);

        static Napi::FunctionReference constructor;

    private:

        InferenceEngine::DataPtr _ieDataPtr;
    };
}

#endif //INFERENCE_ENGINE_JS_DATA_H
