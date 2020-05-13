const path = require('path');
const fs = require('fs');

const buildPath = path.join(__dirname, '..', 'build');
const ieLibraryName = 'inference_engine_js';

let requirePath = path.join(buildPath,'Release', ieLibraryName)

if (!fs.existsSync(`${requirePath}.node`)) {
    requirePath = path.join(buildPath, 'Debug', ieLibraryName);
    if (!fs.existsSync(`${requirePath}.node`)) {
        throw  Error('Cannot find Inference Engine bindings library')
    }
}

const inference_engine = require(requirePath);

const inference_engine_helpers = require( path.join(__dirname, 'inference_engine_helpers'));

module.exports = inference_engine;
module.exports.helpers = inference_engine_helpers;
