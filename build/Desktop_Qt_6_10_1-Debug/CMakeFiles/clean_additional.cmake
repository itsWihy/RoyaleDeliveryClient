# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RoyaleDeliveryClient_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RoyaleDeliveryClient_autogen.dir/ParseCache.txt"
  "RoyaleDeliveryClient_autogen"
  )
endif()
