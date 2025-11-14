cmake_minimum_required(VERSION 3.10)
project(ImageTreatmentSystem VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
# Check if OpenCV_DIR is set
if(NOT DEFINED OpenCV_DIR)
    message(FATAL_ERROR "OpenCV_DIR is not set! Please set it in CMakeSettings.json or as an environment variable.")
endif()

message(STATUS "Looking for OpenCV in: ${OpenCV_DIR}")

# Check if the directory exists
if(NOT EXISTS "${OpenCV_DIR}")
    message(FATAL_ERROR "OpenCV_DIR points to a non-existent directory: ${OpenCV_DIR}")
endif()

# Try to find OpenCV using the standard method first
find_package(OpenCV QUIET)

message(STATUS "After find_package: OpenCV_FOUND = ${OpenCV_FOUND}")

# If not found, try to locate it manually
if(NOT OpenCV_FOUND)
    message(STATUS "OpenCV not found by find_package, trying manual detection...")
    # Check common OpenCV installation paths
    if(EXISTS "${OpenCV_DIR}/include/opencv2/opencv.hpp")
        message(STATUS "Found OpenCV headers at: ${OpenCV_DIR}/include/opencv2/opencv.hpp")
        set(OpenCV_INCLUDE_DIRS "${OpenCV_DIR}/include")
        
        # Find library directory - try multiple possible locations
        set(OpenCV_LIB_DIR "")
        
        # Try vc17 (VS 2022), vc16 (VS 2019), vc15 (VS 2017)
        foreach(VC_VERSION vc17 vc16 vc15)
            set(TEST_LIB_DIR "${OpenCV_DIR}/x64/${VC_VERSION}/lib")
            if(EXISTS "${TEST_LIB_DIR}")
                # Check if there are any .lib files
                file(GLOB TEST_LIBS "${TEST_LIB_DIR}/*.lib")
                if(TEST_LIBS)
                    set(OpenCV_LIB_DIR "${TEST_LIB_DIR}")
                    message(STATUS "Found OpenCV libraries in: ${OpenCV_LIB_DIR}")
                    break()
                endif()
            endif()
        endforeach()
        
        # If still not found, search for any lib directory
        if(NOT OpenCV_LIB_DIR)
            message(STATUS "Searching for lib directories in: ${OpenCV_DIR}/x64")
            file(GLOB LIB_DIRS "${OpenCV_DIR}/x64/*/lib")
            foreach(LIB_DIR ${LIB_DIRS})
                file(GLOB TEST_LIBS "${LIB_DIR}/*.lib")
                if(TEST_LIBS)
                    set(OpenCV_LIB_DIR "${LIB_DIR}")
                    message(STATUS "Found OpenCV libraries in: ${OpenCV_LIB_DIR}")
                    break()
                endif()
            endforeach()
        endif()
        
        if(OpenCV_LIB_DIR)
            message(STATUS "Using OpenCV lib directory: ${OpenCV_LIB_DIR}")
            
            # Find OpenCV core library (required)
            find_library(OpenCV_CORE_LIB 
                NAMES opencv_core opencv_world
                PATHS ${OpenCV_LIB_DIR}
                NO_DEFAULT_PATH
            )
            message(STATUS "OpenCV_CORE_LIB = ${OpenCV_CORE_LIB}")
            
            # If find_library didn't work, try file(GLOB) instead
            if(NOT OpenCV_CORE_LIB)
                file(GLOB OpenCV_CORE_LIBS "${OpenCV_LIB_DIR}/opencv_core*.lib")
                file(GLOB OpenCV_WORLD_LIBS "${OpenCV_LIB_DIR}/opencv_world*.lib")
                if(OpenCV_WORLD_LIBS)
                    set(OpenCV_CORE_LIB ${OpenCV_WORLD_LIBS})
                elseif(OpenCV_CORE_LIBS)
                    set(OpenCV_CORE_LIB ${OpenCV_CORE_LIBS})
                endif()
            endif()
            
            if(OpenCV_CORE_LIB)
                # Check if opencv_world exists (single library with everything)
                file(GLOB OpenCV_WORLD_LIBS "${OpenCV_LIB_DIR}/opencv_world*.lib")
                if(OpenCV_WORLD_LIBS)
                    # Use opencv_world (single library)
                    set(OpenCV_LIBS ${OpenCV_WORLD_LIBS})
                else()
                    # Link individual core libraries
                    file(GLOB OpenCV_CORE_LIBS "${OpenCV_LIB_DIR}/opencv_core*.lib")
                    file(GLOB OpenCV_IMGPROC_LIBS "${OpenCV_LIB_DIR}/opencv_imgproc*.lib")
                    file(GLOB OpenCV_IMGCODECS_LIBS "${OpenCV_LIB_DIR}/opencv_imgcodecs*.lib")
                    file(GLOB OpenCV_VIDEOIO_LIBS "${OpenCV_LIB_DIR}/opencv_videoio*.lib")
                    file(GLOB OpenCV_HIGHGUI_LIBS "${OpenCV_LIB_DIR}/opencv_highgui*.lib")
                    
                    set(OpenCV_LIBS 
                        ${OpenCV_CORE_LIBS}
                        ${OpenCV_IMGPROC_LIBS}
                        ${OpenCV_IMGCODECS_LIBS}
                        ${OpenCV_VIDEOIO_LIBS}
                        ${OpenCV_HIGHGUI_LIBS}
                    )
                endif()
                
                set(OpenCV_FOUND TRUE)
                message(STATUS "OpenCV found manually at: ${OpenCV_DIR}")
                message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
                message(STATUS "OpenCV lib dir: ${OpenCV_LIB_DIR}")
                message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
            else()
                message(WARNING "OpenCV headers found but no core library found in ${OpenCV_LIB_DIR}")
            endif()
        else()
            message(WARNING "OpenCV headers found but no library directory found")
        endif()
    else()
        message(WARNING "OpenCV_DIR is set to ${OpenCV_DIR} but headers not found at ${OpenCV_DIR}/include/opencv2/opencv.hpp")
    endif()
endif()

# Final check
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found! Please set OpenCV_DIR to the OpenCV build directory (e.g., C:/opencv/build)")
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# Header files (for IDE support)
set(HEADER_FILES
    include/Treatment.h
    include/TreatmentChain.h
    include/ImageSource.h
    include/treatments/GaussianBlurTreatment.h
    include/treatments/CannyEdgeTreatment.h
    include/treatments/ThresholdTreatment.h
    include/treatments/BrightnessTreatment.h
    include/treatments/MedianBlurTreatment.h
    include/treatments/GrayscaleTreatment.h
    include/treatments/SharpenTreatment.h
    include/treatments/ErosionTreatment.h
    include/treatments/DilationTreatment.h
    include/treatments/MosaicTreatment.h
)

# Main executable
add_executable(image_treatment 
    src/test_webcam.cpp
    ${HEADER_FILES}
)

# Link OpenCV libraries
target_link_libraries(image_treatment ${OpenCV_LIBS})

# Add OpenCV include directories  
target_include_directories(image_treatment PRIVATE ${OpenCV_INCLUDE_DIRS})

# Install targets
install(TARGETS image_treatment DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/ImageTreatment)

# Print configuration
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

