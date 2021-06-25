SET(imgRe_lib_SRCS
    core/imgType.h
    core/imgMgr.h
    core/imgMgr.cpp 
    core/imgViewPort.h
    core/imgViewPort.cpp
    core/imgFileMgr.h
    core/imgFileMgr.cpp
	core/imgFrame.h
	core/imgFrame.cpp
	core/imgConfig.h
	core/imgConfig.cpp
	core/selRect.h
	core/selRect.cpp
	core/imgScene.h
	core/imgScene.cpp
	core/viewPort.h
	core/viewPort.cpp
	core/feature.h
	core/feature.cpp
	core/treeitem.h
	core/treeitem.cpp
	core/treemodel.h
	core/treemodel.cpp
	core/treeView.h
	core/treeView.cpp
	core/progressIndicator.h
	core/progressIndicator.cpp
	core/matchThread.h
	core/matchThread.cpp
	core/labelItem.h
        core/labelItem.cpp
        core/match.cpp
		)


#############################  GUI plugin compilation ###############################
SET(imgRe_gui_SRCS
        gui/autoGenTileDialog.h
        gui/autoGenTileDialog.cpp
        gui/zoomDialog.h
        gui/zoomDialog.cpp
        gui/mainWindow.h
        gui/mainWindow.cpp
        gui/scrollDialog.cpp
        gui/scrollDialog.h
        gui/tileSelDialog.h
        gui/tileSelDialog.cpp
		gui/tilesListDialog.h
                gui/tilesListDialog.cpp
        )

SET(imgRe_UIS
        gui/mainWindow.ui
        gui/autoGenTilesDialog.ui
        gui/scrollDialog.ui
        gui/settingDialog.ui
        gui/tileListDialog.ui
        gui/tileSelDialog.ui
        gui/zoomDialog.ui
                gui/test.ui
)

QT5_WRAP_UI(imgRe_UIS_H ${imgRe_UIS})

SET(imgRe_exe_SRCS main.cpp)

################# compiles resources files ############
SET(imgRe_RES
        ${CMAKE_SOURCE_DIR}/data/icon_r.qrc
        )
QT5_ADD_RESOURCES(imgRe_RES_CXX ${imgRe_RES})

SET(imgRe_lib_SRCS ${imgRe_lib_SRCS} ${imgRe_gui_SRCS} ${imgRe_UIS_H} )

IF(WIN32)
  IF( MINGW )
    IF(NOT CMAKE_RC_COMPILER)
             set(CMAKE_RC_COMPILER windres.exe)
    ENDIF()

    ENABLE_LANGUAGE(RC)
    # resource compilation for MinGW
    ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_BINARY_DIR}/AppIco.o
                        COMMAND ${CMAKE_RC_COMPILER}
                                -I${CMAKE_CURRENT_SOURCE_DIR}
                                -i${CMAKE_BINARY_DIR}/imgRes.rc
                                -o ${CMAKE_BINARY_DIR}/AppIco.o )

    SET(imgRe_lib_SRCS ${imgRe_lib_SRCS} ${CMAKE_BINARY_DIR}/AppIco.o)
  ELSE( MINGW )
    SET(imgRe_lib_SRCS ${imgRe_lib_SRCS} ${CMAKE_BINARY_DIR}/imgRes.rc) # Error on visual studio 2008
  ENDIF( MINGW )
ENDIF(WIN32)

################################ Build main program #########################################

#opencv3
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})  # Not needed for CMake >= 2.8.11
SET(IMGRE_OPENCV_LIBRARIES ${OpenCV_LIBS})


# Setup Qt5 dependencies
SET(IMGRE_QT_LIBRARIES Qt5::Core Qt5::Gui  Qt5::Widgets)


SET(IMGRE_DEPS  ${IMGRE_QT_LIBRARIES} ${IMGRE_OPENCV_LIBRARIES})

#set opencv depencies



ADD_EXECUTABLE(imgRes WIN32 ${imgRe_exe_SRCS}  ${imgRe_lib_SRCS}  ${imgRe_RES_CXX})

TARGET_LINK_LIBRARIES(imgRes ${IMGRE_DEPS})

