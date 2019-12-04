const cv = require('opencv4nodejs');
const { flattenDeep } = require('lodash');
const {Core, CNNNetwork} = require('bindings')('InferenceEngineJS');

const patToModel = `${process.env.MODELS_PATH}/classification/inception_v3/inception_v3.`;

const ieCore = new Core();

//TODO: Load Extensions

const network = new CNNNetwork(`${patToModel}xml`, `${patToModel}bin`);

let inputInfo = network.getInputsInfo();

if (Object.keys(inputInfo).length > 1) {
    throw Error('Sample supports topologies with 1 input only');
}

inputInfo = inputInfo[0];

const inputLayerName = Object.keys(inputInfo)[0];
const inputLayer = inputInfo[inputLayerName];

console.log(`Found input layer ${inputLayerName}`);

inputLayer.setPrecision('U8');
inputLayer.setLayout('NCHW');

const sourceImage = cv.imread(process.env.IMAGE_PATH);

network.setBatchSize(1);

const executableNetwork = ieCore.loadNetwork(network, 'CPU');

const inferRequest = executableNetwork.createInferRequest();

const outputInfo = network.getOutputsInfo();
const outputLayerName = Object.keys(outputInfo[0])[0];

function toBGRArray(arrayOfArrays) {
    const bgrArrayOfArrays = arrayOfArrays.map((pixelsArray) => pixelsArray.map((pixelRGBArray) => pixelRGBArray.reverse()));
    return flattenDeep(bgrArrayOfArrays);
}

for (let i = 0, len = network.getInputsInfo().length; i < len; i++) {
    const inputLayerName = Object.keys(inputInfo)[0];
    const inputBlob = inferRequest.getBlob(inputLayerName);
    const dims = inputBlob.getDims();
    const image = sourceImage.resize(dims[2], dims[3]);
    const data = toBGRArray(image.getDataAsArray());
    inputBlob.fillImageAsU8(data);
}

inferRequest.infer();

constoutputBlob = inferRequest.getBlob(outputLayerName);
constoutputBlob.getTopClassificationResults(10);
console.log('done');