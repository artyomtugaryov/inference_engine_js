export function parseSSDResults(inferenceResults: number[], dims: number[], threshold: number): number[];

export class ObjectDetectionPrediction {
    constructor(x1: number, y1: number, x2: number, y2: number, probability: number, classId: number);

    get width(): number;

    get height(): number;

    get minX(): number;

    get minY(): number;

}