# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\minesweeper_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\minesweeper_autogen.dir\\ParseCache.txt"
  "minesweeper_autogen"
  )
endif()
