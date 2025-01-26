cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(u6o7 CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if (NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "Win32")
endif ()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if (NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif ()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
        "Debug"
        "Release"
        CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if (MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif ()

################################################################################
# Global linker options
################################################################################
if (MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif ()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
            "Package \"${PACKAGE}\" with version \"${VERSION}\" "
            "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################


set(PROJECT_NAME u6o7)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "../data_both.h"
        "../data_client.h"
        "../data_host.h"
        "../define_both.h"
        "../define_client.h"
        "../define_host.h"
        "../function_both.h"
        "../function_client.h"
        "../function_host.h"
        "../host_setup.h"
        "../random/rand.h"
        "../random/random.h"
        "../random/standard.h"
        "../Resource.h"
        "../spell_code/spell_code.h"
        "../stdafx.h"
        "../u6o7.h"
        "../use_code/use_code.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
        "../client_bmp.inc"
        "../globals.inc"
        "../host.inc"
        "../new_getspr.inc"
        "../old_getspr.inc"
        "../secret_a.inc"
        "../secret_b.inc"
        "../setup_both.inc"
        "../setup_client.inc"
        "../setup_host.inc"
        "../small.ico"
        "../u6o7.ico"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
        "../data_both.cpp"
        "../data_client.cpp"
        "../data_host.cpp"
        "../dmusic.cpp"
        "../frame.cpp"
        "../function_both.cpp"
        "../function_client.cpp"
        "../function_host.cpp"
        "../house.cpp"
        "../inline_asm/fast.asm"
        "../inline_asm/fast2hi.asm"
        "../inline_asm/fast3.asm"
        "../inline_asm/fast4.asm"
        "../inline_asm/fast4hi.asm"
        "../inline_asm/fast5.asm"
        "../inline_asm/fasthi.asm"
        "../myddraw.cpp"
        "../myfile.cpp"
        "../random/randi.cpp"
        "../random/random.cpp"
        "../sound.cpp"
        "../spell_code/spell_code.cpp"
        "../stdafx.cpp"
        "../txt.cpp"
        "../u6o7.cpp"
        "../use_code/use_code.cpp"
        "../Xenkan.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
        ${Header_Files}
        ${Resource_Files}
        ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE u6o7)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_NAME_DEBUG "Ultima VI Online Full"
        PDB_OUTPUT_DIRECTORY_DEBUG "../build/"
        RUNTIME_OUTPUT_NAME_RELEASE "Ultima VI Online Full"
        PDB_OUTPUT_DIRECTORY_RELEASE "../build/"
        OUTPUT_DIRECTORY_DEBUG "../../bin/client/debug/"
        OUTPUT_DIRECTORY_RELEASE "../../bin/client/release/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
        MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
        MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "HOST;"
        "CLIENT"
)

################################################################################
# Compile and link options
################################################################################
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Gm;
            /Od;
            /RTC1;
            /W1;
            /ZI
            >
            $<$<CONFIG:Release>:
            /GL-;
            /Ob2;
            /Oi;
            /Oy-;
            /W3;
            /Zi;
            /Ot
            >
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
            >
            /DEBUG;
            /MACHINE:X86;
            /SUBSYSTEM:WINDOWS;
            /INCREMENTAL:NO
    )
endif ()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "dplay;"
        "mylib;"
        "dxguid;"
        "wininet;"
        "fdv_dmvr;"
        "ws2_32;"
        "winmm;"
        "user32;"
        "kernel32;"
        "gdi32;"
        "winspool;"
        "comdlg32;"
        "advapi32;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "odbc32;"
        "odbccp32;"
        "vfw32;"
        "dplayx;"
        "ddraw;"
        "dsound"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)


