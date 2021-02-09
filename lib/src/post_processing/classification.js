function parseClassificationResults(inferenceResults, dims, threshold = 0.5) {
    const result = [];
    const batchSize = dims[0];
    const resultLength = dims[1];
    for (let b = 0; b < batchSize; b++) {
        const batchOffset = b * resultLength;
        result.push([])
        for (let i = 0; i < resultLength; i++) {
            result[b].push(inferenceResults[batchOffset + i]);
        }
    }
    return result;
}

module.exports = {
    parseClassificationResults
}