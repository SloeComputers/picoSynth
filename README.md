# picoSynth

Retro synth.

## Status

## Hardware

Re-uses the [picoX7](https://github.com/AnotherJohnH/picoX7) hardware platform. See [README](https://github.com/AnotherJohnH/Hardware/blob/main/picoX7/README.md)

## Software

### Checkout

This repo uses git sub-modules, so checkout using --recurse to clone all the
dependent source...

    git clone --recurse-submodules https://github.com/AnotherJohnH/picoSynth.git

or

    git clone --recurse-submodules ssh://git@github.com/AnotherJohnH/picoSynth.git

### Software dependencies

+ https://github.com/AnotherJohnH/Platform
+ arm-none-eabi-gcc
+ cmake via UNIX make or auto detection of ninja if installed
+ Python3
+ SDL2 (only for native test target)

### Build

Being developed on MacOS but should build on Linux too.

Indirect build of all supported targets, rpipico2 and native with cmake and make (or ninja)...

    make

Build a single hardware target e.g. rpipico2 using cmake...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rp2350 -DCMAKE_TOOLCHAIN_FILE=Platform/MTL/rp2350/target/toolchain.cmake ..
    make

flashable images will be found under the build sub-directory here...

    build/Source/picoSynth_RPIPICO_I2S_DAC.uf2

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgements

 + The raspberry-pi foundation for the innovative [RP2350](https://www.raspberrypi.com/documentation/microcontrollers/rp2350.html)
