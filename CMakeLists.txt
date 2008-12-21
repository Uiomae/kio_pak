cmake_minimum_required(VERSION 2.6)

SET(PAK_INSTALL_PREFIX "/usr" CACHE PATH "Prefix preppended to install directories")
SET(CMAKE_INSTALL_PREFIX "${PAK_INSTALL_PREFIX}" CACHE INTERNAL "Prefix preppended to install directories" FORCE)

PROJECT(SingStarPakKioSlave)
FIND_PACKAGE(KDE3 REQUIRED)
ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE3_DEFINITIONS})
LINK_DIRECTORIES(${KDE3_LIB_DIR})
INCLUDE_DIRECTORIES( ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

set(kio_pak_SRCS pak.cpp kPak.cpp)

KDE3_AUTOMOC(${kio_pak_SRCS})
KDE3_ADD_KPART(kio_pak ${kio_pak_SRCS})
target_link_libraries(kio_pak ${QT_AND_KDECORE_LIBS})
install_targets(/lib/kde3 kio_pak)

## install files ##

install_FILES(/share/services FILES pak.protocol)

# For uninstall target
CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

