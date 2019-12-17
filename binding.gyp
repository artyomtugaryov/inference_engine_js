{
  "targets": [
    { 
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags!": [ "-std=c++11" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "include_dirs" : [
        "<!@(node -p \"require('node-addon-api').include\")",
        "include",
        "<!@(node -p \"process.env.INTEL_OPENVINO_DIR\")/deployment_tools/inference_engine/include"
      ],
      "libraries": [
        "<!@(node -p \"process.env.INTEL_OPENVINO_DIR\")/deployment_tools/inference_engine/lib/intel64/libinference_engine.so"
      ],
      "target_name": "InferenceEngineJS",
      "sources":  [ '<!@(ls -1 src/*.cpp)' ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }
  ]
}
