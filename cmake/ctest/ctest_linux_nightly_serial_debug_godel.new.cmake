
INCLUDE("${CTEST_SCRIPT_DIRECTORY}/TrilinosCTestDriverCore.godel.gcc.cmake")

#
# Set the options specific to this build case
#

SET(COMM_TYPE SERIAL)
SET(BUILD_TYPE DEBUG)
SET(BUILD_DIR_NAME SERIAL_DEBUG)

#SET(CTEST_DO_COVERAGE_TESTING TRUE)
#SET(CTEST_DO_MEMORY_TESTING TRUE)

SET(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")

SET( EXTRA_CONFIGURE_OPTIONS
  "-DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}"
  "-DCMAKE_CXX_FLAGS:STRING=\"${COVERAGE_FLAGS}\""
  "-DCMAKE_C_FLAGS:STRING=\"${COVERAGE_FLAGS}\""
  "-DCMAKE_Fortran:STRING=\"${COVERAGE_FLAGS}\""
  "-DCMAKE_EXE_LINKER_FLAGS:STRING=\"${COVERAGE_FLAGS}\""
  "-DDART_TESTING_TIMEOUT:STRING=600"
  "-DCMAKE_VERBOSE_MAKEFILE:BOOL=TRUE"
  "-DTrilinos_ENABLE_DEPENCENCY_UNIT_TESTS:BOOL=OFF"
  "-DTrilinos_ENABLE_EXPLICIT_INSTANTIATION:BOOL=ON"
  "-DTPL_ENABLE_Boost:BOOL=ON"
  )

#
# Set the rest of the system-specific options and run the dashboard build/test
#

TRILINOS_SYSTEM_SPECIFIC_CTEST_DRIVER()
