#ifndef INFERENCEENGINE_JS_CNN_NETWORK_H
#define INFERENCEENGINE_JS_CNN_NETWORK_H

#include <napi.h>

#include <ie_core.hpp>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class CNNNetwork : public Napi::ObjectWrap<CNNNetwork> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        explicit CNNNetwork(const Napi::CallbackInfo &info);

        void setBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getBatchSize(const Napi::CallbackInfo &info);

        Napi::Value getPrecision(const Napi::CallbackInfo &info);

        Napi::Value getName(const Napi::CallbackInfo &info);

        Napi::Value layerCount(const Napi::CallbackInfo &info);

        Napi::Value getLayerByName(const Napi::CallbackInfo &info);

        Napi::Value getInputsInfo(const Napi::CallbackInfo &info);

        InferenceEngine::CNNNetwork* getCNNNetworkPtr();

    private:
        static Napi::FunctionReference constructor;

        InferenceEngine::CNNNetwork _ieNetwork;
    };
}
#endif //INFERENCEENGINE_JS_CNN_NETWORK_H
