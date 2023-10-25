# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/expressif/esp-idf/components/bootloader/subproject"
  "D:/Git/workspace/vlp_local/hello_world/build/bootloader"
  "D:/Git/workspace/vlp_local/hello_world/build/bootloader-prefix"
  "D:/Git/workspace/vlp_local/hello_world/build/bootloader-prefix/tmp"
  "D:/Git/workspace/vlp_local/hello_world/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Git/workspace/vlp_local/hello_world/build/bootloader-prefix/src"
  "D:/Git/workspace/vlp_local/hello_world/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Git/workspace/vlp_local/hello_world/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Git/workspace/vlp_local/hello_world/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
