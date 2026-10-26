@echo off
rem ARM64X from the command line: compile ARM64 and ARM64EC objects, link both with /machine:arm64x
setlocal
for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -property installationPath`) do set "VS=%%i"
set "VCVARS=%VS%\VC\Auxiliary\Build\vcvarsall.bat"
set "L=%~dp0libs"
set "INC=/I"%L%\zlib\include" /I"%L%\libpng\include" /I"%L%\pugixml\include""
if not exist out mkdir out
call "%VCVARS%" arm64 > nul || exit /b 1
cl /nologo /c /MD /O2 /EHsc /std:c++17 %INC% oftest.cpp /Fo:oftest_arm64.obj || exit /b 1
cl /nologo /c /MD /O2 /EHsc /std:c++17 /arm64EC %INC% oftest.cpp /Fo:oftest_arm64ec.obj || exit /b 1
link /nologo /dll /machine:arm64x /out:out\oftest.dll oftest_arm64.obj oftest_arm64ec.obj ^
	"%L%\zlib\lib\vs\ARM64\zlib.lib" "%L%\libpng\lib\vs\ARM64\libpng.lib" "%L%\pugixml\lib\vs\ARM64\pugixml.lib" ^
	"%L%\zlib\lib\vs\ARM64EC\zlib.lib" "%L%\libpng\lib\vs\ARM64EC\libpng.lib" "%L%\pugixml\lib\vs\ARM64EC\pugixml.lib" || exit /b 1
cl /nologo /MD host.c /Fe:out\host_arm64.exe || exit /b 1
cl /nologo /MD /arm64EC host.c /Fe:out\host_arm64ec.exe /link /machine:arm64ec || exit /b 1
call "%VCVARS%" arm64_x64 > nul || call "%VCVARS%" x64 > nul || exit /b 1
cl /nologo /MD host.c /Fe:out\host_x64.exe || exit /b 1
echo built out\oftest.dll and the hosts
