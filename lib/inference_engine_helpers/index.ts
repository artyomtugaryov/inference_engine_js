const path = require('path');

const common = require(path.join(__dirname, 'common'));
const classification = require(path.join(__dirname, 'classification'));
const object_detection = require(path.join(__dirname, 'object_detection'));

export = {
    ...common,
    ...classification,
    ...object_detection
}