{
  "targets": [
    {
      "target_name": "electron_desktop_frame",
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src"
      ],
      "sources": [
        "src/main.cc"
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "UNICODE"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "conditions": [
        [
          "OS=='win'",
          {
            "defines": [
              "_UNICODE",
              "_WIN32_WINNT=0x0601"
            ],
            "configurations": {
              "Release": {
                "msvs_settings": {
                  "VCCLCompilerTool": {
                    "ExceptionHandling": 1
                  }
                }
              },
              "Debug": {
                "msvs_settings": {
                  "VCCLCompilerTool": {
                    "ExceptionHandling": 1
                  }
                }
              }
            }
          }
        ]
      ]
    }
  ]
}
