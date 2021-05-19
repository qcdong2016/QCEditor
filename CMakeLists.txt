#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME QCEditor)

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)


find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Xml Multimedia REQUIRED)


# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
#___AUTO_START___
set(GAME_SOURCE
	Classes/BoxList.cpp
	Classes/BoxList.h
	Classes/BoxList.ui
	Classes/Editor/Common.cpp
	Classes/Editor/Common.h
	Classes/Editor/EditorNode.cpp
	Classes/Editor/EditorNode.h
	Classes/Editor/FilePathManager.cpp
	Classes/Editor/FilePathManager.h
	Classes/Editor/PropertyDef.cpp
	Classes/Editor/PropertyDef.h
	Classes/Editor/QC_Application.cpp
	Classes/Editor/QC_Application.h
	Classes/Editor/QC_GLWidget.cpp
	Classes/Editor/QC_GLWidget.h
	Classes/Editor/SceneCtrl.cpp
	Classes/Editor/SceneCtrl.h
	Classes/Editor/WindowBox.cpp
	Classes/Editor/WindowBox.h
	Classes/EditorMain.cpp
	Classes/EditorMain.h
	Classes/EditorMain.qrc
	Classes/EditorMain.ui
	Classes/libqc/Any.cpp
	Classes/libqc/Any.h
	Classes/libqc/AttributeAccessor.cpp
	Classes/libqc/AttributeAccessor.h
	Classes/libqc/DefaultValue.cpp
	Classes/libqc/DefaultValue.h
	Classes/libqc/Serializer.cpp
	Classes/libqc/Serializer.h
	Classes/libqc/StringUtil.h
	Classes/libqc/Varinat.cpp
	Classes/libqc/Varinat.h
	Classes/libqc/rapidxml-1.13/rapidxml.hpp
	Classes/libqc/rapidxml-1.13/rapidxml_iterators.hpp
	Classes/libqc/rapidxml-1.13/rapidxml_print.hpp
	Classes/libqc/rapidxml-1.13/rapidxml_utils.hpp
	Classes/main.cpp

Classes/qtpropertybrowser/src/qtbuttonpropertybrowser.cpp
Classes/qtpropertybrowser/src/qteditorfactory.cpp
Classes/qtpropertybrowser/src/qtgroupboxpropertybrowser.cpp
Classes/qtpropertybrowser/src/qtpropertybrowser.cpp
Classes/qtpropertybrowser/src/qtpropertybrowserutils.cpp
Classes/qtpropertybrowser/src/qtpropertymanager.cpp
Classes/qtpropertybrowser/src/qttreepropertybrowser.cpp
Classes/qtpropertybrowser/src/qtvariantproperty.cpp
Classes/qtpropertybrowser/src/qtpropertybrowser.qrc
)
#___AUTO_END___

if(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    set(APP_UI_RES
        proj.ios_mac/mac/Icon.icns
        proj.ios_mac/mac/Info.plist
        )
    list(APPEND GAME_SOURCE
        #  proj.ios_mac/mac/main.cpp
            proj.ios_mac/mac/Prefix.pch
            ${APP_UI_RES}
            )
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# 设置MSVC编译编码
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

add_executable(${APP_NAME} ${all_code_files})

set(QT_LIBS
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Multimedia
)

if(WINDOWS)
    list(APPEND QT_LIBS Qt5::WinMain)
endif()

target_compile_definitions(${APP_NAME} PRIVATE QC_EDITOR)
target_link_libraries(${APP_NAME} cocos2d  ${QT_LIBS})
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE Classes/libqc
        PRIVATE Classes/qtpropertybrowser/src
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
    )
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
