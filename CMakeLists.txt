#### CMake 版本
cmake_minimum_required(VERSION 3.14)

### 用于生成 .lib 
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

##  判断是否为debug模式
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug Mode")
    set(DEBUG_MODE ON)
else()
    message(STATUS "Release Mode")
    set(DEBUG_MODE OFF)
endif()

### 是否生成 exe
set(BUILD_EXE false)

#set(CMAKE_BUILD_TYPE Debug)

############################################ 参数设置区 ########################################
#### 当前项目的版本号
set(PROJECT_VERSION 0.1)


###################### 必改项： 库名称 库名称宏 ##############
#### 配置修改 项目/库名称 

set(THISPROJECTNAME TCPROBOT)

#### 库名称宏 命名取决于 文件 xxx.global, 用于区分不同库的导出和导入
#if defined(TEST11_LIBRARY)
#define TEST11_EXPORT Q_DECL_EXPORT
#else
#define TEST11_EXPORT Q_DECL_IMPORT
#endif

#### 库名称宏
if(BUILD_EXE)
    set(THIS_LIB_NAME TCPROBOT_EXE)
else()
    set(THIS_LIB_NAME TCPROBOT_LIBRARY)
endif()

###################### 必改项： 文件输出路径  ##############
#### 库文件输出路径
# 设置输出路径
# 根据操作系统类型设置库和运行时输出路径
if (UNIX)
    ##  linux 库文件输出路径
    set(LINUX_LIBRARY_OUTPUT_PATH_ROOT "/home/user01/TcpRobot")

    ## 当前工程所在文件夹，默认为当前CMakeLiat 所在目录
    set(PROJECT_CURRENT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    ## 
else (WIN32)
    ## 设置Windows库的输出路径和包含路径
    set(WIN_LIBRARY_OUTPUT_PATH__ROOT "D:/Soft/AtamiLib/TcpRobot")

     ## 当前工程所在文件夹，默认为当前CMakeLiat 所在目录
    set(PROJECT_CURRENT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    set(PROJECT_CURRENT_PATH_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
## 路径显示
message("WIN_PROJECT_CURRENT_PATH : " ${PROJECT_CURRENT_PATH})


###################### 必改项： 外部库导入  ##################

##### 是否引用 QT 
set(QT_OPEN true)
##### QT 版本 5
set(QT_VERSION_5 true)

#### QT 版本 6
set(QT_VERSION_6 false)

#### QT 核心模块
set(THIS_QT_CORE 
    Core 
    Widgets 
    Network 
    Gui 
    Svg
    Sql
    Charts
    OpenGL
    Concurrent
    3DRender
    3DExtras
)

#### QT5 核心库
if(QT_VERSION_5)
    set(THIS_QT_CORE_LIB 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Network 
        Qt5::Gui 
        Qt5::Svg 
        Qt5::Sql 
        Qt5::Charts 
        Qt5::OpenGL 
        Qt5::Concurrent 
        Qt5::3DRender 
        Qt5::3DExtras
    )
endif()

#### QT6 核心库
if(QT_VERSION_6)
    set(THIS_QT_CORE_LIB Qt6::Core)
endif()


#### 其余外部库

if(WIN32)
    if(DEBUG_MODE)
        ## 设置是否加载外部库 若为空，设置为 false
        set(ADD_OTHER true)
         ### 其他库
         ### 头文件
        set(ADD_OTHER_HEADER 
             ## 头文件目录
             "D:/Soft/AtamiLib/CommunicationModule/include/"
             "D:/Soft/AtamiLib/TcpXViewModule/include/"
             "D:/Soft/OCC3/OCCT/Debug/include/inc/"
             "D:/Soft/OCC3/products/freeimage-3.17.0-vc14-64/include/"
             "D:/Soft/OCC3/products/freetype-2.5.5-vc14-64/include/"
             "D:/Soft/OCC3/products/tcltk-86-64/include/"
             "D:/Soft/eigen-user/"
             "D:/Soft/AtamiLib/QCusWidgetLib/include/"
         )
        ### 库文件
        set(ADD_OTHER_LIB 
            ## 库文件 lib 目录
            "D:/Soft/AtamiLib/CommunicationModule/Win64/Lib/Debug/"
            "D:/Soft/AtamiLib/TcpXViewModule/Win64/Lib/Debug/"
            "D:/Soft/OCC3/OCCT/Debug/Win64/lib/"
            "D:/Soft/OCC3/products/freeimage-3.17.0-vc14-64/lib/"
            "D:/Soft/OCC3/products/freetype-2.5.5-vc14-64/lib/"
            "D:/Soft/OCC3/products/tcltk-86-64/lib/"
            "D:/Soft/AtamiLib/QCusWidgetLib/Win64/Lib/Debug/"
         )
         ### 库文件 dll
        set(ADD_OTHER_DLL 
             ## 库文件 dll 目录
             "D:/Soft/AtamiLib/CommunicationModule/Win64/Lib/Debug/"
             "D:/Soft/OCC3/OCCT/Debug/Win64/bind/"
             "D:/Soft/OCC3/products/freeimage-3.17.0-vc14-64/"
             "D:/Soft/OCC3/products/freetype-2.5.5-vc14-64/"
             "D:/Soft/OCC3/products/tcltk-86-64/"
             "D:/Soft/OCC3/products/TBB2021/"
             "D:/Soft/AtamiLib/QCusWidgetLib/Win64/Lib/Debug/"
        )
     else()
        ## 设置是否加载外部库 若为空，设置为 false
        set(ADD_OTHER true)
         ### 其他库
         ### 头文件
        set(ADD_OTHER_HEADER 
             ## 头文件目录
             "D:/Soft/AtamiLib/CommunicationModule/include"
         )
        ### 库文件
        set(ADD_OTHER_LIB 
            ## 库文件 lib 目录
            "D:/Soft/AtamiLib/CommunicationModule/Lib/Release/"

         )
         ### 库文件 dll
        set(ADD_OTHER_DLL 
             ## 库文件 dll 目录
             "D:/Soft/AtamiLib/CommunicationModule/Lib/Release/"
        )
     endif()
else(UNIX)
    set(ADD_OTHER false)
    ### 其他库
    ### 头文件
    set(ADD_OTHER_HEADER 
        ## 头文件目录
        #include/TcpXViewModule.h
    )
    ### 库文件
    set(ADD_OTHER_SO
        ## 库文件

    )
endif()


###################### 可选项： 指定模式  ##############
# 自动收集模式
set(AUTOFINDFILE true)

# 非自动收集时，需要手动指定
if(AUTOFINDFILE)
    #开启自动收集
else()
    set(HEADERFILE include/Test11_global.h include/Test11.h)
    set(SOURCEFILE source/Test11.cpp)
endif()

######################################## 参数检查区 #########################################
if(ADD_OTHER)
    foreach(HEADER ${ADD_OTHER_LIB} ${ADD_OTHER_HEADER} ${ADD_OTHER_DLL})
        if(NOT EXISTS ${HEADER})
            message(FATAL_ERROR "三方库路径不存在，请检查路径是否正确。")
        endif()
    endforeach()
endif()






######################################### 功能区 #################################################

#### 定义一个CMake项目，项目名称为Test11，项目版本为0.1，使用的编程语言为C++
project(${THIS_LIB_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_definitions(-DQSCINTILLA_DLL)
# 设置CMake自动处理Qt的UI、MOC和RCC文件
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置C++标准为C++17，并要求严格遵循标准，禁用编译器扩展
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(QT_OPEN)
    # 查找Qt软件包，首先查找Qt6，然后是Qt5，必须包含Core组件
    find_package(QT NAMES  Qt5 REQUIRED COMPONENTS ${THIS_QT_CORE})
    
    # 根据查找到的Qt版本查找对应版本的Core组件
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${THIS_QT_CORE})
endif()

# 获取 Qt 的架构信息
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(QT_BIT "64")
else()
    set(QT_BIT "32")
endif()

### 自动收集 .h .cpp
if(AUTOFINDFILE)
    file(GLOB AUTOHEADERSFILE RELATIVE ${PROJECT_CURRENT_PATH} "**/*.h")
    file(GLOB AUTOSOURCEFILE RELATIVE ${PROJECT_CURRENT_PATH} "**/*.cpp")
    set(HEADERFILE ${AUTOHEADERSFILE})
    set(SOURCEFILE ${AUTOSOURCEFILE}) 
endif()

message("HEADERFILE : " ${HEADERFILE})
set(PROJECT_SOURCES
   ${HEADERFILE}
   ${SOURCEFILE}
)
# 设置编译器路径
# 添加共享库 Test11，并指定源文件
add_library(${THISPROJECTNAME} SHARED ${PROJECT_SOURCES})
if(QT_OPEN)
        target_link_libraries(${THISPROJECTNAME} PRIVATE 
        ${THIS_QT_CORE_LIB}
        )
endif()
 # 链接其他库
 set(LIBS_FILES_LIST "")
 if(WIN32)
    foreach(var ${ADD_OTHER_LIB})
    file(GLOB_RECURSE LIBS_FILES "${var}*.lib")
    list(APPEND LIBS_FILES_LIST ${LIBS_FILES})
    endforeach()
else(UNIX)
    foreach(var ${ADD_OTHER_SO})
    file(GLOB_RECURSE LIBS_FILES "${var}*.so*")
    list(APPEND LIBS_FILES_LIST ${LIBS_FILES})
    endforeach()
endif()


if(ADD_OTHER)
    include_directories(${ADD_OTHER_HEADER})
    target_link_libraries(${THISPROJECTNAME} PRIVATE ${LIBS_FILES_LIST})
    message("添加其他三方库 : " ${LIBS_FILES_LIST})
endif()

target_compile_definitions(${THISPROJECTNAME} PRIVATE ${THIS_LIB_NAME})

# 设置输出属性

if (UNIX)
    ##  linux 库文件输出路径
    if(DEBUG_MODE)
        ## 调试模式
        set(LINUX_LIBRARY_OUTPUT_PATH ${LINUX_LIBRARY_OUTPUT_PATH_ROOT}/Lib/Debug)
    else()
        ## 发布模式
        set(LINUX_LIBRARY_OUTPUT_PATH ${LINUX_LIBRARY_OUTPUT_PATH_ROOT}/Lib/Release)
    endif()

    ##  linux .h 文件输出路径
    set(LINUX_LIBRARY_INCLUDE_PATH ${LINUX_LIBRARY_OUTPUT_PATH_ROOT}/Include)

    ## 运行目录输出
    set(LINUX_RUNTIME_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH_ROOT})
     ## 路径文件创建
    make_directory(${LINUX_LIBRARY_OUTPUT_PATH})
    make_directory(${LINUX_LIBRARY_INCLUDE_PATH})
    file(COPY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH})
    

else (WIN32)
    ## 设置Windows库的输出路径和包含路径

    if(DEBUG_MODE)
        ## 调试模式
        if(QT_BIT EQUAL 64)
            set(WIN_LIBRARY_OUTPUT_PATH ${WIN_LIBRARY_OUTPUT_PATH__ROOT}/Win64/Lib/Debug)
        else()
            set(WIN_LIBRARY_OUTPUT_PATH ${WIN_LIBRARY_OUTPUT_PATH__ROOT}/Win32/Lib/Debug)
        endif()
    else()
        ## 发布模式
        set(WIN_LIBRARY_OUTPUT_PATH ${WIN_LIBRARY_OUTPUT_PATH__ROOT}/Lib/Release)
    endif()

    ## 设置Windows库的包含路径
    set(WIN_LIBRARY_INCLUDE_PATH ${WIN_LIBRARY_OUTPUT_PATH__ROOT})
    ## 运行目录输出
    set(WIN_RUNTIME_DIRECTORY ${WIN_LIBRARY_OUTPUT_PATH__ROOT})
    ## 路径文件创建
    make_directory(${WIN_LIBRARY_OUTPUT_PATH})
    make_directory(${WIN_LIBRARY_INCLUDE_PATH})
endif()



# 路径文件创建
#make_directory(${WIN_LIBRARY_INCLUDE_PATH})

################################################## 安装 ##########################################
#“安装”通常指的是将编译或构建过程中生成的文件（如可执行文件、库文件、配置文件等）复制到特定的目录
################################################## 头文件 安装 ###################################
if(WIN32)
    install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${WIN_LIBRARY_INCLUDE_PATH})
else(UNIX)
    install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
endif()

################################################## 库文件 安装 ###################################
if (UNIX)
    if(DEBUG_MODE)
    # 设置输出属性
        set_target_properties(${THISPROJECTNAME} PROPERTIES
            OUTPUT_NAME "${THISPROJECTNAME}"
            SUFFIX "_debug.so"
            #LIBRARY_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
            #RUNTIME_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
            make_directory(${LINUX_LIBRARY_INCLUDE_PATH})
            install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH})
        )
        install(TARGETS ${THISPROJECTNAME}
            EXPORT ${THIS_LIB_NAME}
            ARCHIVE DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
            LIBRARY DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
            RUNTIME DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
            if(ADD_OTHER)
                install(DIRECTORY ${ADD_OTHER_HEADER} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH}/include)
                install(DIRECTORY ${ADD_OTHER_LIB} DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
                install(DIRECTORY ${ADD_OTHER_DLL} DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
            endif()

        
    else()
            # 设置输出属性
            set_target_properties(${THISPROJECTNAME} PROPERTIES
                OUTPUT_NAME "${THISPROJECTNAME}"
                SUFFIX "_release.so"
                LIBRARY_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
                RUNTIME_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
                make_directory(${LINUX_LIBRARY_INCLUDE_PATH})
                 install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH})
            )
            install(TARGETS ${THISPROJECTNAME}
                    EXPORT ${THIS_LIB_NAME}
                    ARCHIVE DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
                    LIBRARY DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
                    RUNTIME DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})

            ## 其他库
            if(ADD_OTHER)
                  install(DIRECTORY ${ADD_OTHER_HEADER} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH}/include)
                  install(DIRECTORY ${ADD_OTHER_LIB} DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
                  install(DIRECTORY ${ADD_OTHER_DLL} DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
             endif()
   endif()


else (WIN32)
    if(DEBUG_MODE)
        ### 调试模式 修改后缀
        set_target_properties(${THISPROJECTNAME} PROPERTIES OUTPUT_NAME "${THISPROJECTNAME}" SUFFIX "_debug.dll")
        
        # 安装到输出目录
        install(TARGETS ${THISPROJECTNAME}
                EXPORT ${THIS_LIB_NAME}
                ARCHIVE DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                LIBRARY DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                RUNTIME DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})

                if(ADD_OTHER)
                    #install(DIRECTORY ${ADD_OTHER_HEADER} DESTINATION ${WIN_LIBRARY_INCLUDE_PATH}/include)
                    #install(DIRECTORY ${ADD_OTHER_LIB} DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})
                   # install(DIRECTORY ${ADD_OTHER_DLL} DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})
                endif()

    else()

        ### 发布模式 修改后缀
        set_target_properties(${THISPROJECTNAME} PROPERTIES OUTPUT_NAME "${THISPROJECTNAME}" SUFFIX "_release.dll")
        
        # TARGETS   ${THISPROJECTNAME}： 指定要安装的目标文件，${THISPROJECTNAME}是目标的名称。
        # EXPORT    ${THIS_LIB_NAME}：   导出目标的安装信息，${THIS_LIB_NAME}是导出的库名称。
        # ARCHIVE   DESTINATION         ${WIN_LIBRARY_OUTPUT_PATH}：指定静态库文件的安装路径，${WIN_LIBRARY_OUTPUT_PATH}是路径变量。
        # LIBRARY   DESTINATION         ${WIN_LIBRARY_OUTPUT_PATH}：指定共享库文件的安装路径，${WIN_LIBRARY_OUTPUT_PATH}是路径变量。
        # RUNTIME   DESTINATION          ${WIN_LIBRARY_OUTPUT_PATH}：指定可执行文件的安装路径，${WIN_LIBRARY_OUTPUT_PATH}是路径变量。
        install(TARGETS ${THISPROJECTNAME}
                EXPORT ${THIS_LIB_NAME}
                ARCHIVE DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                LIBRARY DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                RUNTIME DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})

        if(ADD_OTHER)
            install(DIRECTORY ${ADD_OTHER_HEADER} DESTINATION ${WIN_LIBRARY_INCLUDE_PATH}/include)
            install(DIRECTORY ${ADD_OTHER_LIB} DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})
            install(DIRECTORY ${ADD_OTHER_DLL} DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})
        endif()

    # 结束
    endif()
