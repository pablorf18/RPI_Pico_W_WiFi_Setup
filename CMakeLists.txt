cmake_minimum_required(VERSION 3.13)

set(PROGRAM_NAME pico_wifi_ap_example)

#set the script to resolve fsdata
set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/external/makefsdata)

#if not exist, download from github. NOTE: needed internet connection
if (NOT EXISTS ${MAKE_FS_DATA_SCRIPT})
        file(DOWNLOAD
                https://raw.githubusercontent.com/lwip-tcpip/lwip/e799c266facc3c70190676eccad49d6c2db2caac/src/apps/http/makefsdata/makefsdata
                ${MAKE_FS_DATA_SCRIPT}
                )
endif()
message("Running makefsdata script")

#run the script to create the fsdata file
execute_process(COMMAND
        perl ${MAKE_FS_DATA_SCRIPT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
        )

set(CMAKE_CXX_STANDARD 17)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(${PROGRAM_NAME}) 

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(WifiScanner)
add_subdirectory(WebHandlers)

add_executable(${PROGRAM_NAME}
    main.cpp
)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROGRAM_NAME} 0)
pico_enable_stdio_usb(${PROGRAM_NAME} 1)

#set the RPI Pico SSID and password for AP mode
set(WIFI_SSID "Pico_W" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "12345678" CACHE INTERNAL "WiFi password")

target_compile_definitions(${PROGRAM_NAME} PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )

target_include_directories(${PROGRAM_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/common
)

target_link_libraries(${PROGRAM_NAME}
    wifi_scanner
    web_handlers
    pico_stdlib
    pico_cyw43_arch_lwip_poll
    pico_lwip_http
)

pico_add_extra_outputs(${PROGRAM_NAME})