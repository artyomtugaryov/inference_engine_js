// @ts-ignore
const { Core } = require('bindings')('InferenceEngineJS');

// @ts-ignore
const ieCore = new Core();

const availableDevices = ieCore.getAvailableDevices();


console.log('Available devices:');
for (let device of availableDevices) {
    console.log("\tDevice: ", device);
    console.log("\tMetrics:");
    for (let supportedMetricName of ieCore.getMetric(device, 'SUPPORTED_METRICS')) {
        try {
            const metric = ieCore.getMetric(device, supportedMetricName);
            console.log(`\t\t${supportedMetricName}: ${metric}`);
        } catch (e) {
            console.log(`\t\t${supportedMetricName}: UNSUPPORTED TYPE`);
        }
    }
}
