# Microsoft Developer Studio Project File - Name="mainCmd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mainCmd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mainCmdApiSim.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mainCmdApiSim.mak" CFG="mainCmd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mainCmd - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mainCmd - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "mainCmd"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mainCmd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "c:\pss_w32out\mainCmd\Release"
# PROP Intermediate_Dir "c:\pss_w32out\mainCmd\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40d /d "NDEBUG"
# ADD RSC /l 0x40d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "mainCmd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\pss_w32out\apiSimulation\mainCmd\Debug"
# PROP Intermediate_Dir "c:\pss_w32out\apiSimulation\mainCmd\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\mainDrv\h" /I "..\..\..\mainOs\h" /I ".\h" /I "..\..\..\mainExtDrv\h" /I "..\..\h" /I "..\..\..\simulation\h" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_API_SIMULATION_" /FR /FD /GZ /c
# ADD BASE RSC /l 0x40d /d "_DEBUG"
# ADD RSC /l 0x40d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "mainCmd - Win32 Release"
# Name "mainCmd - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter "c"
# Begin Group "cmdDb"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\src\cmdShell\cmdDb\cmdBase.c

!IF  "$(CFG)" == "mainCmd - Win32 Release"

!ELSEIF  "$(CFG)" == "mainCmd - Win32 Debug"

# ADD CPP /D "CMD_SHELL_DEBUG" /D "CMD_BANNER" /D "FIELD_PROMPT" /D "CMD_INCLUDE_SERIAL"

!ENDIF 

# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\src\cmdShell\common\cmdVersion.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\common\cmdWrapUtils.c
# End Source File
# End Group
# Begin Group "os"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\src\cmdShell\os\win32\os_w32IO.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\os\win32\os_w32Mutex.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\os\win32\os_w32Serial.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\os\win32\os_w32Socket.c
# End Source File
# End Group
# Begin Group "shell"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\src\cmdShell\shell\cmdConsole.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\shell\cmdEngine.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\shell\cmdMain.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\shell\cmdParser.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\shell\cmdServer.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\shell\cmdStream.c
# End Source File
# Begin Source File

SOURCE=..\..\src\cmdShell\shell\userInitCmdDb.c
# End Source File
# End Group
# End Group
# End Target
# End Project
