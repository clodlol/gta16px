first of all, download SFML 3 from their website and paste the folder into the same directory .gitignore is in

secondly, from SFML/bin folder copy sfml-audio-3.dll, sfml-graphics-3.dll, sfml-network-3.dll, sfml-system-3.dll, sfml-window-3.dll and paste into a new folder named bin in the same directory as the .gitignore

we need assets/tilesheets/map_tilesheet.png in the same folder as the .gitignore

then for msvc compilers, we can run this command

cl.exe /Zi /EHsc /nologo /std:c++17 '/I path/to/SFML/include' 'path\to\src\main.cpp' 'path\to\src\Map.cpp' /link '/LIBPATH: path/to/SFML/lib' sfml-graphics.lib sfml-window.lib sfml-system.lib '/OUT: path/to/bin/game.exe'

here, replace path/to/ with the relevant paths
