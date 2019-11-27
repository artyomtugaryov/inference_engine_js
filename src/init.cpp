#include <napi.h>


#include "core.h"
#include "executable_network.h"
#include "cnn_network.h"
#include "cnn_layer.h"
#include "input_info.h"

Napi::Object Init (Napi::Env env, Napi::Object exports) {
    InferenceEngineJS::Core::Init(env, exports);
    InferenceEngineJS::CNNNetwork::Init(env, exports);
    InferenceEngineJS::CNNLayer::Init(env, exports);
    InferenceEngineJS::InputInfo::Init(env, exports);
    InferenceEngineJS::ExecutableNetwork::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)