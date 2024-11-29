# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\smart_maintainance-center_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\smart_maintainance-center_autogen.dir\\ParseCache.txt"
  "smart_maintainance-center_autogen"
  )
endif()
