#ifndef INFERENCEENGINE_JS_CORE_H
#define INFERENCEENGINE_JS_CORE_H

#include <vector>
#include <array>

#include <napi.h>

#include <ie_core.hpp>

#include "common.h"

namespace InferenceEngineJS {
    class Core : public Napi::ObjectWrap<Core> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        explicit Core(const Napi::CallbackInfo &info);

    private:
        static Napi::FunctionReference constructor;

        std::shared_ptr<InferenceEngine::Core> _ieCore;

        Napi::Value getVersion(const Napi::CallbackInfo &info);

        void addExtension(const Napi::CallbackInfo &info);

        Napi::Value readNetwork (const Napi::CallbackInfo &info);

        Napi::Value getMetric(const Napi::CallbackInfo &info);

        Napi::Value getAvailableDevices(const Napi::CallbackInfo &info);

        void registerPlugin(const Napi::CallbackInfo &info);

        void registerPlugins(const Napi::CallbackInfo &info);

        void unregisterPlugin(const Napi::CallbackInfo &info);

        Napi::Value loadNetwork(const Napi::CallbackInfo &info);
    };
}

#endif //INFERENCEENGINE_JS_CORE_H