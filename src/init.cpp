#include <napi.h>

#include "blob.h"
#include "cnn_network.h"
#include "core.h"
#include "data.h"
#include "executable_network.h"
#include "infer_request.h"
#include "input_info.h"

Napi::Object Init (Napi::Env env, Napi::Object exports) {
    InferenceEngineJS::Blob::Init(env, exports);
    InferenceEngineJS::CNNNetwork::Init(env, exports);
    InferenceEngineJS::Core::Init(env, exports);
    InferenceEngineJS::Data::Init(env, exports);
    InferenceEngineJS::ExecutableNetwork::Init(env, exports);
    InferenceEngineJS::InferRequest::Init(env, exports);
    InferenceEngineJS::InputInfo::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
