# Microsoft Developer Studio Project File - Name="geekfibres" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=geekfibres - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "geekfibres.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "geekfibres.mak" CFG="geekfibres - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "geekfibres - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"geekfibres.exe"
# Begin Target

# Name "geekfibres - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\animacion.cpp
# End Source File
# Begin Source File

SOURCE=.\src\avance.cpp
# End Source File
# Begin Source File

SOURCE=.\src\boundingbox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\disparo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\juego.cpp
# End Source File
# Begin Source File

SOURCE=.\src\listaimagenes.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\malo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\motorfisico.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mundo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\personaje.cpp
# End Source File
# Begin Source File

SOURCE=.\src\protagonista.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sonido.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\animacion.h
# End Source File
# Begin Source File

SOURCE=.\src\avance.h
# End Source File
# Begin Source File

SOURCE=.\src\bool.h
# End Source File
# Begin Source File

SOURCE=.\src\boundingbox.h
# End Source File
# Begin Source File

SOURCE=.\src\debug.h
# End Source File
# Begin Source File

SOURCE=.\src\disparo.h
# End Source File
# Begin Source File

SOURCE=.\src\editor.h
# End Source File
# Begin Source File

SOURCE=.\src\juego.h
# End Source File
# Begin Source File

SOURCE=.\src\listaimagenes.h
# End Source File
# Begin Source File

SOURCE=.\src\main.h
# End Source File
# Begin Source File

SOURCE=.\src\malo.h
# End Source File
# Begin Source File

SOURCE=.\src\motorfisico.h
# End Source File
# Begin Source File

SOURCE=.\src\mundo.h
# End Source File
# Begin Source File

SOURCE=.\src\personaje.h
# End Source File
# Begin Source File

SOURCE=.\src\portable_sdl.h
# End Source File
# Begin Source File

SOURCE=.\src\portable_sdl_mixer.h
# End Source File
# Begin Source File

SOURCE=.\src\portable_sdl_ttf.h
# End Source File
# Begin Source File

SOURCE=.\src\protagonista.h
# End Source File
# Begin Source File

SOURCE=.\src\resolucion.h
# End Source File
# Begin Source File

SOURCE=.\src\sonido.h
# End Source File
# Begin Source File

SOURCE=.\src\tile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\..\..\Lib_sdl\sdl\lib\SDL.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib_sdl\sdl\lib\SDLmain.lib
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Lib_sdl\SDL_mixer-1.2.5\lib\SDL_mixer.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Lib_sdl\SDL_ttf-2.0.6\lib\SDL_ttf.lib"
# End Source File
# End Target
# End Project
