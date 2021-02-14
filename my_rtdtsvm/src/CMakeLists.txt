# Project source & header files
file(GLOB SOURCES
  "*.h"
  "*.cpp"
  "*.cc"
)

set(INCLUDES ${PCAP_INCLUDE_DIR})
set(LIBS ${PCAP_LIBRARIES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


if(UNIX)
  # gcc warning: scoped enums only available with -std=c++0x or -std=gnu++0
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

# ntohs() function in Windows
if( WIN32 )
  set( LIBS ${LIBS} "Ws2_32")
endif()

include_directories(${INCLUDES})
add_executable(my_rtdtsvm ${SOURCES})
target_link_libraries(my_rtdtsvm PRIVATE Threads::Threads ${LIBS} )
