#ifndef INFERENCE_ENGINE_ADDON_IE_EXEC_NETWORK_H
#define INFERENCE_ENGINE_ADDON_IE_EXEC_NETWORK_H

#include <napi.h>

#include <inference_engine.hpp>


namespace InferenceEngineJS {
    class ExecutableNetwork : public Napi::ObjectWrap<ExecutableNetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        ExecutableNetwork(const Napi::CallbackInfo &info);

        static Napi::FunctionReference constructor;
    private:

        InferenceEngine::ExecutableNetwork::Ptr _ieExecNetworkPtr;

    };
}

#endif //INFERENCE_ENGINE_ADDON_IE_EXEC_NETWORK_H