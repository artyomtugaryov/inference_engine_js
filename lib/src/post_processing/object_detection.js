class ObjectDetectionPrediction {
    constructor(x1, y1, x2, y2, probability, classId) {
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
        this.probability = probability;
        this.classId = classId;
    }

    get width() {
        return Math.abs(this.x1 - this.x2);
    }

    get height() {
        return Math.abs(this.y1 - this.y2);
    }

    get minX() {
        return Math.min(this.x1, this.x2);
    }

    get minY() {
        return Math.min(this.y1, this.y2);
    }
}

function parseSSDResults(inferenceResults, dims, threshold = 0.5) {
    const result = [];

    const classIdIndex = 1;
    const predictionIndex = 2;
    const x1Index = 3;
    const y1Index = 4;
    const x2Index = 5;
    const y2Index = 6;
    const dataBoxLength = dims[dims.length - 1];

    for (let b = 0; b < dims[0]; b++) {
        const batchOffset = b * dims[2];
        for (let i = 0; i < dims[2]; i++) {
            const curProposal = batchOffset + dataBoxLength * i;

            const prediction = inferenceResults[curProposal + predictionIndex];
            const classId = inferenceResults[curProposal + classIdIndex];
            const x1 = inferenceResults[curProposal + x1Index];
            const y1 = inferenceResults[curProposal + y1Index];
            const x2 = inferenceResults[curProposal + x2Index];
            const y2 = inferenceResults[curProposal + y2Index];

            if (prediction < threshold) {
                continue;
            }

            result.push(new ObjectDetectionPrediction(x1, y1, x2, y2, prediction, classId));
        }
    }
    return result;
}

module.exports = {
    ObjectDetectionPrediction,
    parseSSDResults
}