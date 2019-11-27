const { Core, CNNNetwork } = require('bindings')('InferenceEngineJS');

const patToModel = `${process.env.MODELS_PATH}/classification/inception_v3/inception_v3.`;

const ieCore = new Core();
const network = new CNNNetwork(`${patToModel}xml`, `${patToModel}bin`);

const inputInfo = network.getInputsInfo();

if (Object.keys(inputInfo).length > 1) {
    throw Error('Sample supports topologies with 1 input only');
}
const inputName = Object.getOwnPropertyNames(inputInfo)[0];
const inputLayer = inputInfo[inputName];

inputLayer.setPrecision('U8');
inputLayer.setLayout('NCHW');