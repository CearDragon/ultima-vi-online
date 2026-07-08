cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

# Win32 resource targeting only makes sense for the MSVC/Windows build.
if (CMAKE_HOST_WIN32)
    set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)
endif ()

project(u6o7 CXX)

# ============================================================================
# LH-P6: Headless Linux host build (GCC/Clang, i386).
#
# When configured with a non-Windows toolchain, build ONLY the `host` binary,
# OS-independent, for running in a Linux container. The entire Windows/MSVC
# path below (icon pipeline, RC compiler, the `both`/`client`/`host` Win32
# targets, DirectX link libs) is skipped via the early return().
#
# Key choices (see docs/plans/plan-linuxHost.md):
#   * -m32 : preserve the 32-bit data model so .sav/wire layout and the
#            `txt`/struct sizes match the Windows client byte-for-byte.
#   * -include platform.h : force the platform shim into every TU so the Win32
#            type/socket/thread/time/gui aliases are always defined, regardless
#            of per-header include order.
#   * DirectX/UI TUs (myddraw/frame/dmusic/sound) and stdafx.cpp are NOT
#            compiled; platform/plat_stubs.cpp supplies the no-op entry points.
#   * -rdynamic : symbol names for the POSIX crash backtrace().
#
# Prerequisites on the build host (Debian/Ubuntu):
#   sudo apt-get install g++-multilib   # 32-bit libstdc++/libc + libpthread
#
# Build:
#   cmake --preset linux-host           # or: cmake -S . -B build-linux -G Ninja
#   cmake --build build-linux           #     -DCMAKE_CXX_FLAGS=-m32 ...
# ============================================================================
if (NOT WIN32)
    message(STATUS "u6o7: configuring headless Linux host (i386)")

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_executable(host
            src/common/u6o7.cpp                 # #includes loop_host.cpp, setup_host.inc, host.inc, ...
            src/common/data_both.cpp
            src/server/data_host.cpp
            src/common/function_both.cpp
            src/server/function_host.cpp
            src/common/house.cpp
            src/common/myfile.cpp
            src/common/random/randi.cpp
            src/common/random/random.cpp
            src/common/spell_code/spell_code.cpp
            src/common/txt.cpp
            src/common/use_code/use_code.cpp
            src/common/Xenkan.cpp
            src/common/platform/plat_stubs.cpp  # no-op frame_init()/setupddraw()
    )

    target_include_directories(host PRIVATE
            ${CMAKE_SOURCE_DIR}/src/common
            ${CMAKE_SOURCE_DIR}/src/common/include
            ${CMAKE_SOURCE_DIR}/src/client
            ${CMAKE_SOURCE_DIR}/src/server
    )

    # HOST selects the server build variant. CONSOLE is intentionally NOT
    # defined (it pulls Win32 _cprintf/<conio.h>); see the stdout-logging
    # follow-up in the plan. _WIN32 is naturally absent on this toolchain.
    target_compile_definitions(host PRIVATE HOST)

    target_compile_options(host PRIVATE
            -m32
            # Keep "-include <path>" as a single shell token so CMake never
            # splits/reorders the flag away from its argument.
            "SHELL:-include ${CMAKE_SOURCE_DIR}/src/common/platform/platform.h"
            -fpermissive   # legacy C-with-extensions leans on lax conversions
            -w             # silence the legacy warning flood (matches /W1 intent) for now
    )

    target_link_options(host PRIVATE -m32 -rdynamic)
    target_link_libraries(host PRIVATE pthread m)

    set_target_properties(host PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/host/linux
            OUTPUT_NAME u6o-host
    )

    return()
endif ()

# Enable the Win32 resource compiler so target-attached .rc files are
# built and linked into the EXE (icons, version info, dialogs, etc.).
# Done after project() so MSVC toolchain detection has already run.
enable_language(RC)

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
# Icon pipeline (PNG -> multi-resolution ICO -> .rc -> .res -> EXE)
#
# Drop a new square PNG at assets/images/icon.png and rebuild any target;
# the .ico files are regenerated by tools/build_icon.ps1 and the .rc is
# re-emitted by configure_file(). Nothing else in source needs to change.
################################################################################
set(ICON_SOURCE_PNG     "${CMAKE_SOURCE_DIR}/assets/images/icon.png")
set(ICON_BUILDER_SCRIPT "${CMAKE_SOURCE_DIR}/tools/build_icon.ps1")
set(ICON_OUTPUT_DIR     "${CMAKE_BINARY_DIR}/generated/icons")
set(GENERATED_ICON_U6O7  "${ICON_OUTPUT_DIR}/u6o7.ico")
set(GENERATED_ICON_SMALL "${ICON_OUTPUT_DIR}/small.ico")

