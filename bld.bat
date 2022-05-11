@echo off

:: set winsdkdir="%WindowsSdkDir%Include\%WindowsSDKVersion%um"
set compileropts=-DDEBUG_MODE -D_ENABLE_ATOMIC_ALIGNMENT_FIX -W4 -wd4100 -Od -EHsc -Zi -Gm- -fp:fast -nologo
:: -I%winsdkdir%
set linkeropts=-MAP -opt:ref -subsystem:windows,6.00 -incremental:no
set libraries=User32.lib Gdi32.lib Ole32.lib Winmm.lib Advapi32.lib D3D11.lib DXGI.lib

call shdr.bat

mkdir w:\d3d-build >nul 2>nul
pushd w:\d3d-build
del w:\d3d-build\*.pdb /F /Q 2>nul
cl %compileropts% -MT -Fe"d3d-example.exe" w:\d3d-example\code\win_main.cpp %libraries% /link %linkeropts%
popd
