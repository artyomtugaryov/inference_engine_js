#include <napi.h>


#include "ie_core.h"
#include "ie_exec_network.h"
#include "ie_network.h"
#include "ie_layer.h"
#include "ie_input_info.h"

Napi::Object Init (Napi::Env env, Napi::Object exports) {
    InferenceEngineJS::IECore::Init(env, exports);
    InferenceEngineJS::IENetwork::Init(env, exports);
    InferenceEngineJS::IELayer::Init(env, exports);
    InferenceEngineJS::IEInputInfo::Init(env, exports);
    InferenceEngineJS::IEExecNetwork::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)