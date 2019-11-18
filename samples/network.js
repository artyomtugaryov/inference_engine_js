const ie = require('bindings')('InferenceEngineAddon');

const patToModel = `${process.env.MODELS_PATH}/classification/inception_v3/inception_v3.`;

const ieNetwork = new ie.IENetwork(`${patToModel}xml`, `${patToModel}bin`);

console.log(`Batch size of the ${ieNetwork.getName()} network is ${ieNetwork.getBatchSize()}.`);

console.log(`This network contains ${ieNetwork.size()} layers.`);

console.log(`The network precision is  ${ieNetwork.getPrecision()} `);

console.log(`Input layer data has ${ieNetwork.getInputsInfo()[0].data.getDims()} dimension.`);
