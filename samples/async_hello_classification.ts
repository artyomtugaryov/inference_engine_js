import { imread } from 'opencv4nodejs';
import { size } from 'lodash';
import { printClassificationResult } from './common';
const { Core, helpers } = require('../lib/inference_engine');

if (!process.env.MODEL_PATH) {
    throw Error('"MODEL_PATH" environment variable is not set');
}

const patToModel = process.env.MODEL_PATH.split('.').slice(0, -1).join('.');

if (!process.env.IMAGE_PATH) {
    throw Error('"IMAGE_PATH" environment variable is not set');
}

const sourceImage = imread(process.env.IMAGE_PATH);

const ieCore = new Core();

const network = ieCore.readNetwork(`${patToModel}.xml`, `${patToModel}.bin`);

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

for (let i = 0, len = network.getInputsInfo().length; i < len; i++) {
    const inputLayerName = inputInfo.name;
    const inputBlob = inferRequest.getBlob(inputLayerName);
    const dims = inputBlob.getDims();
    const image = sourceImage.resize(dims[2], dims[3]);
    const data = helpers.toCHWArray(image);
    inputBlob.fillWithU8(data);
}

inferRequest.setCompletionCallback(()=>{

    const outputInfo = network.getOutputsInfo();
    const outputLayerName = outputInfo[0].name;

    const outputBlob = inferRequest.getBlob(outputLayerName);

    const inferenceResults = outputBlob.data();

    const classificationResults = helpers.parseClassificationResults(inferenceResults, outputBlob.getDims());

    classificationResults.forEach((inferResultForImage: number[]) => {
        printClassificationResult(inferResultForImage);
    })
});

inferRequest.startAsync();

console.log("End of the script");

export {}