{
  "targets": [
    { 
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags!": [ "-std=c++11" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "include_dirs" : [
        "<!@(node -p \"require('node-addon-api').include\")",
        "include",
        "$INTEL_OPENVINO_DIR/deployment_tools/inference_engine/include"
      ],
      "libraries": [
        "$INTEL_OPENVINO_DIR/deployment_tools/inference_engine/lib/intel64/libinference_engine.so"
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
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }
  ]
}
