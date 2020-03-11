# ESP-IDF template for a component. main is now treated as a component

# idf_component_register() is required to add the component (using the 
# variables set above) to the build. A library with the name of the 
# component will be built and linked into the final app. If this step 
# is skipped, this component will not be part of the build.

include(${CMAKE_CURRENT_LIST_DIR}/files.cmake)

idf_component_register(SRCS ${SOURCES}
        INCLUDE_DIRS
            ${CMAKE_CURRENT_LIST_DIR}
            $ENV{IDF_PATH}/components
        REQUIRES
            smooth_component
            gui-lvgl
        )
