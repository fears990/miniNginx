cmake_minimum_required(VERSION 3.31)

project(miniNginx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(miniNginx
    main.cpp

    source/lru_cache.cpp
    source/thread_pool.cpp
    source/blocking_safe_stack.cpp
    source/thread_safe_stack.cpp
    source/thread_safe_queue.cpp
    source/blocking_thread_logger.cpp
)

include(GNUInstallDirs)
install(TARGETS miniNginx
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
