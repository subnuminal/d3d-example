@echo off
mkdir w:\d3d-build >nul 2>nul
pushd w:\d3d-build
echo WAITING FOR ALL SHADERS TO COMPILE > ..\d3d-example\assets\shaderlock.tmp
fxc /nologo /T vs_5_0 /Fo ..\d3d-example\assets\shaders\QuadVertex.cso ..\d3d-example\shaders\quads.hlsl /E vs_main 
fxc /nologo /T ps_5_0 /Fo ..\d3d-example\assets\shaders\QuadPixel.cso ..\d3d-example\shaders\quads.hlsl /E ps_main 
del ..\d3d-example\assets\shaderlock.tmp
popd
