export class InputInfo {
    getDims(): number[];

    setPrecision(precision: string): null;

    setLayout(layout: string): null;
}

export class InputInfoMap {
    name: string;
    value: InputInfo;
}