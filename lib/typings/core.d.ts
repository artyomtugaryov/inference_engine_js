import {CNNNetwork} from "./cnn_network";

export class Core {
    getAvailableDevices(): string[];

    getMetric(device: string, metricName: string): any;

    getVersion(): string;

    readNetwork(xmlFile: string, binFile: string): CNNNetwork;

    loadNetwork(network: CNNNetwork): null;
}
