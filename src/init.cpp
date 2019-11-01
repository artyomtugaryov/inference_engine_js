#include <napi.h>

#include "ie_core.h"
#include "ie_network.h"

Napi::Object Init (Napi::Env env, Napi::Object exports) {
    IECore::Init(env, exports);
    IENetwork::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)