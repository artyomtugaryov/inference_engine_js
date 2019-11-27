#include <executable_network.h>
#include "cnn_network.h"
#include "core.h"

Napi::Object InferenceEngineJS::Core::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Core", {
            InstanceMethod("getAvailableDevices", &Core::getAvailableDevices),
            InstanceMethod("getMetric", &Core::getMetric),
            InstanceMethod("getVersion", &Core::getVersion),
            InstanceMethod("setConfig", &Core::getVersion),
            InstanceMethod("addExtension", &Core::addExtension),
            InstanceMethod("registerPlugin", &Core::registerPlugin),
            InstanceMethod("registerPlugins", &Core::registerPlugins),
            InstanceMethod("unregisterPlugin", &Core::unregisterPlugin),
            InstanceMethod("loadNetwork", &Core::loadNetwork),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Core", func);

    return exports;
}

InferenceEngineJS::Core::Core(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Core>(info) {

    if (info[0].IsUndefined()) {
        this->_ieCore = std::make_shared<InferenceEngine::Core>();
    } else {
        auto xmlConfigFile = std::string(info[0].ToString());
        this->_ieCore = std::make_shared<InferenceEngine::Core>(xmlConfigFile);
    }
}

Napi::FunctionReference InferenceEngineJS::Core::constructor;

Napi::Value InferenceEngineJS::Core::getVersion(const Napi::CallbackInfo &info) {

    auto env = info.Env();
    auto deviceName = std::string(info[0].ToString());

    auto versions = this->_ieCore->GetVersions(deviceName);
    std::stringstream ss;
    for (auto version: versions) {
        auto d = version.first;
        auto v = version.second;
        ss << "Device\t" << d;
        ss << "\t" << v.description << " version ......... ";
        ss << v.apiVersion.major << "." << v.apiVersion.minor;

        ss << "\n\tBuild ........... ";
        ss << v.buildNumber;
    }
    return Napi::String::New(env, ss.str());
}

void InferenceEngineJS::Core::addExtension(const Napi::CallbackInfo &info) {

    auto extLibPath = std::string(info[0].ToString());
    auto deviceName = std::string(info[1].ToString());

    auto extension_ptr = InferenceEngine::make_so_pointer<InferenceEngine::IExtension>(extLibPath);
    auto extension = std::dynamic_pointer_cast<InferenceEngine::IExtension>(extension_ptr);

    this->_ieCore->AddExtension(extension, deviceName);
}

void InferenceEngineJS::Core::setConfig(const Napi::CallbackInfo &info) {
    auto configObject = info[0].As<Napi::Object>();
    auto device = info[1].As<Napi::String>();
    auto params = objectToMap(configObject);
    this->_ieCore->SetConfig(params, device);
}

Napi::Value InferenceEngineJS::Core::getMetric(const Napi::CallbackInfo &info) {

    auto device = std::string(info[0].ToString());
    auto metric = std::string(info[1].ToString());
    auto parameter = this->_ieCore->GetMetric(device, metric);

    return parseParameter(info.Env(), parameter);
}

Napi::Value InferenceEngineJS::Core::getAvailableDevices(const Napi::CallbackInfo &info) {

    auto env = info.Env();
    auto result = Napi::Array::New(env);
    auto devices = this->_ieCore->GetAvailableDevices();
    auto size = devices.size();

    for (std::size_t i = 0; i < size; i++) {
        result[i] = Napi::String::New(env, devices[i]);
    }

    return result;
}

void InferenceEngineJS::Core::registerPlugin(const Napi::CallbackInfo &info) {

    auto pluginName = std::string(info[0].ToString());
    auto deviceName = std::string(info[1].ToString());

    this->_ieCore->RegisterPlugin(pluginName, deviceName);

}

void InferenceEngineJS::Core::unregisterPlugin(const Napi::CallbackInfo &info) {

    auto deviceName = std::string(info[0].ToString());
    this->_ieCore->UnregisterPlugin(deviceName);
}

void InferenceEngineJS::Core::registerPlugins(const Napi::CallbackInfo &info) {

    auto xmlConfigFile = std::string(info[0].ToString());
    this->_ieCore->RegisterPlugins(xmlConfigFile);
}

Napi::Value InferenceEngineJS::Core::loadNetwork(const Napi::CallbackInfo &info){
    auto env = info.Env();
    if (info[0].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set CNNNetwork to InferenceEngineJS::Core loadNetwork for initialize").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    if (info[1].IsUndefined()) {
        Napi::Error::New(info.Env(), "Set device to InferenceEngineJS::Core loadNetwork for load Network").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    auto ieNetworkPtr = Napi::ObjectWrap<InferenceEngineJS::CNNNetwork>::Unwrap(info[0].As<Napi::Object>());
    auto execNetworkObject = ExecutableNetwork::constructor.New({
        Napi::External<InferenceEngine::CNNNetwork>::New(env, ieNetworkPtr->getCNNNetworkPtr()),
        Napi::External<InferenceEngine::Core>::New(env, this->_ieCore.get()),
        info[1]
    });
    return execNetworkObject;
}
