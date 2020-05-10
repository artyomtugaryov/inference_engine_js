const path = require('path')
const { Core } = require('../lib/inference_engine');

if (!process.env.MODELS_PATH) {
    throw Error('"MODELS_PATH" environment variable is not set');
}
const patToModel = path.process.env.MODEL_PATH.split('.').slice(0, -1).join('.');

const ieCore = Core()

const network = new ieCore.readNetwork(`${patToModel}.xml`, `${patToModel}.bin`);

console.log(`Batch size of the ${network.getName()} network is ${network.getBatchSize()}.`);

console.log(`This network contains ${network.size()} layers.`);

console.log(`Input layer data has ${network.getInputsInfo()[0].value.getDims()} dimension.`);
