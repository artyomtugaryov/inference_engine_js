import { imread } from 'opencv4nodejs';
import { size } from 'lodash';
import { drawBoxInImage, ObjectDetectionPrediction, parseSSDResults, toCHWArray } from "./common";

const { Core } = require('../lib/inference_engine');

async function main() {

    if (!process.env.MODEL_PATH) {
        throw Error('"MODEL_PATH" environment variable is not set');
    }

    const patToModel = process.env.MODEL_PATH.split('.').slice(0, -1).join('.');

    if (!process.env.IMAGE_PATH) {
        throw Error('"IMAGE_PATH" environment variable is not set');
    }

    const pathToImage = process.env.IMAGE_PATH.toString();

    const sourceImage = imread(pathToImage);

    const ieCore = new Core();

    const network = await ieCore.readNetwork(`${patToModel}.xml`, `${patToModel}.bin`);

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
        const data = toCHWArray(image);
        inputBlob.fillWithU8(data);
    }

    inferRequest.infer();

    const outputInfo = network.getOutputsInfo();

    const outputLayerName = outputInfo[0].name;

    const outputBlob = inferRequest.getBlob(outputLayerName);

    const outData: number[] = outputBlob.data();
    const boxes: ObjectDetectionPrediction[] = parseSSDResults(outData, outputBlob.getDims());


    boxes.forEach((box: ObjectDetectionPrediction) => {
        drawBoxInImage(pathToImage, 'out.png', box);
    })

}

main();