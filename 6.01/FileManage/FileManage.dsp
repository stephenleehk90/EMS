# Microsoft Developer Studio Project File - Name="FileManage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FileManage - Win32 StandardDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FileManage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FileManage.mak" CFG="FileManage - Win32 StandardDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FileManage - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FileManage - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FileManage - Win32 StandardRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FileManage - Win32 StandardDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FileManage - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../editpro/Release/editpro.lib ..\commkits\Release\CommKits.lib ..\fileoper\Release\fileoper.lib ..\fzado\release\fzado.lib ..\emssc\release\emssc.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\Release\FileManage.dll"

!ELSEIF  "$(CFG)" == "FileManage - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICDOE" /D "_UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\editpro\debug\editpro.lib ..\commkits\debug\CommKits.lib ..\fileoper\debug\fileoper.lib ..\fzado\debug\fzado.lib ..\emssc\debug\emssc.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Debug\FileManage.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "FileManage - Win32 StandardRelease"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FileManage___Win32_StandardRelease"
# PROP BASE Intermediate_Dir "FileManage___Win32_StandardRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StandardRelease"
# PROP Intermediate_Dir "StandardRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_STANDARD_VERSION" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../editpro/Release/editpro.lib ..\commkits\Release\CommKits.lib ..\fileoper\Release\fileoper.lib ..\fzado\release\fzado.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\Release/FileManage.dll"
# ADD LINK32 ../editpro/StandardRelease/editpro.lib ..\commkits\StandardRelease\CommKits.lib ..\fileoper\StandardRelease\fileoper.lib ..\fzado\Standardrelease\fzado.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\StandardRelease/FileManage.dll"

!ELSEIF  "$(CFG)" == "FileManage - Win32 StandardDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FileManage___Win32_StandardDebug"
# PROP BASE Intermediate_Dir "FileManage___Win32_StandardDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "StandardDebug"
# PROP Intermediate_Dir "StandardDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_STANDARD_VERSION" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\editpro\debug\editpro.lib ..\commkits\debug\CommKits.lib ..\fileoper\debug\fileoper.lib ..\fzado\debug\fzado.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Debug\FileManage.dll" /pdbtype:sept
# ADD LINK32 ..\editpro\Standarddebug\editpro.lib ..\commkits\Standarddebug\CommKits.lib ..\fileoper\Standarddebug\fileoper.lib ..\fzado\Standarddebug\fzado.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\StandardDebug\FileManage.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FileManage - Win32 Release"
# Name "FileManage - Win32 Debug"
# Name "FileManage - Win32 StandardRelease"
# Name "FileManage - Win32 StandardDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\annmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\anntoolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\AttachFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\include\ChildMDIFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildRepFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChoseDicDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ClearFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ColReport.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyList.cpp
# End Source File
# Begin Source File

SOURCE=.\CountryCodeSel.cpp
# End Source File
# Begin Source File

SOURCE=.\crvtrackcursorinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\crystalreportviewer9.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDisplyPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOpenUnsaveFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgViewPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\DoColDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DoColFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DoColMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DoColTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DoColView.cpp
# End Source File
# Begin Source File

SOURCE=.\DoFileDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DoFileFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DoFileMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DoFileTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DoFileView.cpp
# End Source File
# Begin Source File

SOURCE=.\dtpicker.cpp
# End Source File
# Begin Source File

SOURCE=.\editdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EditFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\editframenew.cpp
# End Source File
# Begin Source File

SOURCE=.\EditOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\editroomDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\editroomView.cpp
# End Source File
# Begin Source File

SOURCE=.\editview_bottom.cpp
# End Source File
# Begin Source File

SOURCE=.\editview_top.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDB.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoDisplayView.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManage.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManage.def
# End Source File
# Begin Source File

SOURCE=.\FileManage.rc
# End Source File
# Begin Source File

SOURCE=.\FileMgrBase.cpp
# End Source File
# Begin Source File

SOURCE=.\FileRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\FileReport.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font1.cpp
# End Source File
# Begin Source File

SOURCE=.\FSReport.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\inputcommentdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\inputlinenumdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\lead.cpp
# End Source File
# Begin Source File

SOURCE=.\leadmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\leadmarkers.cpp
# End Source File
# Begin Source File

SOURCE=.\leadscrollinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkToDMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListItem.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSt.cpp
# End Source File
# Begin Source File

SOURCE=.\PicListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordReport.cpp
# End Source File
# Begin Source File

SOURCE=.\Recycle.cpp
# End Source File
# Begin Source File

SOURCE=.\ReplaceFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ReplaceView.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveProg.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\searchparam.cpp
# End Source File
# Begin Source File

SOURCE=.\SelPos.cpp
# End Source File
# Begin Source File

