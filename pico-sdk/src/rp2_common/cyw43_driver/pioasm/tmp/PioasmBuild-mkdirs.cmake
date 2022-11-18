# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/marco/sdk/pico-sdk/tools/pioasm"
  "/Users/marco/dev/homebrew/hid_controller/pioasm"
  "/Users/marco/dev/homebrew/hid_controller/pico-sdk/src/rp2_common/cyw43_driver/pioasm"
  "/Users/marco/dev/homebrew/hid_controller/pico-sdk/src/rp2_common/cyw43_driver/pioasm/tmp"
  "/Users/marco/dev/homebrew/hid_controller/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/marco/dev/homebrew/hid_controller/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src"
  "/Users/marco/dev/homebrew/hid_controller/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/marco/dev/homebrew/hid_controller/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
