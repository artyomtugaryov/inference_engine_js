import { imread, Mat, Vec3 } from 'opencv4nodejs';
import { size } from 'lodash';

const { Core } = require('../lib/inference_engine');

if (!process.env.MODEL_PATH) {
    throw Error('"MODELS_PATH" environment variable is not set');
}

const patToModel = process.env.MODEL_PATH;

if (!process.env.IMAGE_PATH) {
    throw Error('"IMAGE_PATH" environment variable is not set');
}

const sourceImage = imread(process.env.IMAGE_PATH);

const ieCore = new Core();

//TODO: Load Extensions

const network = ieCore.readNetwork(`${patToModel}xml`, `${patToModel}bin`);

let inputInfo = network.getInputsInfo();

if (size(inputInfo) > 1) {
    throw Error('Sample supports topologies with 1 input only');
}

inputInfo = inputInfo[0];

const inputLayer = inputInfo.value;

inputLayer.setPrecision('U8');
inputLayer.setLayout('NCHW');

network.setBatchSize(1);

const executableNetwork = ieCore.loadNetwork(network, 'CPU');

const inferRequest = executableNetwork.createInferRequest();

function toCHWArray(image: Mat): number[] {
    const result = [];
    const height = image.rows;
    const width = image.cols;
    const channels = image.channels;
    for (let w = 0; w < width; w++) {
        for (let h = 0; h < height; h++) {
            const pixel: Vec3 = <Vec3><unknown>image.at(h, w);
            const rgb = [pixel.x, pixel.y, pixel.z];
            for (let c = 0; c < channels; c++) {
                result[c * height * width + h * height + w] = rgb[c];
            }
        }
    }
    return result;
}

function printClassificationResult(inferResultForImage: [],  topNumber: number = 10) {
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
    const inputLayerName = inputInfo.name;
    const inputBlob = inferRequest.getBlob(inputLayerName);
    const dims = inputBlob.getDims();
    const image = sourceImage.resize(dims[2], dims[3]);
    const data = toCHWArray(image);
    inputBlob.fillWithU8(data);
}

inferRequest.infer();

const outputInfo = network.getOutputsInfo();
const outputLayerName = outputInfo[0].name;

const outputBlob = inferRequest.getBlob(outputLayerName);

const inferResults = outputBlob.getClassificationResult();

for (let batch = 0; batch < inferResults.length; ++batch) {
    const inferResultForImage = inferResults[batch];
    printClassificationResult(inferResultForImage);
}

console.log('Done');
