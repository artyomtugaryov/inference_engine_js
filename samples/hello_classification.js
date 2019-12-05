const cv = require('opencv4nodejs');
const {flattenDeep, flatten} = require('lodash');
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

function toCHWArray(image) {
    const result = [];
    const height = image.rows;
    const width = image.cols;
    const channels = image.channels;
    for (let w = 0; w < width; w++) {
        for (let h = 0; h < height; h++) {
            const pixel = image.at(h, w);
            const rgb = [pixel.x, pixel.y, pixel.z];
            for (let c = 0; c < channels; c++) {
                result[c * height * width + h * height + w] = rgb[c];
            }
        }
    }
    return result;
}


for (let i = 0, len = network.getInputsInfo().length; i < len; i++) {
    const inputLayerName = Object.keys(inputInfo)[0];
    const inputBlob = inferRequest.getBlob(inputLayerName);
    const dims = inputBlob.getDims();
    const image = sourceImage.resize(dims[2], dims[3]);
    const data = toCHWArray(image);
    inputBlob.fillImageAsU8(data);
}

inferRequest.infer();

constoutputBlob = inferRequest.getBlob(outputLayerName);
constoutputBlob.getTopClassificationResults(100);
console.log('done');