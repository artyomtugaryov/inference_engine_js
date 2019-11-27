#ifndef INFERENCE_ENGINE_ADDON_IE_EXEC_NETWORK_H
#define INFERENCE_ENGINE_ADDON_IE_EXEC_NETWORK_H

#include <napi.h>

#include <ie_core.hpp>


namespace InferenceEngineJS {
    class IEExecNetwork : public Napi::ObjectWrap<IEExecNetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        IEExecNetwork(const Napi::CallbackInfo &info);

    private:
        static Napi::FunctionReference constructor;

        InferenceEngine::ExecutableNetwork::Ptr _ieExecNetworkPtr;

    };
}

#endif //INFERENCE_ENGINE_ADDON_IE_EXEC_NETWORK_H