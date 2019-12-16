{
  "targets": [
    {
      "cflags_cc": [ "-fexceptions", "-frtti", "-fpermissive"],
      "include_dirs" : [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(node -p \"require('napi-thread-safe-callback').include\")",
        "<!@(node -p \"process.env.INTEL_OPENVINO_DIR\")/deployment_tools/inference_engine/include",
        "include"
      ],
      "libraries": [
        "<!@(node -p \"process.env.INTEL_OPENVINO_DIR\")/deployment_tools/inference_engine/lib/intel64/libinference_engine.so"
      ],
      "target_name": "InferenceEngineJS",
      "sources": [
        "src/blob.cpp",
        "src/cnn_layer.cpp",
        "src/cnn_network.cpp",
        "src/common.cpp",
        "src/core.cpp",
        "src/executable_network.cpp",
        "src/infer_request.cpp",
        "src/init.cpp",
        "src/input_info.cpp"
       ],
    }
  ]
}
