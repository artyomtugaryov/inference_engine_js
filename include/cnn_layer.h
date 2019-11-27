#ifndef INFERENCEENGINE_JS_CNN_LAYER_H
#define INFERENCEENGINE_JS_CNN_LAYER_H

#include <napi.h>

#include <ie_core.hpp>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class CNNLayer : public Napi::ObjectWrap<CNNLayer> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static Napi::FunctionReference constructor;

        explicit CNNLayer(const Napi::CallbackInfo &info);

        Napi::Value getParamAsString(const Napi::CallbackInfo &info);


    private:

        InferenceEngine::CNNLayerPtr _ieCNNLayer;
    };
}
#endif //INFERENCEENGINE_JS_CNN_LAYER_H
