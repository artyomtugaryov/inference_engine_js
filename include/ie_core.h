#ifndef INFERENCE_ENGINE_ADDON_IE_CORE_H
#define INFERENCE_ENGINE_ADDON_IE_CORE_H

#include <vector>
#include <array>

#include <napi.h>

#include <ie_core.hpp>

#include "common.hpp"

namespace InferenceEngineJS {
    class IECore : public Napi::ObjectWrap<IECore> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        IECore(const Napi::CallbackInfo &info);

    private:
        static Napi::FunctionReference constructor;

        std::shared_ptr<InferenceEngine::Core> _ie_core;

        Napi::Value getVersion(const Napi::CallbackInfo &info);

        void addExtension(const Napi::CallbackInfo &info);

        void setConfig(const Napi::CallbackInfo &info);

        Napi::Value getMetric(const Napi::CallbackInfo &info);

        Napi::Value getAvailableDevices(const Napi::CallbackInfo &info);

        void registerPlugin(const Napi::CallbackInfo &info);

        void registerPlugins(const Napi::CallbackInfo &info);

        void unregisterPlugin(const Napi::CallbackInfo &info);
    };
}

#endif //INFERENCE_ENGINE_ADDON_IE_CORE_H