endif()

# 设置宏定义
if(BUILD_SHARED_LIBS)
    target_compile_definitions(${THISPROJECTNAME} PUBLIC ${THIS_LIB_NAME})
endif()

if(BUILD_EXE)

cmake_minimum_required(VERSION 3.14)
### 修改编译模式时，需要清除缓存防止包含旧的lib库
cmake_policy(SET CMP0079 NEW)
#include(qt.cmake)
add_definitions(-DUNICODE -D_UNICODE)
## 设置项目名称
set(PROJECT_NAME AtamiXView)
## 设置项目版本号
set(PROJECT_VERSION 0.1)




## 设置 是否 在原有三方库创建的目录下创建子目录用于收集 dll文件
set(CREATE_THIRD_PARTY_LIB_DIR OFF)

##  判断是否为debug模式
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug Mode")

    ### 自定义安装路径
    set(CMAKE_INSTALL_PREFIX "D:/AppExe/AtamiXView/Debug/")
    set(DEBUG_MODE ON)
else()
    message(STATUS "Release Mode")

    ### 自定义安装路径
    set(CMAKE_INSTALL_PREFIX "D:/AppExe/AtamiXView/Release/")
    set(DEBUG_MODE OFF)
endif()

############################### 参数设置区 #################################
### 工程源文件设置 ### 
set(PROJECT_SOURCES
    main.cpp
)

