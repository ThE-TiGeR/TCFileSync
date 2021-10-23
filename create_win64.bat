set INSTALL_DIR=%~dp0\..\installed
mkdir build_win64
pushd build_win64
cmake -G "Visual Studio 16 2019" -A "x64" -DBUILD_SHARED_LIBS=true -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
popd
pause