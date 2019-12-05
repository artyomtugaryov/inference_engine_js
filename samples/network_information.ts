const ie = require('bindings')('InferenceEngineJS');

if (!process.env.MODELS_PATH) {
    throw Error('"MODELS_PATH" environment variable is not set');
}

const patToModel = `${process.env.MODELS_PATH}/classification/inception_v3/inception_v3.`;

const network = new ie.CNNNetwork(`${patToModel}xml`, `${patToModel}bin`);

console.log(`Batch size of the ${network.getName()} network is ${network.getBatchSize()}.`);

console.log(`This network contains ${network.size()} layers.`);

console.log(`The network precision is  ${network.getPrecision()} `);

console.log(`Input layer data has ${network.getInputsInfo()[0].data.getDims()} dimension.`);
