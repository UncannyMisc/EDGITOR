
REM x64-windows-static-md triplets are required for debug builds and x64-windows-static for release
REM I'm not entirely certain why, this is just what I found from testing on various personal projects. - Keith
echo Using VCPKG_ROOT from %VCPKG_ROOT%
vcpkg install sdl2:x64-windows-static-md sdl2:x64-windows-static
vcpkg install sdl2-image:x64-windows-static-md sdl2-image:x64-windows-static
vcpkg install sdl2-ttf:x64-windows-static-md sdl2-ttf:x64-windows-static
