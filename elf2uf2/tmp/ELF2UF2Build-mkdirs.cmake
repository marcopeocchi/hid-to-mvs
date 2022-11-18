# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/marco/sdk/pico-sdk/tools/elf2uf2"
  "/Users/marco/dev/homebrew/hid_controller/elf2uf2"
  "/Users/marco/dev/homebrew/hid_controller/elf2uf2"
  "/Users/marco/dev/homebrew/hid_controller/elf2uf2/tmp"
  "/Users/marco/dev/homebrew/hid_controller/elf2uf2/src/ELF2UF2Build-stamp"
  "/Users/marco/dev/homebrew/hid_controller/elf2uf2/src"
  "/Users/marco/dev/homebrew/hid_controller/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/marco/dev/homebrew/hid_controller/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
