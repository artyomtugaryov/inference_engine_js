const ie = require('bindings')('InferenceEngineJS');

const patToModel = `${process.env.MODELS_PATH}/classification/inception_v3/inception_v3.`;

const network_information = new ie.CNNNetwork(`${patToModel}xml`, `${patToModel}bin`);

console.log(`Batch size of the ${network_information.getName()} network is ${network_information.getBatchSize()}.`);

console.log(`This network contains ${network_information.size()} layers.`);

console.log(`The network precision is  ${network_information.getPrecision()} `);

console.log(`Input layer data has ${network_information.getInputsInfo()[0].data.getDims()} dimension.`);
