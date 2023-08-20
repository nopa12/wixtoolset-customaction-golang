call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

del /f /q *.dll
del /f /q *.obj
del /f /q *.pdb
del /f /q *.lib
del /f /q *.exp
del /f /q *.ilk

cl /c /Zi /nologo /GS /W3 /I "..\wix311-binaries\sdk\inc" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "CUSTOMACTIONTEST_EXPORTS" /Fplogging.pch /Felogging.obj /Fdvc142.pdb /FS /Gd /TP logging.c

cl /c /Zi /nologo /GS /W3 /I "..\wix311-binaries\sdk\inc" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "CUSTOMACTIONTEST_EXPORTS" /Fpstdafx.pch /Festdafx.obj /Fdvc142.pdb /FS /Gd /TP stdafx.c

cl /c /Zi /nologo /GS /W3 /I "..\wix311-binaries\sdk\inc" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "CUSTOMACTIONTEST_EXPORTS" /FpCustomAction.pch /FeCustomAction.obj /Fdvc142.pdb /FS /Gd /TP CustomAction.c

link /DLL /NOLOGO /MACHINE:X64 /OUT:customaction.dll /LIBPATH:"\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64" /LIBPATH:..\wix311-binaries\sdk\vs2017\lib\x64\ /PDB:vc142.pdb /INCREMENTAL /SUBSYSTEM:WINDOWS /DLL ws2_32.lib Shell32.lib version.lib user32.lib AdvAPI32.Lib Msi.Lib deputil.lib dutil.lib wcautil.lib CustomAction.obj stdafx.obj logging.obj