#### 子文件目录 ####
set(SUB_DIRS
include
source
)
############################### QT 设置区 #####################################
##### QT 版本 5
set(QT_VERSION_5 true)

#### QT 版本 6
set(QT_VERSION_6 false)

#### QT 核心模块
set(THIS_QT_CORE 
    Core 
    Widgets 
    Network 
    Gui 
    Svg
    Sql
    Charts
    OpenGL
    Concurrent
    3DRender
    3DExtras
)

#### QT5 核心库
if(QT_VERSION_5)
    set(THIS_QT_CORE_LIB 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Network 
        Qt5::Gui 
        Qt5::Svg 
        Qt5::Sql 
        Qt5::Charts 
        Qt5::OpenGL 
        Qt5::Concurrent 
        Qt5::3DRender 
        Qt5::3DExtras
    )
endif()

#### QT6 核心库
if(QT_VERSION_6)
    set(THIS_QT_CORE_LIB Qt6::Core)
endif()
### 用于参数设置，根据自己的工程情况进行修改 ###
if(WIN32)
    ### windows 环境下 exe 输出路径设置 ###
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out/build")
    ### 三方库 头文件的 输出路径设置 ###
    set(CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/include")
    message(STATUS "CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
    ### QT 的安装路径设置 ###
    set(QT_INSTALL_DIR "D:/Soft/QT/5.14.2/msvc2017_64/")
    ### 动态库的输出路径设置 ###

    ### 创建输出目录 ###
    make_directory(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    make_directory(${CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY})

else(UNIX)
    ### linux 环境下 exe 输出路径设置 ###
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out/build")
    ### 三方库 头文件的 输出路径设置 ###
    set(CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/include")
    
    ### QT 的安装路径设置 ###
    set(QT_INSTALL_DIR "xxxxx")
    ### 创建输出目录 ###
    make_directory(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    make_directory(${CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY})
endif()

### 设置是从外部目录导入还是从运行目录导入###
# 默认从运行目录导入#
set(SET_LINK_INCLUE_DIR_THIS ON)


######################## 添加动态库 按照模板添加即可 #######################
###############################模板 - 动态库################################

#### 设置局部变量 用于存储 动态库的路径####
#### 编译环境分为 window 与 linux 请根据自己的环境设置 ####

############################# windows 环境 #########################################
#### 动态库路径设置 dll \ lib####

#### DYNAMIC_LIB_DIR 是动态库 LIB 的路径 ####
#### 这里只需要将动态库文件路径加入即可 ####

if(DEBUG_MODE)

    set(WIN32_DYNAMIC_LIB_DIR
        ${ADD_OTHER_LIB}
        )
    #### 动态库 DLL 的路径 ####
    #### 若存储路径和LIB 路径不同 请设置 ####
    set(WIN32_DYNAMIC_DLL_DIR 
        ${ADD_OTHER_DLL}
        )
    #### 动态库头文件 h 路径设置 ####
    #### 这里只需要将动态库 头文件 路径加入即可 ####
    set(WIN32_DYNAMIC_INCLUDE_DIR 
        ${ADD_OTHER_HEADER}
        )
else()

    set(WIN32_DYNAMIC_LIB_DIR
        "D:/Soft/AtamiLib/CommunicationModule/Lib/Release/"
        "D:/Soft/opencv/Release/"

        )
    #### 动态库 DLL 的路径 ####
    #### 若存储路径和LIB 路径不同 请设置 ####
    set(WIN32_DYNAMIC_DLL_DIR 
        #"/home/Test11/lib/"
    )
    #### 动态库头文件 h 路径设置 ####
    #### 这里只需要将动态库 头文件 路径加入即可 ####
    set(WIN32_DYNAMIC_INCLUDE_DIR 
        "D:/Soft/AtamiLib/CommunicationModule/include/"
        "D:/Soft/opencv/build/include/"
    )
endif()

## 拷贝 特定文件夹下到运行目录 ###
if(WIN32)
    #foreach(var IN ITEMS ${CMAKE_CURRENT_SOURCE_DIR}/AtamiProfile)
        #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/AtamiProfile/  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AtamiProfile)
    #endforeach()
endif()




################################# linux 环境 ###############################
#### 动态库路径设置so ####
if(DEBUG_MODE)
    set(LINUX_DYNAMIC_SO_DIR
        "/home/user01/CommunicationModule/Lib/Debug/"
        )
        #### 动态库头文件 h 路径设置 ####
        #### 这里只需要将动态库 头文件 路径加入即可 ####
    set(LINUX_DYNAMIC_INCLUDE_DIR 
    "/home/user01/CommunicationModule/Include/"
        )

else()
    set(LINUX_DYNAMIC_SO_DIR
        "/home/user01/CommunicationModule/Lib/Release/"
        )
        #### 动态库头文件 h 路径设置 ####
        #### 这里只需要将动态库 头文件 路径加入即可 ####
     set(LINUX_DYNAMIC_INCLUDE_DIR 
         "/home/user01/CommunicationModule/Include/"
        )
endif()



######################## 参数检查 ###################

#### 检查文件路径是否存在 ####
if(WIN32)
    foreach(var IN ITEMS ${WIN32_DYNAMIC_LIB_DIR} ${WIN32_DYNAMIC_DLL_DIR} ${WIN32_DYNAMIC_INCLUDE_DIR})
        if(NOT EXISTS ${var})
            message(FATAL_ERROR "${var} 不存在，请检查路径是否正确")
        endif()
    endforeach()
else()
    foreach(var IN ITEMS ${LINUX_DYNAMIC_SO_DIR} ${LINUX_DYNAMIC_INCLUDE_DIR})
        if(NOT EXISTS ${var})
            message(FATAL_ERROR "${var} 不存在，请检查路径是否正确")
        endif()
    endforeach()
endif()

################################################## 功能区 ##################################
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 解决msvc下 Qt乱码的问题
add_compile_options("/utf-8")
add_definitions(-DUNICODE -D_UNICODE)

# 多处理器编译，加快编译速度
add_compile_options(/MP)

## 设置 工程名称与版本号
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)
############################################### 文件拷贝设置 ###########################################


### 拷贝动态库头文件到运行目录 ###
if(WIN32)
    foreach(var IN ITEMS ${WIN32_DYNAMIC_INCLUDE_DIR})
        file(COPY ${var} DESTINATION ${CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY})
        message(STATUS "拷贝动态库头文件到运行目录/include: ${var}")
    endforeach()
    message(STATUS "拷贝动态库头文件到运行目录/include: 完成")
else(UNIX)
    foreach(var IN ITEMS ${LINUX_DYNAMIC_INCLUDE_DIR})
        file(COPY ${var} DESTINATION ${CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY})
    endforeach()
endif()

### 拷贝动态库 lib 到运行目录 ###
if(WIN32)
    foreach(varLIB IN ITEMS ${WIN32_DYNAMIC_LIB_DIR})
        ### 查询动态库文件 ###
        message(STATUS "动态库目录:  ${varLIB}")

        ### 去指定目录下查找动态库文件 ###
        file(GLOB_RECURSE WIN_32_DYNAMIC_LIB_FILES_LIB DEFAULT_RECURSE "${varLIB}*.lib")
        #file(GLOB_RECURSE WIN_32_DYNAMIC_LIB_FILES_LIB ${varLIB} "**/*.lib")
        message(STATUS "WIN32_DYNAMIC_LIB_FILES_LIB: ${WIN_32_DYNAMIC_LIB_FILES_LIB}")
        ### 查询动态库文件 ###
        file(COPY ${WIN_32_DYNAMIC_LIB_FILES_LIB} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)
        #### 清除变量 ###
        message(STATUS "拷贝动态库 lib 到运行目录/lib: ${WIN_32_DYNAMIC_LIB_FILES_LIB}")
        ### 调试信息 ###
    endforeach()
else(LINUX)
    foreach(var IN ITEMS ${LINUX_DYNAMIC_SO_DIR})
        ### 查询动态库文件 ###
        file(COPY ${var} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()
endif()

### 拷贝动态库 dll 到运行目录 ###
if(WIN32)
    foreach(var IN ITEMS ${WIN32_DYNAMIC_DLL_DIR})
        ### 遍历当前目录下所有动态库文件 并拷贝到运行目录 ###
        file(GLOB_RECURSE WIN32_DYNAMIC_DLL_FILES "${var}*.dll")
        ### 并拷贝到运行目录 ###
        foreach(file ${WIN32_DYNAMIC_DLL_FILES})
            file(COPY ${file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
            message(STATUS "拷贝动态库 dll 到运行目录: ${file}")
        endforeach()
        #file(COPY ${WIN32_DYNAMIC_DLL_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        ### 调试信息 ###
        #message(STATUS "拷贝动态库 dll 到运行目录: ${var}")

        if(CREATE_THIRD_PARTY_LIB_DIR)
            ### 创建子目录 用于收集 dll文件 ###
            make_directory(${var}/DLL_DIR)
            ### 拷贝 dll 文件到子目录 ###
            file(COPY ${WIN32_DYNAMIC_DLL_FILES} DESTINATION ${var}/DLL_DIR)
            ### 调试信息 ###
            message(STATUS "拷贝动态库 dll 三方库目录/DLL_DIR: ${var}/DLL_DIR")
        endif()
        
    endforeach()
else(UNIX)
    foreach(var IN ITEMS ${LINUX_DYNAMIC_SO_DIR})
        ### 遍历当前目录下所有动态库文件 并拷贝到运行目录 ###
        file(GLOB_RECURSE LINUX_DYNAMIC_SO_FILES ${var} "**/*.so")
        ### 并拷贝到运行目录 ###
        file(COPY ${LINUX_DYNAMIC_SO_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()
endif()
##################################### 动态库头文件路径 #####################################
if(WIN32)
    ### 动态库头文件路径设置 ###
    if(SET_LINK_INCLUE_DIR_THIS)
        ### 从拷贝的头文件路径中设置 即运行目录的头文件路径 ###
        include_directories(${CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY})
    else()
        ### 从外部目录设置 即外部库的头文件路径 ###
        foreach(var IN ITEMS ${WIN32_DYNAMIC_INCLUDE_DIR})
            include_directories(${var})
        endforeach()
    endif()
else(LINUX)
    if(SET_LINK_INCLUE_DIR_THIS)
        ### 从拷贝的头文件路径中设置 即运行目录的头文件路径 ###
        include_directories(${CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY})
    else()
        ### 从外部目录设置 即外部库的头文件路径 ###
        foreach(var IN ITEMS ${LINUX_DYNAMIC_INCLUDE_DIR})
            include_directories(${var})
        endforeach()
    endif()
endif()

##################################### 动态库路径设置 #####################################
if(WIN32)
    if(SET_LINK_INCLUE_DIR_THIS)
        ### 从拷贝的动态库路径中设置 即运行目录的动态库路径 ###
        link_directories("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib")
    else()
        ### 从外部目录设置 即外部库的动态库路径 ###
        foreach(var IN ITEMS ${WIN32_DYNAMIC_LIB_DIR})
             ### 动态库文件加入链接目录 ###*/*.lib")
            link_directories(${WIN32_DYNAMIC_LIB_FILES})
        endforeach()
    endif()
else(LINUX)
    if(SET_LINK_INCLUE_DIR_THIS)
        ### 从拷贝的动态库路径中设置 即运行目录的动态库路径 ###
        file(GLOB_RECURSE LINUX_DYNAMIC_SO_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} "**/*.so")
        ### 动态库文件加入链接目录 ###
        link_directories(${LINUX_DYNAMIC_SO_FILES})
    else()
        ### 从外部目录设置 即外部库的动态库路径 ###
        foreach(var IN ITEMS ${LINUX_DYNAMIC_SO_DIR})
            ### 查询动态库文件 ###
            file(GLOB_RECURSE LINUX_DYNAMIC_SO_FILES ${var} "**/*.so")
            ### 动态库文件加入链接目录 ###
            link_directories(${LINUX_DYNAMIC_SO_FILES})
            ### 调试信息 ###
            message(STATUS "LINUX_DYNAMIC_SO_FILES: ${LINUX_DYNAMIC_SO_FILES}")
        endforeach()
    endif()
endif()

################################ 子目录设置 ############################
aux_source_directory(. ALL_SRCS)
foreach(dir IN ITEMS ${SUB_DIRS})
    #add_subdirectory(${dir})
    file(GLOB_RECURSE SUB_SRCS ${dir}/*.cpp)
    list(APPEND ALL_SRCS ${SUB_SRCS})

    file(GLOB_RECURSE SUB_HEADERS ${dir}/*.h)
    list(APPEND ALL_HEADERS ${SUB_HEADERS})
endforeach()
message(STATUS "子目录添加完成: ${ALL_SRCS}")
################################ QT设置 ################################
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${THIS_QT_CORE})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${THIS_QT_CORE})

#qt_standard_project_setup()

############################### 使用QT生成可执行文件 #########################

file(GLOB_RECURSE UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
file(GLOB_RECURSE QRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
qt5_wrap_ui(UI_FILES_H ${UI_FILES})
qt5_add_resources(QRC_FILES_REC ${QRC_FILES})

message(STATUS "UI_FILES: ${UI_FILES_H}")
message(STATUS "QRC_FILES: ${QRC_FILES_REC}")

foreach(var IN ITEMS ${UI_HEADERS})
    set_source_files_properties(${var} PROPERTIES HEADER_FILE_ONLY TRUE)
endforeach()





if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(
        ${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${UI_FILES_H}
        ${QRC_FILES_REC}
        ${ALL_HEADERS}
        ${ALL_SRCS}
    )
endif()





##################################### 动态库路径设置 #####################################

set(THRID_PARTY_LIB_DIR_LIST "")
if(WIN32)
    if(SET_LINK_INCLUE_DIR_THIS)
        ### 从拷贝的动态库路径中设置 即运行目录的动态库路径 ###
        file(GLOB_RECURSE WIN32_DYNAMIC_LIB_FILES_RUN_LIB "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib/*.lib")
        list(APPEND THRID_PARTY_LIB_DIR_LIST ${WIN32_DYNAMIC_LIB_FILES_RUN_LIB})
    else()
        ### 从外部目录设置 即外部库的动态库路径 ###
        foreach(var IN ITEMS ${WIN32_DYNAMIC_LIB_DIR})
            ### 查询动态库文件 ###
            file(GLOB_RECURSE WIN32_DYNAMIC_LIB_FILES ${var} "**/*.lib")
            list(APPEND THRID_PARTY_LIB_DIR_LIST ${WIN32_DYNAMIC_LIB_FILES})
        endforeach()
    endif()
else(LINUX)
    if(SET_LINK_INCLUE_DIR_THIS)
        ### 从拷贝的动态库路径中设置 即运行目录的动态库路径 ###
        file(GLOB_RECURSE LINUX_DYNAMIC_SO_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} "**/*.so")
        ### 动态库文件加入链接目录 ###
        link_directories(${LINUX_DYNAMIC_SO_FILES})
    else()
        ### 从外部目录设置 即外部库的动态库路径 ###
        foreach(var IN ITEMS ${LINUX_DYNAMIC_SO_DIR})
            ### 查询动态库文件 ###
            file(GLOB_RECURSE LINUX_DYNAMIC_SO_FILES ${var} "**/*.so")
            ### 动态库文件加入链接目录 ###
            link_directories(${LINUX_DYNAMIC_SO_FILES})
            ### 调试信息 ###
            message(STATUS "LINUX_DYNAMIC_SO_FILES: ${LINUX_DYNAMIC_SO_FILES}")
        endforeach()
    endif()
endif()



######################## 链接库设置 ######################
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
                        ${THIS_QT_CORE_LIB}
                        ${WIN32_DYNAMIC_LIB_FILES}
                        ${THRID_PARTY_LIB_DIR_LIST}
                        )
else(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC
                        ${THIS_QT_CORE_LIB}
                        ${LINUX_DYNAMIC_SO_FILES}
                        )
endif()


######################### 其他设置 ###############
# 根据QT版本设置MACOSX_BUNDLE_GUI_IDENTIFIER选项
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QTWidgetAndLib)
endif()

# 设置QTWidgetAndLib目标的属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 如果QT主版本为6，则调用qt_finalize_executable函数
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTWidgetAndLib)
endif()
########################################### 安装设置 ###################################

### 设置安装路径 ###
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)
if(WIN32)
    if(DEBUG_MODE)
        # 安装 .lib 和 .dll 文件
        install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
            DESTINATION lib
            FILES_MATCHING PATTERN "*.lib"
        )
        install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
            DESTINATION bin
            FILES_MATCHING PATTERN "*.dll"
        )

        # 安装 Qt 库
        foreach(var IN ITEMS ${THIS_QT_CORE})
            install(DIRECTORY ${QT_INSTALL_DIR}/bin/
                DESTINATION bin
                FILES_MATCHING PATTERN "Qt5${var}d.dll"
            )
        endforeach()


        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/iconengines
            DESTINATION bin
            FILES_MATCHING PATTERN "*"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/imageformats
            DESTINATION bin
            FILES_MATCHING PATTERN "*"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/platforms
            DESTINATION bin
            FILES_MATCHING PATTERN "qwindowsd.dll"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/styles
            DESTINATION bin
            FILES_MATCHING PATTERN "qwindowsvistastyled.dll"
        )

    else()
        # 安装 .lib 和 .dll 文件
        install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
            DESTINATION lib
            FILES_MATCHING PATTERN "*.lib"
        )
        install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
            DESTINATION bin
            FILES_MATCHING PATTERN "qwindowsvistastyled.dll"
        )

         # 安装 Qt 库
        foreach(var IN ITEMS ${THIS_QT_CORE})
            install(DIRECTORY ${QT_INSTALL_DIR}/bin/
                DESTINATION bin
                FILES_MATCHING PATTERN "Qt5${var}.dll"
            )
        endforeach()

       

        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/iconengines
            DESTINATION bin
            FILES_MATCHING PATTERN "*"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/imageformats
            DESTINATION bin
            FILES_MATCHING PATTERN "*"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/platforms
            DESTINATION bin
            FILES_MATCHING PATTERN "qwindows.dll"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/styles
            DESTINATION bin
            FILES_MATCHING PATTERN "qwindowsvistastyle.dll"
        )
    endif()
