#ifndef INFERENCEENGINE_JS_INFER_REQUEST_H
#define INFERENCEENGINE_JS_INFER_REQUEST_H

#include <napi.h>

#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class InferRequest : public Napi::ObjectWrap<InferRequest> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static Napi::FunctionReference constructor;

        explicit InferRequest(const Napi::CallbackInfo &info);

        Napi::Value getBlob(const Napi::CallbackInfo &info);

        void infer(const Napi::CallbackInfo &info);
    private:
        InferenceEngine::InferRequest::Ptr _inferRequestPtr;
    };
}


#endif //INFERENCEENGINE_JS_INFER_REQUEST_H
