const { Core } = require('../lib/inference_engine');

async function main() {

    if (!process.env.MODEL_PATH) {
        throw Error('"MODEL_PATH" environment variable is not set');
    }

    const patToModel = process.env.MODEL_PATH.split('.').slice(0, -1).join('.');

    const ieCore = new Core()

    const network = await ieCore.readNetwork(`${patToModel}.xml`, `${patToModel}.bin`);

    console.log(`Batch size of the ${network.getName()} network is ${network.getBatchSize()}.`);

    console.log(`This network contains ${network.size()} layers.`);

    console.log(`Input layer data has ${network.getInputsInfo()[0].value.getDims()} dimension.`);
}

main();