endif()

if(UNIX)
    if(DEBUG_MODE STREQUAL "Debug")
        # 安装 .so 文件
        install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
            DESTINATION lib
            FILES_MATCHING PATTERN "*.so"
        )
        # 安装 Qt 库
        install(DIRECTORY ${QT_INSTALL_DIR}/lib/
            DESTINATION lib
            FILES_MATCHING PATTERN "*.so*"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/
            DESTINATION lib
            FILES_MATCHING PATTERN "*"
        )
    else()
        # 安装 .so 文件
        install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
            DESTINATION lib
            FILES_MATCHING PATTERN "*.so"
        )
        # 安装 Qt 库
        install(DIRECTORY ${QT_INSTALL_DIR}/lib/
            DESTINATION lib
            FILES_MATCHING PATTERN "*.so*"
        )
        # 安装 Qt 平台 #直接拷贝文件夹
        install(DIRECTORY ${QT_INSTALL_DIR}/plugins/
            DESTINATION lib
            FILES_MATCHING PATTERN "*"
        )
    endif()
endif()
### 设置安装路径 ###
install(DIRECTORY ${CMAKE_LIBRARY_INCLUDE_OUTPUT_DIRECTORY}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
### 设置安装路径 ###
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
    DESTINATION bin
    FILES_MATCHING PATTERN "*.dll"
)