# RC string literals are processed by the C preprocessor first, so we
# need backslash-escaped paths to survive the trip through the .rc file.
# configure_file does plain text substitution; produce a version that's
# safe to drop inside a "..." string in the .rc.
string(REPLACE "/" "\\\\" GENERATED_ICON_U6O7_RC  "${GENERATED_ICON_U6O7}")
string(REPLACE "/" "\\\\" GENERATED_ICON_SMALL_RC "${GENERATED_ICON_SMALL}")

file(MAKE_DIRECTORY "${ICON_OUTPUT_DIR}")

# PowerShell's [string[]] command-line binder wants the array as a
# single comma-joined argument, not as N positional args, so build a
# combined "<path1>,<path2>,..." string here.
set(_ICON_OUTPUTS_CSV "${GENERATED_ICON_U6O7},${GENERATED_ICON_SMALL}")

add_custom_command(
    OUTPUT  "${GENERATED_ICON_U6O7}" "${GENERATED_ICON_SMALL}"
    DEPENDS "${ICON_SOURCE_PNG}" "${ICON_BUILDER_SCRIPT}"
    COMMAND powershell.exe -NoProfile -ExecutionPolicy Bypass
            -File "${ICON_BUILDER_SCRIPT}"
            -SourcePng "${ICON_SOURCE_PNG}"
            -OutputIco "${_ICON_OUTPUTS_CSV}"
    COMMENT "Rasterizing icon.png -> u6o7.ico, small.ico"
    VERBATIM
)

# Materialize the .rc from its template, baking in the absolute paths
# of the just-generated .ico files. @ONLY substitution restricts the
# template syntax to @VAR@ (so the .rc's #define / #include lines pass
# through untouched).
set(GENERATED_RC "${CMAKE_BINARY_DIR}/generated/u6o7.rc")
configure_file(
    "${CMAKE_SOURCE_DIR}/src/common/u6o7.rc.in"
    "${GENERATED_RC}"
    @ONLY
)

# Custom target so individual EXE targets can depend on "the icons are
# built" without each declaring the OUTPUT files directly.
add_custom_target(generate_icons
    DEPENDS "${GENERATED_ICON_U6O7}" "${GENERATED_ICON_SMALL}"
)

# Helper: attach the generated .rc to a target, ensure RC can find
# Resource.h, and make the target wait on icon rasterization.
function(u6o_attach_icons TARGET)
    target_sources(${TARGET} PRIVATE "${GENERATED_RC}")
    # rc.exe needs an /I to resolve `#include "Resource.h"` from the .rc.
    set_source_files_properties("${GENERATED_RC}" PROPERTIES
        COMPILE_OPTIONS "/I${CMAKE_SOURCE_DIR}/src/client"
    )
    add_dependencies(${TARGET} generate_icons)
endfunction()

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


set(PROJECT_NAME both)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "src/common/data_both.h"
        "src/client/data_client.h"
        "src/server/data_host.h"
        "src/common/define_both.h"
        "src/client/define_client.h"
        "src/server/define_host.h"
        "src/common/function_both.h"
        "src/client/function_client.h"
        "src/server/function_host.h"
        "src/server/host_setup.h"
        "src/common/random/rand.h"
        "src/common/random/random.h"
        "src/common/random/standard.h"
        "src/client/Resource.h"
        "src/common/spell_code/spell_code.h"
        "src/common/stdafx.h"
        "src/common/u6o7.h"
        "src/common/use_code/use_code.h"
        "src/client/viewport.h"
        "src/client/ui_layout.h"
        "src/client/splash.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
        "src/client/client_bmp.inc"
        "src/common/globals.inc"
        "src/server/host.inc"
        "src/client/new_getspr.inc"
        "src/common/secret_a.inc"
        "src/common/secret_b.inc"
        "src/common/setup_both.inc"
        "src/client/setup_client.inc"
        "src/server/setup_host.inc"
        "src/common/u6o7.rc.in"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
        "src/common/data_both.cpp"
        "src/client/data_client.cpp"
        "src/server/data_host.cpp"
        "src/client/dmusic.cpp"
        "src/client/frame.cpp"
        "src/common/function_both.cpp"
        "src/client/function_client.cpp"
        "src/server/function_host.cpp"
        "src/common/house.cpp"
        "src/common/inline_asm/fast.asm"
        "src/common/inline_asm/fast2hi.asm"
        "src/common/inline_asm/fast3.asm"
        "src/common/inline_asm/fast4.asm"
        "src/common/inline_asm/fast4hi.asm"
        "src/common/inline_asm/fast5.asm"
        "src/common/inline_asm/fasthi.asm"
        "src/client/myddraw.cpp"
        "src/client/present.cpp"
        "src/common/myfile.cpp"
        "src/common/random/randi.cpp"
        "src/common/random/random.cpp"
        "src/client/sound.cpp"
        "src/common/spell_code/spell_code.cpp"
        "src/common/stdafx.cpp"
        "src/common/txt.cpp"
        "src/client/viewport.cpp"
        "src/client/ui_layout.cpp"
        "src/client/ui_panels_apply.cpp"
        "src/client/splash.cpp"
        "src/common/u6o7.cpp"
        "src/common/use_code/use_code.cpp"
        "src/common/Xenkan.cpp"
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
u6o_attach_icons(${PROJECT_NAME})

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
        PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/build/"
        RUNTIME_OUTPUT_NAME_RELEASE "Ultima VI Online Full"
        PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/build/"
        OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/client/debug/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/client/release/"
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
        MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/client"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server"
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

