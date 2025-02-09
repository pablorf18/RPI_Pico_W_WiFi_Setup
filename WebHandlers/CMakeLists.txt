add_library(web_handlers STATIC
    CGI.cpp
    CGI.h
    SSI.cpp
    SSI.h
)

target_include_directories(web_handlers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(web_handlers
    wifi_scanner
    pico_stdlib
    pico_cyw43_arch_lwip_poll
    pico_lwip_http
)