set(PROJECT_NAME u6oh)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "../data_both.h"
        "../data_client.h"
        "../data_host.h"
        "../define_both.h"
        "../define_client.h"
        "../define_host.h"
        "../function_both.h"
        "../function_host.h"
        "../host_setup.h"
        "../random/rand.h"
        "../random/random.h"
        "../random/standard.h"
        "../Resource.h"
        "../spell_code/spell_code.h"
        "../stdafx.h"
        "../u6o7.h"
        "../use_code/use_code.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
        "../globals.inc"
        "../host.inc"
        "../new_getspr.inc"
        "../old_getspr.inc"
        "../secret_a.inc"
        "../secret_b.inc"
        "../setup_both.inc"
        "../setup_client.inc"
        "../setup_host.inc"
        "../small.ico"
        "../u6o7.ico"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
        "../data_both.cpp"
        "../data_host.cpp"
        "../dmusic.cpp"
        "../frame.cpp"
        "../function_both.cpp"
        "../function_host.cpp"
        "../host_setup.cpp"
        "../house.cpp"
        "../myddraw.cpp"
        "../myfile.cpp"
        "../random/randi.cpp"
        "../random/random.cpp"
        "../spell_code/spell_code.cpp"
        "../stdafx.cpp"
        "../txt.cpp"
        "../u6o7.cpp"
        "../use_code/use_code.cpp"
        "../Xenkan.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
        ${Header_Files}
        ${Resource_Files}
        ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable("${PROJECT_NAME}" ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE u6o7)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_NAME_DEBUG "Ultima VI Online Host"
        PDB_OUTPUT_DIRECTORY_DEBUG "../build/"
        RUNTIME_OUTPUT_NAME_RELEASE "Ultima VI Online Host"
        PDB_OUTPUT_DIRECTORY_RELEASE "../build/"
        OUTPUT_DIRECTORY_DEBUG "../../bin/host/debug/"
        OUTPUT_DIRECTORY_RELEASE "../../bin/host/release/"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
        MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
        MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "HOST;"
        "CONSOLE"
)

################################################################################
# Compile and link options
################################################################################
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Gm;
            /Od;
            /RTC1;
            /W1;
            /ZI
            >
            $<$<CONFIG:Release>:
            /GL-;
            /Ob2;
            /Oi;
            /Oy-;
            /W3;
            /Zi;
            /Ot
            >
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /ASSEMBLYDEBUG
            >
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
            >
            /DEBUG;
            /MACHINE:X86;
            /SUBSYSTEM:WINDOWS;
            /INCREMENTAL:NO
    )
endif ()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "dplay;"
        "mylib;"
        "dxguid;"
        "wininet;"
        "fdv_dmvr;"
        "ws2_32;"
        "winmm;"
        "user32;"
        "kernel32;"
        "gdi32;"
        "winspool;"
        "comdlg32;"
        "advapi32;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "odbc32;"
        "odbccp32;"
        "vfw32;"
        "dplayx;"
        "ddraw;"
        "dsound"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)


set(PROJECT_NAME u6oclient2)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "../data_both.h"
        "../data_client.h"
        "../define_both.h"
        "../define_client.h"
        "../function_both.h"
        "../function_client.h"
        "../random/rand.h"
        "../random/random.h"
        "../random/standard.h"
        "../Resource.h"
        "../setup_both_basic.h"
        "../spell_code/spell_code.h"
        "../stdafx.h"
        "../u6o7.h"
        "../use_code/use_code.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
        "../client_bmp.inc"
        "../globals.inc"
        "../new_getspr.inc"
        "../old_getspr.inc"
        "../secret_a.inc"
        "../secret_b.inc"
        "../setup_both.inc"
        "../setup_client.inc"
        "../small.ico"
        "../u6o7.ico"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
        "../data_both.cpp"
        "../data_client.cpp"
        "../dmusic.cpp"
        "../frame.cpp"
        "../function_both.cpp"
        "../function_client.cpp"
        "../myddraw.cpp"
        "../myfile.cpp"
        "../random/randi.cpp"
        "../random/random.cpp"
        "../sound.cpp"
        "../stdafx.cpp"
        "../txt.cpp"
        "../u6o7.cpp"
        "../Xenkan.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
        ${Header_Files}
        ${Resource_Files}
        ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE u6o7)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_NAME_DEBUG "Ultima VI Online"
        PDB_OUTPUT_DIRECTORY_DEBUG "../build/"
        RUNTIME_OUTPUT_NAME_RELEASE "Ultima VI Online"
        PDB_OUTPUT_DIRECTORY_RELEASE "../build/"
        OUTPUT_DIRECTORY_DEBUG "../../bin/client/debug/"
        OUTPUT_DIRECTORY_RELEASE "../../bin/client/release/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "/MT"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "CLIENT"
)

################################################################################
# Compile and link options
################################################################################
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Gm;
            /Od;
            /RTC1;
            /W1;
            /ZI
            >
            $<$<CONFIG:Release>:
            /GL-;
            /Ob2;
            /Oi;
            /Oy-;
            /W3;
            /Zi;
            /Ot
            >
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
            >
            /DEBUG;
            /MACHINE:X86;
            /SUBSYSTEM:WINDOWS;
            /INCREMENTAL:NO
    )
endif ()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "dplay;"
        "mylib;"
        "dxguid;"
        "wininet;"
        "fdv_dmvr;"
        "ws2_32;"
        "winmm;"
        "user32;"
        "kernel32;"
        "gdi32;"
        "winspool;"
        "comdlg32;"
        "advapi32;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "odbc32;"
        "odbccp32;"
        "vfw32;"
        "dplayx;"
        "ddraw;"
        "dsound"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)

