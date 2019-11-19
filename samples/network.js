const ie = require('bindings')('InferenceEngineJS');

const patToModel = `${process.env.MODEL_PATH}/classification/inception_v3/inception_v3.`;

const ieNetwork = new ie.IENetwork(`${patToModel}xml`, `${patToModel}bin`);

console.log(ieNetwork.getBatchSize());

console.log(ieNetwork.size());
console.log(ieNetwork.getLayerByName('pool').getParamAsString('pool-method'));
