import { Mat, Vec3, imread, Rect, imwrite } from "opencv4nodejs";

function toCHWArray(image: Mat): number[] {
    const result = [];
    const height = image.rows;
    const width = image.cols;
    const channels = image.channels;
    for (let w = 0; w < width; w++) {
        for (let h = 0; h < height; h++) {
            const pixel: Vec3 = <Vec3><unknown>image.at(h, w);
            const rgb = [pixel.x, pixel.y, pixel.z];
            for (let c = 0; c < channels; c++) {
                result[c * height * width + h * height + w] = rgb[c];
            }
        }
    }
    return result;
}


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


function printClassificationResult(inferResults: number[], topNumber: number = 10) {
    const indices = [];

    for (let i = 0; i < inferResults.length; ++i) {
        indices[i] = i;
    }

    const sortedClasses = indices
        .map((item, index) => [inferResults[index], item])
        .sort(([count1], [count2]) => count2 - count1)
        .map(([, item]) => item);

    console.log(`Top ${topNumber} results:`);
    console.log(`ClassID:\tProbability:`);
    for (let i = 0; i < topNumber; i++) {
        console.log(`${sortedClasses[i]}\t\t${inferResults[sortedClasses[i]]}`)
    }
}

class ObjectDetectionPrediction {
    constructor(x1: number, y1: number, x2: number, y2: number, probability: number, classId: number) {
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

    private probability: number;
    private classId: number;
    readonly x1: number;
    readonly y1: number;
    readonly x2: number;
    readonly y2: number;
}

function parseSSDResults(inferenceResults: number[], dims: number[], threshold: number = 0.5): ObjectDetectionPrediction[] {
    const result: ObjectDetectionPrediction[] = [];

    const classIdIndex = 1;
    const predictionIndex = 2;
    const x1Index = 3;
    const y1Index = 4;
    const x2Index = 5;
    const y2Index = 6;
    const dataBoxLength = dims[dims.length - 1];

    for (let b: number = 0; b < dims[0]; b++) {
        const batchOffset = b * dims[2];
        for (let i: number = 0; i < dims[2]; i++) {
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

function drawBoxInImage(sourceImage: string, outImage: string, box: ObjectDetectionPrediction) {
    const image = imread(sourceImage);

    const imageWidth = image.sizes[1];
    const imageHeight = image.sizes[0];

    const rectangle = new Rect(box.minX * imageWidth, box.minY * imageHeight, box.width * imageWidth, box.height * imageHeight);
    image.drawRectangle(rectangle);
    imwrite(outImage, image);
}

export {
    toCHWArray,
    drawBoxInImage,
    parseSSDResults,
    ObjectDetectionPrediction,
    parseClassificationResults,
    printClassificationResult
}
