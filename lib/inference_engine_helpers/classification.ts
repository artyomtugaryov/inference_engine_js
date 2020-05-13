function parseClassificationResults(inferenceResults: number[], dims: number[], threshold: number = 0.5): number[][] {
    const result: number[][] = [];
    const batchSize = dims[0];
    const resultLength = dims[1];
    for (let b: number = 0; b < batchSize; b++) {
        const batchOffset = b * resultLength;
        result.push([])
        for (let i: number = 0; i < resultLength; i++) {
            result[b].push(inferenceResults[batchOffset + i]);
        }
    }
    return result;
}

export = {
    parseClassificationResults
};