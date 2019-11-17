#ifndef INFERENCE_ENGINE_ADDON_IE_INPUT_INFO_H
#define INFERENCE_ENGINE_ADDON_IE_INPUT_INFO_H

#include <napi.h>
#include <ie_input_info.hpp>

namespace InferenceEngineJS {
    class IEInputInfo : public Napi::ObjectWrap<IEInputInfo> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        explicit IEInputInfo(const Napi::CallbackInfo &info);

    private:
        static Napi::FunctionReference constructor;

        InferenceEngine::InputInfo::Ptr _ieInputInfo;
    };
}

#endif //INFERENCE_ENGINE_ADDON_IE_INPUT_INFO_H
