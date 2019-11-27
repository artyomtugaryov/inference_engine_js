const { Core, CNNNetwork } = require('bindings')('InferenceEngineJS');

const patToModel = `${process.env.MODELS_PATH}/classification/inception_v3/inception_v3.`;

const ieCore = new Core();

//TODO: Load Extensions

const network = new CNNNetwork(`${patToModel}xml`, `${patToModel}bin`);

let inputInfo = network.getInputsInfo();

if (Object.keys(inputInfo).length > 1) {
    throw Error('Sample supports topologies with 1 input only');
}

inputInfo = inputInfo[0]; 

const inputLayerName = Object.keys(inputInfo);
const inputLayer = inputInfo[inputLayerName];

console.log(`Found input layer ${inputLayerName}`);

inputLayer.setPrecision('U8');
inputLayer.setLayout('NCHW');

//TODO: Read images
