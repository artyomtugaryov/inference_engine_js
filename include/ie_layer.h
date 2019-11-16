#ifndef INFERENCE_ENGINE_ADDON_IE_LAYER_H
#define INFERENCE_ENGINE_ADDON_IE_LAYER_H

#include <napi.h>

#include <ie_core.hpp>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class IELayer : public Napi::ObjectWrap<IELayer> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static IELayer New(const Napi::CallbackInfo &info, const InferenceEngine::CNNLayerPtr &layer);

        IELayer(const Napi::CallbackInfo &info);

    private:

        static Napi::FunctionReference constructor;

        InferenceEngine::CNNLayerPtr _ieCNNLayer;
    };
}
#endif //INFERENCE_ENGINE_ADDON_IE_LAYER_H
