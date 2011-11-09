CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Comphoto2011)
OPTION (${PROJECT_NAME}_BUILD_TESTS "Build all of unit tests." ON)
SET (PROJECT_MAIN_EXECUTABLE server)

# ----------------------------------------
# check if OpenCV exists and version > 2.0
# ----------------------------------------
FIND_PACKAGE (OpenCV REQUIRED)
IF (${OpenCV_VERSION} VERSION_LESS 2.0.0)
  MESSAGE (FATAL_ERROR "OpenCV version is not compatible: ${OpenCV_VERSION}")
ENDIF()
INCLUDE_DIRECTORIES (${OpenCV_INCLUDE_DIRS})

# ----------------------------------------
# check if Wt exists
# ----------------------------------------
FIND_PACKAGE (Wt REQUIRED)


#SET (CMAKE_CXX_FLAGS "-g -Wall")
ADD_DEFINITIONS (-Wall -ansi -Wno-deprecated)


SET (EXECUTABLE_OUTPUT_PATH ${Comphoto2011_BINARY_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${Comphoto2011_BINARY_DIR}/lib)

# -------------------------------------
# set common include folders for module
# -------------------------------------
SET (COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
SET (PROJECT_LIBS ${PROJECT_BINARY_DIR}/lib)

INCLUDE_DIRECTORIES (${COMMON_INCLUDES})
#LINK_DIRECTORIES (${PROJECT_BINARY_DIR}/lib)


# -----------------------
# build custom libraries
# -----------------------
SET (FILTER_LIBS guidedfilter boxfilter)
FOREACH (filter ${FILTER_LIBS})
  ADD_LIBRARY (${filter} src/${filter}.cpp)
ENDFOREACH(filter)

# -----------------------
# build test cases
# -----------------------
IF (${PROJECT_NAME}_BUILD_TESTS)
  SET (UNIT_TESTS test_guidedfilter test_boxfilter)
  ENABLE_TESTING()
  FIND_PACKAGE (GTest REQUIRED)
  INCLUDE_DIRECTORIES (${GTEST_INCLUDE_DIRS})

  FOREACH (unit_test ${UNIT_TESTS})
    ADD_EXECUTABLE (${unit_test} test/${unit_test}.cpp)
    TARGET_LINK_LIBRARIES (${unit_test} ${OpenCV_LIBS} ${FILTER_LIBS} ${GTEST_BOTH_LIBRARIES} pthread)
    ADD_TEST (${unit_test} ${unit_test})
  ENDFOREACH (unit_test)
ENDIF()

# -----------------------
# build user interfaces
# -----------------------
SET (UI_LIBS project1 processedimage)
FIND_PACKAGE (Wt REQUIRED)
INCLUDE_DIRECTORIES (${Wt_INCLUDE_DIR})
FOREACH (ui ${UI_LIBS})
  ADD_LIBRARY (${ui} src/${ui}.cpp)
ENDFOREACH(ui)


# -------------------------------------
# build demo program
# -------------------------------------
ADD_EXECUTABLE (demo demo.cpp)
TARGET_LINK_LIBRARIES (demo
    ${OpenCV_LIBS}
    ${FILTER_LIBS}
)

# -------------------------------------
# build project executable
# -------------------------------------
SET (PROJECT_TARGET ${PROJECT_MAIN_EXECUTABLE}.wt)

ADD_EXECUTABLE (${PROJECT_TARGET} main.cpp)
TARGET_LINK_LIBRARIES (${PROJECT_TARGET}
    ${Wt_LIBRARY}
    ${Wt_HTTP_LIBRARY}
    ${OpenCV_LIBS}
    ${UI_LIBS} 
    ${FILTER_LIBS}
)

