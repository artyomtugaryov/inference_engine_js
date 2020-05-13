import { Mat, Vec3 } from "opencv4nodejs";

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

export = {
    toCHWArray
};