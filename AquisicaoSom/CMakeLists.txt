cmake_minimum_required(VERSION 2.8)

project(gravacao)


find_package(qibuild)
find_package(ALSA)

INCLUDE_DIRECTORIES(${ALSA_INCLUDE_DIRS})

qi_create_bin(gravacao gravacao.c)
qi_use_lib(gravacao ALSA)

TARGET_LINK_LIBRARIES(gravacao ${ALSA_LIBRARIES})
