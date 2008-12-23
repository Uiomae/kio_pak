#	kio_pak: A KDE3 kioslave to explore and modify SingStar .pak files
#	Copyright (C) 2008  Uiomae <uiomae@gmail.com>
#
#	This file is part of kio_pak.
#
#	kio_pak is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	kio_pak is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with kio_pak.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.6)

SET(PAK_INSTALL_PREFIX "/usr" CACHE PATH "Prefix preppended to install directories")
SET(CMAKE_INSTALL_PREFIX "${PAK_INSTALL_PREFIX}" CACHE INTERNAL "Prefix preppended to install directories" FORCE)

PROJECT(SingStarPakKioSlave)
FIND_PACKAGE(KDE3 REQUIRED)
ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE3_DEFINITIONS})
LINK_DIRECTORIES(${KDE3_LIB_DIR})
INCLUDE_DIRECTORIES( ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

set(kio_pak_SRCS pak.cpp kPak.cpp libpak.cpp)

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