### 安装其他三方库 ###
if(WIN32)
    ### 查询 WIN32_DYNAMIC_LIB_DIR 里有多个路径 ###
    foreach(var IN ITEMS ${WIN32_DYNAMIC_LIB_DIR})
        install(DIRECTORY ${var}/
            DESTINATION lib
            FILES_MATCHING PATTERN "**/*.lib"
         )
    endforeach()
    ### 查询 WIN32_DYNAMIC_DLL_DIR 里有多个路径 ###
    foreach(var IN ITEMS ${WIN32_DYNAMIC_DLL_DIR})
        install(DIRECTORY ${var}/
            DESTINATION bin
            FILES_MATCHING PATTERN "**/*.dll"
         )
    endforeach()
    ### 查询 WIN32_DYNAMIC_INCLUDE_DIR 里有多个路径 ###
    foreach(var IN ITEMS ${WIN32_DYNAMIC_INCLUDE_DIR})
        install(DIRECTORY ${var}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
         )
    endforeach()
else(UNIX)
    ### 查询 LINUX_DYNAMIC_SO_DIR 里有多个路径 ###
    foreach(var IN ITEMS ${LINUX_DYNAMIC_SO_DIR})
        install(DIRECTORY ${var}/
            DESTINATION lib
            FILES_MATCHING PATTERN "**/*.so"
         )
    endforeach()
    ### 查询 LINUX_DYNAMIC_INCLUDE_DIR 里有多个路径 ###
    foreach(var IN ITEMS ${LINUX_DYNAMIC_INCLUDE_DIR})
        install(DIRECTORY ${var}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
         )
    endforeach()
endif()

endif()
