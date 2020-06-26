const path = require('path');
const requirePath = path.join(__dirname, '../build/Release/inference_engine_js')

let inferenceEngineJS = require(requirePath);

module.exports = inferenceEngineJS;