export class CNNNetwork {
    setBatchSize(batch: number): null;

    getBatchSize(): number;

    getName(): string;

    layerCount(): number;

    size(): number;

    getInputsInfo(): Object;

    getOutputsInfo(): Object;
}
