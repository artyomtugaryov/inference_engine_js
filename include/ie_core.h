#ifndef INFERENCE_ENGINE_ADDON_IE_CORE_H
#define INFERENCE_ENGINE_ADDON_IE_CORE_H


#include <ie_core.hpp>
#include <vector>
#include <array>

#include "common.hpp"

class IECore : public Napi::ObjectWrap<IECore> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        IECore(const Napi::CallbackInfo &info);

    private:
        static Napi::FunctionReference constructor;

        std::shared_ptr<InferenceEngine::Core> _ie_core;

        Napi::Value getAvailableDevices(const Napi::CallbackInfo &info);

        Napi::Value getMetric(const Napi::CallbackInfo &info);

        void loadNetwork(const Napi::CallbackInfo &info);
};

#endif //INFERENCE_ENGINE_ADDON_IE_CORE_H