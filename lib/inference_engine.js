const path = require('path');

const requirePath = path.join(__dirname, '../build/Release/inference_engine_js')

const inference_engine_js = require(requirePath);

module.exports = inference_engine_js;
