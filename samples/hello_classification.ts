const cv = require('opencv4nodejs');
const {size} = require('lodash');
const {Core, CNNNetwork} = require('bindings')('InferenceEngineJS');

if (!process.env.MODELS_PATH) {
    throw Error('"MODELS_PATH" environment variable is not set');
}

const patToModel = `${process.env.MODELS_PATH}/classification/inception_v3/inception_v3.`;

const ieCore = new Core();

//TODO: Load Extensions

const network = new CNNNetwork(`${patToModel}xml`, `${patToModel}bin`);

let inputInfo = network.getInputsInfo();

if (size(Object.keys(inputInfo)) > 1) {
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

function toCHWArray(image: cv.Mat) {
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

function printClassificationResult(inferResultForImage: cv.Mat, topNumber: number = 10) {
    const indices = [];

    for (let i = 0; i < inferResultForImage.length; ++i) {
        indices[i] = i;
    }

    const sortedClasses = indices
        .map((item, index) => [inferResultForImage[index], item])
        .sort(([count1], [count2]) => count2 - count1)
        .map(([, item]) => item);

    console.log(`Top ${topNumber} results:`);
    console.log(`ClassID:\tProbability:`);
    for (let i = 0; i < topNumber; i++) {
        console.log(`${sortedClasses[i]}\t\t${inferResultForImage[sortedClasses[i]]}`)
    }

}

for (let i = 0, len = network.getInputsInfo().length; i < len; i++) {
    const inputLayerName = Object.keys(inputInfo)[0];
    const inputBlob = inferRequest.getBlob(inputLayerName);
    const dims = inputBlob.getDims();
    const image = sourceImage.resize(dims[2], dims[3]);
    const data = toCHWArray(image);
    inputBlob.fillWithU8(data);
}

inferRequest.infer();

const constOutputBlob = inferRequest.getBlob(outputLayerName);

const inferResults = constOutputBlob.getClassificationResult();

for (let batch = 0; batch < inferResults.length; ++batch) {

    const inferResultForImage = inferResults[batch];
    printClassificationResult(inferResultForImage);
}
