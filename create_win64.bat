mkdir build_win64
set INSTALL_DIR=%~dp0\..\installed
cd build_win64
cmake -G "Visual Studio 11 Win64" -DBUILD_SHARED_LIBS=true -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
pause