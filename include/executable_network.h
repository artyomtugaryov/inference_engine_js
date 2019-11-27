#ifndef INFERENCEENGINE_JS_IE_EXECUTABLE_NETWORK_H
#define INFERENCEENGINE_JS_IE_EXECUTABLE_NETWORK_H

#include <napi.h>

#include <inference_engine.hpp>


namespace InferenceEngineJS {
    class ExecutableNetwork : public Napi::ObjectWrap<ExecutableNetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static Napi::FunctionReference constructor;

        explicit ExecutableNetwork(const Napi::CallbackInfo &info);

        Napi::Value createInferRequest(const Napi::CallbackInfo &info);
    private:

        InferenceEngine::ExecutableNetwork::Ptr _ieExecNetworkPtr;

    };
}

#endif //INFERENCEENGINE_JS_IE_EXECUTABLE_NETWORK_H