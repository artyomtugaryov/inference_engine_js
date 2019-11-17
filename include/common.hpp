#include <napi.h>
#include <ie_core.hpp>

namespace InferenceEngineJS {
    template<class T, class K>
    const Napi::Array vectorToNapiArray(const Napi::Env &env, const std::vector <T> &vec);

    template<class T, class K>
    const Napi::Array tupleToNapiArray(const Napi::Env &env, const std::tuple <T, T, T> &tpl);

    template<class T, class K>
    const Napi::Array tupleToNapiArray(const Napi::Env &env, const std::tuple <T, T> &tpl);

    Napi::Value parseParameter(const Napi::Env &env, const InferenceEngine::Parameter &param);

    std::map<std::string,std::string> objectToMap(const Napi::Object& object);
}


