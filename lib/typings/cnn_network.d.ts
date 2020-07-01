import { InputInfoMap } from './input_info';
import { OutputInfoMap } from './output_info';

export class CNNNetwork {
    setBatchSize(batch: number): null;

    getBatchSize(): number;

    getName(): string;

    layerCount(): number;

    size(): number;

    getInputsInfo(): Promise<InputInfoMap[]>;

    getOutputsInfo(): Promise<OutputInfoMap[]>;
}
