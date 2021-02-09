const path = require('path');
const fs = require('fs');
const extendWithJsSources = require('./src')

const buildPath = path.join(__dirname, '..', 'build');

const libraryFileName = 'inference_engine_js';
const libraryExtension = 'node';

const releaseBuildType = 'Release';
let libPath = path.join(buildPath, releaseBuildType, libraryFileName);

if (!fs.existsSync(`${libPath}.${libraryExtension}`)) {
    const debugBuildType = 'Debug'
    libPath = path.join(buildPath, debugBuildType, libraryFileName);
    if (!fs.existsSync(`${libPath}.${libraryExtension}`)) {
        throw Error(`Cannot find library in folder ${buildPath}/{Release/Debug}/${libraryFileName})`);
    }
}

let inferenceEngineJS = require(libPath);
inferenceEngineJS = extendWithJsSources(inferenceEngineJS);

module.exports = inferenceEngineJS;