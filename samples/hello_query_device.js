const { isString, isFinite, isArray } = require('lodash');
const { IECore } = require('bindings')('InferenceEngineAddon');

const ieCore = new IECore();

const availableDevices = ieCore.getAvailableDevices();

const mockMetricKey = 'MOCK_METRIC_KEY';
const mockMetric = {
    SUPPORTED_METRICS: [mockMetricKey],
    [mockMetricKey]: 1,
};

// Mock function until bindings implemented
ieCore.getMetric = (device, metricKey) => mockMetric[metricKey];

console.log('Available devices:');
for (device of availableDevices) {
    console.log("\tDevice: ", device);
    console.log("\tMetrics:");
    for (supportedMetricName of ieCore.getMetric(device, 'SUPPORTED_METRICS')) {
        try {
            const metric = ieCore.getMetric(device, supportedMetricName);
            console.log(`\t\t${supportedMetricName}: ${metric}`);
        } catch (e) {
            console.log(`\t\t${supportedMetricName}: UNSUPPORTED TYPE`);
        }
    }
}