SOURCE=.\SelUser.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowTotalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TransDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeViewEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TxtStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\WordCheckDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\annmenu.h
# End Source File
# Begin Source File

SOURCE=.\anntoolbar.h
# End Source File
# Begin Source File

SOURCE=.\AttachFiles.h
# End Source File
# Begin Source File

SOURCE=..\include\ChildMDIFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChildRepFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChoseDicDlg.h
# End Source File
# Begin Source File

SOURCE=.\ClearFile.h
# End Source File
# Begin Source File

SOURCE=.\ColReport.h
# End Source File
# Begin Source File

SOURCE=.\CopyList.h
# End Source File
# Begin Source File

SOURCE=.\CountryCodeSel.h
# End Source File
# Begin Source File

SOURCE=.\crvtrackcursorinfo.h
# End Source File
# Begin Source File

SOURCE=.\crystalreportviewer9.h
# End Source File
# Begin Source File

SOURCE=.\ctest.h
# End Source File
# Begin Source File

SOURCE=.\DlgDisplyPicture.h
# End Source File
# Begin Source File

SOURCE=.\DlgOpenUnsaveFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgViewPicture.h
# End Source File
# Begin Source File

SOURCE=.\DoColDoc.h
# End Source File
# Begin Source File

SOURCE=.\DoColFrm.h
# End Source File
# Begin Source File

SOURCE=.\DoColMgr.h
# End Source File
# Begin Source File

SOURCE=.\DoColTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DoColView.h
# End Source File
# Begin Source File

SOURCE=.\DoFileDoc.h
# End Source File
# Begin Source File

SOURCE=.\DoFileFrm.h
# End Source File
# Begin Source File

SOURCE=.\DoFileMgr.h
# End Source File
# Begin Source File

SOURCE=.\DoFileTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DoFileView.h
# End Source File
# Begin Source File

SOURCE=.\dtpicker.h
# End Source File
# Begin Source File

SOURCE=.\editdoc.h
# End Source File
# Begin Source File

SOURCE=.\editframenew.h
# End Source File
# Begin Source File

SOURCE=.\Editopt.h
# End Source File
# Begin Source File

SOURCE=.\editview_bottom.h
# End Source File
# Begin Source File

SOURCE=.\editview_top.h
# End Source File
# Begin Source File

SOURCE=.\FileDB.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoDisplayView.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\filemanage.h
# End Source File
# Begin Source File

SOURCE=.\FileMgrBase.h
# End Source File
# Begin Source File

SOURCE=.\FileRecord.h
# End Source File
# Begin Source File

SOURCE=.\FileReport.h
# End Source File
# Begin Source File

SOURCE=.\FileSearch.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\font1.h
# End Source File
# Begin Source File

SOURCE=.\font2.h
# End Source File
# Begin Source File

SOURCE=.\FSReport.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\inputcommentdlg.h
# End Source File
# Begin Source File

SOURCE=.\inputlinenumdlg.h
# End Source File
# Begin Source File

SOURCE=.\l_ocx.h
# End Source File
# Begin Source File

SOURCE=.\L_OcxErr.h
# End Source File
# Begin Source File

SOURCE=.\lead.h
# End Source File
# Begin Source File

SOURCE=.\leadmarker.h
# End Source File
# Begin Source File

SOURCE=.\leadmarkers.h
# End Source File
# Begin Source File

SOURCE=.\leadscrollinfo.h
# End Source File
# Begin Source File

SOURCE=.\LinkToDMDlg.h
# End Source File
# Begin Source File

SOURCE=.\ListItem.h
# End Source File
# Begin Source File

SOURCE=.\Optionst.h
# End Source File
# Begin Source File

SOURCE=.\PicListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\Printopt.h
# End Source File
# Begin Source File

SOURCE=.\RecordReport.h
# End Source File
# Begin Source File

SOURCE=.\Recycle.h
# End Source File
# Begin Source File

SOURCE=.\ReplaceFactory.h
# End Source File
# Begin Source File

SOURCE=.\ReplaceView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveProg.h
# End Source File
# Begin Source File

SOURCE=.\SearchMgr.h
# End Source File
# Begin Source File

SOURCE=.\SelPaper.h
# End Source File
# Begin Source File

SOURCE=.\SelPos.h
# End Source File
# Begin Source File

SOURCE=.\ShowTotalDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TransDlg.h
# End Source File
# Begin Source File

SOURCE=.\TreeViewEx.h
# End Source File
# Begin Source File

SOURCE=.\Txtstyle.h
# End Source File
# Begin Source File

