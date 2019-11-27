#include "ie_network.h"
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
            InstanceMethod("loadNetwork", &IECore::loadNetwork),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IECore", func);

    return exports;
}

InferenceEngineJS::IECore::IECore(const Napi::CallbackInfo &info) : Napi::ObjectWrap<IECore>(info) {

    if (info[0].IsUndefined()) {
        this->_ieCore = std::make_shared<InferenceEngine::Core>();
    } else {
        auto xmlConfigFile = std::string(info[0].ToString());
        this->_ieCore = std::make_shared<InferenceEngine::Core>(xmlConfigFile);
    }
}

Napi::FunctionReference InferenceEngineJS::IECore::constructor;

Napi::Value InferenceEngineJS::IECore::getVersion(const Napi::CallbackInfo &info) {

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

void InferenceEngineJS::IECore::addExtension(const Napi::CallbackInfo &info) {

    auto extLibPath = std::string(info[0].ToString());
    auto deviceName = std::string(info[1].ToString());

    auto extension_ptr = InferenceEngine::make_so_pointer<InferenceEngine::IExtension>(extLibPath);
    auto extension = std::dynamic_pointer_cast<InferenceEngine::IExtension>(extension_ptr);

    this->_ieCore->AddExtension(extension, deviceName);
}

void InferenceEngineJS::IECore::setConfig(const Napi::CallbackInfo &info) {
    auto configObject = info[0].As<Napi::Object>();
    auto device = info[1].As<Napi::String>();
    auto params = objectToMap(configObject);
    this->_ieCore->SetConfig(params, device);
}

Napi::Value InferenceEngineJS::IECore::getMetric(const Napi::CallbackInfo &info) {

    auto device = std::string(info[0].ToString());
    auto metric = std::string(info[1].ToString());
    auto parameter = this->_ieCore->GetMetric(device, metric);

    return parseParameter(info.Env(), parameter);
}

Napi::Value InferenceEngineJS::IECore::getAvailableDevices(const Napi::CallbackInfo &info) {

    auto env = info.Env();
    auto result = Napi::Array::New(env);
    auto devices = this->_ieCore->GetAvailableDevices();
    auto size = devices.size();

    for (std::size_t i = 0; i < size; i++) {
        result[i] = Napi::String::New(env, devices[i]);
    }

    return result;
}

void InferenceEngineJS::IECore::registerPlugin(const Napi::CallbackInfo &info) {

    auto pluginName = std::string(info[0].ToString());
    auto deviceName = std::string(info[1].ToString());

    this->_ieCore->RegisterPlugin(pluginName, deviceName);

}

void InferenceEngineJS::IECore::unregisterPlugin(const Napi::CallbackInfo &info) {

    auto deviceName = std::string(info[0].ToString());

    this->_ieCore->UnregisterPlugin(deviceName);
}

void InferenceEngineJS::IECore::registerPlugins(const Napi::CallbackInfo &info) {
    auto xmlConfigFile = std::string(info[0].ToString());
    this->_ieCore->RegisterPlugins(xmlConfigFile);
}

Napi::Value InferenceEngineJS::IECore::loadNetwork(const Napi::CallbackInfo &info){
    if (info[0].IsUndefined()) {
        throw Napi::Error::New(info.Env(), "Set pointer to IENetwork to InferenceEngineJS::IECore loadNetwork for initialize");
    }
    std::size_t argc = 2;
    Napi::Value args[2];
    auto status = napi_get_cb_info(info.Env(), info, &argc, args, nullptr, nullptr);
    std::cout<< typeid(info[0]).name()<<std::endl;
    auto device = std::string(info[1].ToString());
//    auto ieNetworkObject = info[0].As<IENetwork>();
//    std::cout<< typeid(ieNetworkObject).name()<<std::endl;
//    auto ieExecNetwork = this->_ieCore->LoadNetwork(, device);
    return Napi::String::New(info.Env(), "");
}