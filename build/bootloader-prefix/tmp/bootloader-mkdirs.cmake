# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/idea/esp/esp-idf/components/bootloader/subproject"
  "F:/Test/build/bootloader"
  "F:/Test/build/bootloader-prefix"
  "F:/Test/build/bootloader-prefix/tmp"
  "F:/Test/build/bootloader-prefix/src/bootloader-stamp"
  "F:/Test/build/bootloader-prefix/src"
  "F:/Test/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Test/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
