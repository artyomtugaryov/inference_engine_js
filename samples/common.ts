import { imread, Rect, imwrite } from "opencv4nodejs";
import { ObjectDetectionPrediction } from "../lib";
const { postProcessing } = require('../lib/inference_engine');


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

function drawBoxInImage(sourceImage: string, outImage: string, box: ObjectDetectionPrediction) {
    const image = imread(sourceImage);

    const imageWidth = image.sizes[1];
    const imageHeight = image.sizes[0];

    const rectangle = new Rect(box.minX * imageWidth, box.minY * imageHeight, box.width * imageWidth, box.height * imageHeight);
    image.drawRectangle(rectangle);
    imwrite(outImage, image);
}

export {
    drawBoxInImage,
    printClassificationResult
}
