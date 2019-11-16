#ifndef INFERENCE_ENGINE_ADDON_IE_NETWORK_H
#define INFERENCE_ENGINE_ADDON_IE_NETWORK_H

#include <napi.h>

#include <ie_core.hpp>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class IENetwork : public Napi::ObjectWrap<IENetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        explicit IENetwork(const Napi::CallbackInfo &info);

        void setBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getPrecision(const Napi::CallbackInfo &info);

        Napi::Value getName(const Napi::CallbackInfo &info);

        Napi::Value layerCount(const Napi::CallbackInfo &info);

    private:
        static Napi::FunctionReference constructor;

        InferenceEngine::CNNNetwork _ieNetwork;
    };
}
#endif //INFERENCE_ENGINE_ADDON_IE_NETWORK_H
