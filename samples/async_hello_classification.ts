import {imread} from 'opencv4nodejs';
import {size} from 'lodash';
import {toCHWArray, printClassificationResult, parseClassificationResults} from './common';
import {ExecutableNetwork, InferRequest, InputInfo, InputInfoMap} from "../lib";

const {Core} = require('../lib/inference_engine');

async function main() {
    if (!process.env.MODEL_PATH) {
        throw Error('"MODEL_PATH" environment variable is not set');
    }

    const patToModel = process.env.MODEL_PATH.split('.').slice(0, -1).join('.');

    if (!process.env.IMAGE_PATH) {
        throw Error('"IMAGE_PATH" environment variable is not set');
    }

    const sourceImage = imread(process.env.IMAGE_PATH);

    const ieCore = new Core();

    const network = await ieCore.readNetwork(`${patToModel}.xml`, `${patToModel}.bin`);

    const inputLayerName = await network.getInputsInfo().then((inputInfoMap: InputInfoMap[]) => {
        if (size(inputInfoMap) > 1) {
            throw Error('Sample supports topologies with 1 input only');
        }
        inputInfoMap.forEach((inputInfoMap: InputInfoMap, index: number) => {
            const inputLayer: InputInfo = inputInfoMap.value;
            inputLayer.setPrecision('U8');
            inputLayer.setLayout('NCHW');
        })
        return inputInfoMap[0].name
    });

    network.setBatchSize(1);

    const inferRequest: InferRequest = await ieCore.loadNetwork(network, 'CPU')
        .then((executableNetwork: ExecutableNetwork) => executableNetwork.createInferRequest());

    const inputBlob = inferRequest.getBlob(inputLayerName);
    const dims = inputBlob.getDims();
    const image = sourceImage.resize(dims[2], dims[3]);
    const data = toCHWArray(image);
    inputBlob.fillWithU8(data);

    inferRequest.setCompletionCallback(() => {

        const outputInfo = network.getOutputsInfo();
        const outputLayerName = outputInfo[0].name;

        const outputBlob = inferRequest.getBlob(outputLayerName);

        const inferenceResults = outputBlob.data();

        const classificationResults = parseClassificationResults(inferenceResults, outputBlob.getDims());

        classificationResults.forEach((inferResultForImage: number[]) => {
            printClassificationResult(inferResultForImage);
        })
    });

    inferRequest.startAsync();

    console.log("End of main");

}

main();

console.log("End of the script");