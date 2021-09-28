[![Build Status](https://github.com/nu-book/zxing-cpp/workflows/CI/badge.svg?branch=master)](https://github.com/nu-book/zxing-cpp/actions?query=workflow%3ACI)

# ZXing-C++

ZXing-C++ ("zebra crossing") is an open-source, multi-format 1D/2D barcode image processing library implemented in C++.

It was originally ported from the Java [ZXing Library](https://github.com/zxing/zxing) but has been developed further and now includes many improvements in terms of quality and performance. It can both read and write barcodes in a number of formats.

## Features

* In pure C++17, no third-party dependencies
* Stateless, thread-safe readers/scanners and writers/generators
* Wrapper/Bindings for:
  * WinRT
  * Android
  * WebAssembly
  * [Python](wrappers/python/README.md)

## Supported Formats

| 1D product | 1D industrial     | 2D
| ---------- | ----------------- | --------------
| UPC-A      | Code 39           | QR Code
| UPC-E      | Code 93           | DataMatrix
| EAN-8      | Code 128          | Aztec
| EAN-13     | Codabar           | PDF417
| DataBar    | ITF               | MaxiCode (beta)
|            | DataBar Expanded  |

Note: DataBar used to be called RSS.

## Getting Started

### To read barcodes:
As an example, have a look at [`ZXingReader.cpp`](example/ZXingReader.cpp).
1. Load your image into memory (3rd-party library required).
2. Call `ReadBarcode()` from [`ReadBarcode.h`](core/src/ReadBarcode.h), the simplest API to get a `Result`.

### To write barcodes:
As an example, have a look at [`ZXingWriter.cpp`](example/ZXingWriter.cpp).
1. Create a [`MultiFormatWriter`](core/src/MultiFormatWriter.h) instance with the format you want to generate. Set encoding and margins if needed.
2. Call `encode()` with text content and the image size. This returns a [`BitMatrix`](core/src/BitMatrix.h) which is a binary image of the barcode where `true` == visual black and `false` == visual white.
3. Convert the bit matrix to your native image format. See also the `ToMatrix<T>(BitMatrix&)` helper function.

## Web Demos
- [Read barcodes](https://nu-book.github.io/zxing-cpp/demo_reader.html)
- [Write barcodes](https://nu-book.github.io/zxing-cpp/demo_writer.html)
- [Scan with camera](https://nu-book.github.io/zxing-cpp/zxing_viddemo.html)

## WinRT Package
A nuget package is available for WinRT: [huycn.zxingcpp.winrt](https://www.nuget.org/packages/huycn.zxingcpp.winrt). 
To install it, run the following command in the Package Manager Console
```sh
PM> Install-Package huycn.zxingcpp.winrt
```

## Build Instructions

### Standard setup on Windows/macOS/Linux
1. Make sure [CMake](https://cmake.org) version 3.10 or newer is installed.
2. Make sure a C++17 compliant compiler is installed (minimum VS 2019 16.8 / gcc 7 / clang 5)
3. See the cmake `BUILD_...` options to enable the testing code, python wrapper, etc.

### Windows Universal Platform
1. Download and install [CMake](https://cmake.org) 3.4 or more recent if it's not already installed.
2. Edit the file [`wrappers/winrt/BuildWinCom.bat`](wrappers/winrt/BuildWinCom.bat) to adjust the path to your CMake installation.
3. Double-click on the batch script to run it.
4. If the build succeeds, it will put the results in the folder UAP which is ready-to-use SDK extension.

### Android
1. Install AndroidStudio including NDK and CMake (see 'SDK Tools').
2. Open the project in folder [wrappers/android](wrappers/android).
3. The project contains 2 modules: `zxingcpp` is the wrapper library, `app` is the demo app using `zxingcpp`

### WebAssembly
1. [Install Emscripten](https://kripken.github.io/emscripten-site/docs/getting_started/) if not done already.
2. In an empty build folder, invoke `emcmake cmake <path to zxing-cpp.git/wrappers/wasm>`.
3. Invoke `cmake --build .` to create `zxing.js` and `zxing.wasm` (and `_reader`/`_writer` versions).
4. To see how to include these into a working HTML page, have a look at the [reader](wrappers/wasm/demo_reader.html) and [writer](wrappers/wasm/demo_writer.html) demos.
5. To quickly test your build, copy those demo files into your build directory and run e.g. `emrun --serve_after_close demo_reader.html`.

You can also download the latest build output from the continuous integration system from the [Actions](https://github.com/nu-book/zxing-cpp/actions) tab. Look for 'wasm-artifacts'. Also check out the [live demos](https://nu-book.github.io/zxing-cpp/).
