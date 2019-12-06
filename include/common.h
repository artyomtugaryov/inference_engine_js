#ifndef INFERENCE_ENGINE_JS_COMMON_H
#define INFERENCE_ENGINE_JS_COMMON_H

#include <napi.h>
#include <ie_core.hpp>

namespace InferenceEngineJS {
    template<class InputType, class OutputType>
    Napi::Array vectorToNapiArray(const Napi::Env &env, const std::vector<InputType> &vec){
        Napi::Array result = Napi::Array::New(env);
        const auto size = vec.size();
        for (std::size_t i = 0; i < size; i++) {
            result[i] = OutputType::New(env, vec[i]);
        }
        return result;
    }

    template<class T, class K>
    Napi::Array tupleToNapiArray(const Napi::Env &env, const std::tuple<T, T, T> &tpl);

    template<class T, class K>
    Napi::Array tupleToNapiArray(const Napi::Env &env, const std::tuple<T, T> &tpl);

    Napi::Value parseParameter(const Napi::Env &env, const InferenceEngine::Parameter &param);

    std::map<std::string, std::string> objectToMap(const Napi::Object &object);

    Napi::Value mapToObject(const Napi::Object &object);

    InferenceEngine::Layout layoutFromString(const std::string& layout);
}
#endif //INFERENCE_ENGINE_JS_COMMON_H