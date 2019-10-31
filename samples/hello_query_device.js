const { IECore } = require('bindings')('InferenceEngineAddon');

const ieCore = new IECore();

const availableDevices = ieCore.getAvailableDevices();

// Mock function until bindings implemented
ieCore.getMetric = (device, metricKey) => [{ [metricKey]:'Metric value placeholder' }];

console.log('Available devices:');
for (device of availableDevices) {
    console.log("\tDevice: ", device);
    console.log("\tMetrics:");
    for (metric of ieCore.getMetric(device, 'MOCK_METRIC_KEY')) {
        for (metricKey in metric) {
            console.log(`\t\t${metricKey}: ${metric[metricKey]}`);
        }
    }
}
