add_library(wifi_scanner STATIC
    WiFiScanner.cpp
    NetworkInfo.h
    WiFiScanner.h
)

target_include_directories(wifi_scanner PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../common
)

target_link_libraries(wifi_scanner
    pico_stdlib
    pico_cyw43_arch_lwip_poll
)