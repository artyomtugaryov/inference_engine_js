import { InputBlob } from './input_blob';

export class InferRequest {
    getBlob(blobName: string): InputBlob;

    setCompletionCallback(callback: Function): null;

    startAsync(): null;
}
