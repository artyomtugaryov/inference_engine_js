#include "ie_core.h"

Napi::Object InferenceEngineJS::IECore::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IECore", {
            InstanceMethod("getAvailableDevices", &IECore::getAvailableDevices),
            InstanceMethod("getMetric", &IECore::getMetric),
            InstanceMethod("getVersion", &IECore::getVersion),
            InstanceMethod("setConfig", &IECore::getVersion),
            InstanceMethod("addExtension", &IECore::addExtension),
            InstanceMethod("registerPlugin", &IECore::registerPlugin),
            InstanceMethod("registerPlugins", &IECore::registerPlugins),
            InstanceMethod("unregisterPlugin", &IECore::unregisterPlugin),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IECore", func);

    return exports;
}

InferenceEngineJS::IECore::IECore(const Napi::CallbackInfo &info) : Napi::ObjectWrap<IECore>(info) {

    if (info[0].IsUndefined()) {
        this->_ie_core = std::make_shared<InferenceEngine::Core>();
    } else {
        auto xmlConfigFile = std::string(info[0].ToString());
        this->_ie_core = std::make_shared<InferenceEngine::Core>(xmlConfigFile);
    }
}

Napi::FunctionReference InferenceEngineJS::IECore::constructor;

Napi::Value InferenceEngineJS::IECore::getVersion(const Napi::CallbackInfo &info) {

    auto env = info.Env();
    auto deviceName = std::string(info[0].ToString());

    auto versions = this->_ie_core->GetVersions(deviceName);
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

void InferenceEngineJS::IECore::addExtension(const Napi::CallbackInfo &info) {

    auto extLibPath = std::string(info[0].ToString());
    auto deviceName = std::string(info[1].ToString());

    auto extension_ptr = InferenceEngine::make_so_pointer<InferenceEngine::IExtension>(extLibPath);
    auto extension = std::dynamic_pointer_cast<InferenceEngine::IExtension>(extension_ptr);

    this->_ie_core->AddExtension(extension, deviceName);
}

void InferenceEngineJS::IECore::setConfig(const Napi::CallbackInfo &info) {
    auto configObject = info[0].As<Napi::Object>();
    auto device = info[1].As<Napi::String>();
    auto params = objectToMap(configObject);
    this->_ie_core->SetConfig(params, device);
}

Napi::Value InferenceEngineJS::IECore::getMetric(const Napi::CallbackInfo &info) {

    auto device = std::string(info[0].ToString());
    auto metric = std::string(info[1].ToString());
    auto parameter = this->_ie_core->GetMetric(device, metric);

    return parseParameter(info.Env(), parameter);
}

Napi::Value InferenceEngineJS::IECore::getAvailableDevices(const Napi::CallbackInfo &info) {

    auto env = info.Env();
    auto result = Napi::Array::New(env);
    auto devices = this->_ie_core->GetAvailableDevices();
    auto size = devices.size();

    for (auto i = 0; i < size; i++) {
        result[i] = Napi::String::New(env, devices[i]);
    }

    return result;
}

void InferenceEngineJS::IECore::registerPlugin(const Napi::CallbackInfo &info) {

    auto pluginName = std::string(info[0].ToString());
    auto deviceName = std::string(info[1].ToString());

    this->_ie_core->RegisterPlugin(pluginName, deviceName);

}

void InferenceEngineJS::IECore::unregisterPlugin(const Napi::CallbackInfo &info) {

    auto deviceName = std::string(info[0].ToString());

    this->_ie_core->UnregisterPlugin(deviceName);
}

void InferenceEngineJS::IECore::registerPlugins(const Napi::CallbackInfo &info) {
    auto xmlConfigFile = std::string(info[0].ToString());
    this->_ie_core->RegisterPlugins(xmlConfigFile);
}