export class InputBlob {
    getDims(): number[];

    setPrecision(precision: string): null;

    setLayout(layout: string): null;

    fillWithU8(data: number[]): null;

    data(): number[];
}
