#ifndef INFERENCEENGINE_JS_INPUT_INFO_H
#define INFERENCEENGINE_JS_INPUT_INFO_H

#include <napi.h>
#include <inference_engine.hpp>

namespace InferenceEngineJS {
    class InputInfo : public Napi::ObjectWrap<InputInfo> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        explicit InputInfo(const Napi::CallbackInfo &info);

        static Napi::FunctionReference constructor;

        Napi::Value getDims(const Napi::CallbackInfo &info);

        void setPrecision(const Napi::CallbackInfo &info);
        
        void setLayout(const Napi::CallbackInfo &info);


    private:

        InferenceEngine::InputInfo::Ptr _ieInputInfo;
    };
}

#endif //INFERENCEENGINE_JS_INPUT_INFO_H
