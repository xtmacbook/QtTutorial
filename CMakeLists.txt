CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
MESSAGE(STATUS "Found CMake ${CMAKE_VERSION}")

SET(CMAKE_WARN_DEPRECATED 1)

########### Project name ###########
PROJECT(ImageRes)
SET(VERSION 1.0)
SET(PACKAGE_VERSION "${VERSION}")

# currently Qt 5.6/5.9 is recommended
SET(MINIMAL_QT_VERSION "5.4.0")

# Use customized cmake macros
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# Show platform info
MESSAGE(STATUS "Platform: ${CMAKE_SYSTEM}")

IF(NOT RC_COMPILER_PATH)
    IF(NOT MSVC)
         #The mingGW snapshot distributed with the Qt SDK has it under this name.
        SET(RC_COMPILER_FILENAME "windres.exe")
        FIND_FILE(RC_COMPILER_PATH ${RC_COMPILER_FILENAME})
     ENDIF()
     IF(RC_COMPILER_PATH)
          MESSAGE(STATUS "Found .rc compiler: ${RC_COMPILER_PATH}")
     ENDIF(RC_COMPILER_PATH)
ENDIF(NOT RC_COMPILER_PATH)

#Generate icon file name #
SET(PACKAGE_ICON "ImgRes")
SET(PACKAGE_VERSION_RC "${PACKAGE_VERSION},0")
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/imgRes.rc.cmake ${CMAKE_BINARY_DIR}/imgRes.rc @ONLY)
	 
########### Find packages ###########
FIND_PACKAGE(Qt5Core REQUIRED)
GET_TARGET_PROPERTY(QMAKE_LOCATION Qt5::qmake LOCATION)
IF(${Qt5Core_VERSION} VERSION_LESS MINIMAL_QT_VERSION)
     MESSAGE(FATAL_ERROR "Found Qt5: ${QMAKE_LOCATION} (found unsuitable version \"${Qt5Core_VERSION}\", required is \"${MINIMAL_QT_VERSION}\")")
ELSE()
     MESSAGE(STATUS "Found Qt5: ${QMAKE_LOCATION} (found suitable version \"${Qt5Core_VERSION}\")")
ENDIF()
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

#opencv3
FIND_PACKAGE(OpenCV  REQUIRED)
MESSAGE( STATUS "    version: ${OpenCV_VERSION}" )

# Tell CMake to run moc when necessary:
SET(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake to always look for includes there:
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(
      ${CMAKE_BINARY_DIR}
      ${CMAKE_SOURCE_DIR}/src
      ${CMAKE_SOURCE_DIR}/src/core
      ${CMAKE_SOURCE_DIR}/src/core/modules
      ${CMAKE_SOURCE_DIR}/src/gui
      ${INTL_INCLUDE_DIR}
      ${CMAKE_BINARY_DIR}/src
)

########### Subdirectories ###############
ADD_SUBDIRECTORY( src )


