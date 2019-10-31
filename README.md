# OpenVINO Inference Engine Node API addon

## Prerequisites
    1. Installed OpenVINO package
    2. Node v10

## Build
1. Install dependencies:
    ```sh
    npm install
    ```
2. Run the following command in a terminal `source $INTEL_OPENVINO_DIR/setupvars.sh`
    
    **Note:** To work in IDE add to `$LD_LIBRARY_PATH` environment variables as in `setupvars.sh`
3. Replace `$INTEL_OPENVINO_DIR` with path to your OpenVINO package in `binding.gyp` 
4. Build the addon:
    ```sh
    npm run build
    ```
5. Now you are available to use JS wrapper. To run sample execute:
    ```sh
    npm run sample
    ```