SOURCE=.\WordCheckDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\Attach(16).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\attach.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Attachment(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Attachment(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\AttachUnRelateMenu(16).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_check.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_check_disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmfile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00021.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00025.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00026.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00029.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00030.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00031.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00032.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00033.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00034.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00035.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00036.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00037.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00038.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmrcvf.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Copy(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Copy(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Cross(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Cross(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_g.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursorgr.cur
# End Source File
# Begin Source File

SOURCE=".\res\Cut(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Cut(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\dbtreefi.bmp
# End Source File
# Begin Source File

SOURCE=".\res\DelFile(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\DelFile(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\DepartLib(16).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\dofileto.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edittool.bmp
# End Source File
# Begin Source File

SOURCE=".\res\EmergencyFile(16).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\File(16).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\FileLinkMenu(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\FileLinkMenu(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\FileManage.rc2
# End Source File
# Begin Source File

SOURCE=.\res\fl101.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fl102.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fl11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fl111.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fl1112.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fl12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\freehand.bmp
# End Source File
# Begin Source File

SOURCE=".\res\GetFile(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GetFile(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_pic.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lock2.bmp
# End Source File
# Begin Source File

SOURCE=".\res\LockedEmergencySecuriteFile(16).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\LockedFile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LockedSecuriteFile.bmp
# End Source File
# Begin Source File

SOURCE=".\res\LockedSendToTransEmergencyfile(16).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\LockedTransingFile(16).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\LockedTransSecrit.bmp
# End Source File
# Begin Source File

SOURCE=".\res\LockedTransSecritEmergencyFile(16).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Modify(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Modify(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\modifyto.bmp
# End Source File
# Begin Source File

SOURCE=".\res\NextPicture(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\NextPicture(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Node.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NodeOpen.bmp
# End Source File
# Begin Source File

SOURCE=".\res\NotSupportPic(128).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\OriginalSize(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\OriginalSize(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Paste(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Paste(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\PicNotFound(128).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\picviewbar.bmp
# End Source File
# Begin Source File

SOURCE=".\res\PrePicture(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\PrePicture(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\PrePicture_Disable.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Print(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Print(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\PrintPreview(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\PrintPreview(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\redo(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\redo(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Refresh(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Refresh(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\SecritEmergencyFile(16).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\SecritFile.bmp
# End Source File
# Begin Source File

SOURCE=".\res\SendBack(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\SendBack(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\SendToCol_Large(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\SendToCol_Large(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\SendToTransEmergencyFile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SendToTransFile.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Sign(16).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Sign(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Sign(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Submit(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Submit(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\transfer.bmp
# End Source File
# Begin Source File

SOURCE=".\res\TransFile(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TransFile(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TranslateFile(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TranslateFile(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\TransSecrit.bmp
# End Source File
# Begin Source File

SOURCE=".\res\TransSecritEmergencyFile(16).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\undo(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\undo(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\UnLinkFileMenu(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\UnLinkFileMenu(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\UnRelateFile(16).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\UnRelateMenu(16).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Unsign(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Unsign(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\WordBank .bmp"
# End Source File
# Begin Source File

SOURCE=.\res\WordBank_Disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WordChange.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WordChange_Disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WordCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WordCheck_Disable.bmp
# End Source File
# Begin Source File

SOURCE=".\res\ZoomIn(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ZoomIn(24)_Disable.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ZoomOut(24).bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ZoomOut(24)_Disable.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section FileManage : {2DEF4530-8CE6-41C9-84B6-A54536C90213}
# 	2:21:DefaultSinkHeaderFile:crystalreportviewer9.h
# 	2:16:DefaultSinkClass:CCrystalReportViewer9
# End Section
# Section FileManage : {3C9EEBE5-09AB-4EBB-9B42-A25FD57F19C0}
# 	2:5:Class:CCrystalReportViewer9
# 	2:10:HeaderFile:crystalreportviewer9.h
# 	2:8:ImplFile:crystalreportviewer9.cpp
# End Section
# Section FileManage : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont1
# 	2:10:HeaderFile:font1.h
# 	2:8:ImplFile:font1.cpp
# End Section
# Section FileManage : {13FA5946-561C-11D1-BE3F-00A0C95A6A5C}
# 	2:5:Class:CCRVTrackCursorInfo
# 	2:10:HeaderFile:crvtrackcursorinfo.h
# 	2:8:ImplFile:crvtrackcursorinfo.cpp
# End Section
# Section FileManage : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture1
# 	2:10:HeaderFile:picture1.h
# 	2:8:ImplFile:picture1.cpp
# End Section
# Section FileManage : {00100000-B1BA-11CE-ABC6-F5B2E79D9E3F}
# 	2:21:DefaultSinkHeaderFile:lead.h
# 	2:16:DefaultSinkClass:CLead
# End Section