# Dependencies (for both target)
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
        "dsound;"
        "gdiplus"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/lib"
)


set(PROJECT_NAME host)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "src/common/data_both.h"
        "src/client/data_client.h"
        "src/server/data_host.h"
        "src/common/define_both.h"
        "src/client/define_client.h"
        "src/server/define_host.h"
        "src/common/function_both.h"
        "src/server/function_host.h"
        "src/common/random/rand.h"
        "src/common/random/random.h"
        "src/common/random/standard.h"
        "src/client/Resource.h"
        "src/common/spell_code/spell_code.h"
        "src/common/stdafx.h"
        "src/common/u6o7.h"
        "src/common/use_code/use_code.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
        "src/common/globals.inc"
        "src/server/host.inc"
        "src/client/new_getspr.inc"
        "src/common/secret_a.inc"
        "src/common/secret_b.inc"
        "src/common/setup_both.inc"
        "src/client/setup_client.inc"
        "src/server/setup_host.inc"
        "src/common/u6o7.rc.in"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
        "src/common/data_both.cpp"
        "src/server/data_host.cpp"
        "src/client/dmusic.cpp"
        "src/client/frame.cpp"
        "src/common/function_both.cpp"
        "src/server/function_host.cpp"
        "src/common/house.cpp"
        "src/client/myddraw.cpp"
        "src/common/myfile.cpp"
        "src/common/random/randi.cpp"
        "src/common/random/random.cpp"
        "src/common/spell_code/spell_code.cpp"
        "src/common/stdafx.cpp"
        "src/common/txt.cpp"
        "src/common/u6o7.cpp"
        "src/common/use_code/use_code.cpp"
        "src/common/Xenkan.cpp"
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
u6o_attach_icons(${PROJECT_NAME})

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
        PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/build/"
        RUNTIME_OUTPUT_NAME_RELEASE "Ultima VI Online Host"
        PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/build/"
        OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/host/debug/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/host/release/"
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
        MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/client"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server"
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
        "dsound;"
        "gdiplus"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/lib"
)


set(PROJECT_NAME client)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "src/common/data_both.h"
        "src/client/data_client.h"
        "src/common/define_both.h"
        "src/client/define_client.h"
        "src/common/function_both.h"
        "src/client/function_client.h"
        "src/common/random/rand.h"
        "src/common/random/random.h"
        "src/common/random/standard.h"
        "src/client/Resource.h"
        "src/common/setup_both_basic.h"
        "src/common/spell_code/spell_code.h"
        "src/common/stdafx.h"
        "src/common/u6o7.h"
        "src/common/use_code/use_code.h"
        "src/client/viewport.h"
        "src/client/ui_layout.h"
        "src/client/splash.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
        "src/client/client_bmp.inc"
        "src/common/globals.inc"
        "src/client/new_getspr.inc"
        "src/common/secret_a.inc"
        "src/common/secret_b.inc"
        "src/common/setup_both.inc"
        "src/client/setup_client.inc"
        "src/common/u6o7.rc.in"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
        "src/common/data_both.cpp"
        "src/client/data_client.cpp"
        "src/client/dmusic.cpp"
        "src/client/frame.cpp"
        "src/common/function_both.cpp"
        "src/client/function_client.cpp"
        "src/client/myddraw.cpp"
        "src/client/present.cpp"
        "src/common/myfile.cpp"
        "src/common/random/randi.cpp"
        "src/common/random/random.cpp"
        "src/client/sound.cpp"
        "src/common/stdafx.cpp"
        "src/common/txt.cpp"
        "src/common/u6o7.cpp"
        "src/common/Xenkan.cpp"
        "src/client/viewport.cpp"
        "src/client/ui_layout.cpp"
        "src/client/ui_panels_apply.cpp"
        "src/client/splash.cpp"
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
u6o_attach_icons(${PROJECT_NAME})

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
        PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/build/"
        RUNTIME_OUTPUT_NAME_RELEASE "Ultima VI Online"
        PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/build/"
        OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/client/debug/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/client/release/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
#
# Statically link the C/C++ runtime (/MTd debug, /MT release) so the shipped
# client EXE has no dependency on the redistributable CRT DLLs
# (ucrtbase.dll, vcruntime140.dll, msvcp140.dll). Users have always run the
# game from the single EXE with no installer, so keep it self-contained.
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
        MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
        MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/client"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server"
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
        "CLIENT"
)

################################################################################
# Compile and link options
################################################################################
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
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
        "dsound"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/lib"
)






