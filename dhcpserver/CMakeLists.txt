add_library(dhcp_server STATIC
    dhcpserver.c
    dhcpserver.h
)

target_include_directories(dhcp_server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../common
)

target_link_libraries(dhcp_server
    pico_stdlib
    pico_cyw43_arch_lwip_poll
)