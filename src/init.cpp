#include <napi.h>

#include "ie_core.h"
#include "ie_network.h"
#include "ie_layer.h"

Napi::Object Init (Napi::Env env, Napi::Object exports) {
    InferenceEngineJS::IECore::Init(env, exports);
    InferenceEngineJS::IENetwork::Init(env, exports);
    InferenceEngineJS::IELayer::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)