# OpenVINO Inference Engine Node API addon (InferenceEngineJS)

## Prerequisites
1. Installed [OpenVINO](https://github.com/opencv/dldt) package
2. [Node v10](https://nodejs.org/dist/latest-v10.x/)

## Build
### Install dependencies:
```console
npm install
```

### Run the following command in a terminal to set environment for work with the [Inference Engine](https://github.com/opencv/dldt/tree/2019/inference-engine)
```console
source $INTEL_OPENVINO_DIR/bin/setupvars.sh
```
**Note:** To work in IDE add to `$LD_LIBRARY_PATH` environment variables as in `setupvars.sh`
### Build the addon. 
You can build the addon with `node-gyp` or `cmake`.
#### To build the addon with `node-gyp` you should:
1. Set `$INTEL_OPENVINO_DIR` environment variable with path to your OpenVINO package.
2. Run the following command in the terminal
    ```console
    npm run build
    ```
#### To build the addon with `cmake` you should:
1. Run a cmake command:
    ```console
    mkdir cmake-build && cd cmake-build && cmake ../
    ``` 
2. Build the addon:
    ```sh
    cmake --build . --target InferenceEngineAddon -- -j 6
    ```
3. Now you are available to use JS wrapper. To run sample execute:
    ```console
    npm run sample:hello-query-device
    ```
