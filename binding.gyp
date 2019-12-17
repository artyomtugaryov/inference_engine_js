{
  "targets": [
    {
      "cflags_cc": [ "-fexceptions", "-frtti", "-fpermissive"],
      "include_dirs" : [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(node -p \"process.env.INTEL_OPENVINO_DIR\")/deployment_tools/inference_engine/include",
        "include"
      ],
      "libraries": [
        "<!@(node -p \"process.env.INTEL_OPENVINO_DIR\")/deployment_tools/inference_engine/lib/intel64/libinference_engine.so"
      ],
      "target_name": "InferenceEngineJS",
      "sources":  [ '<!@(ls -1 src/*.cpp)' ],
    }
  ]
}
