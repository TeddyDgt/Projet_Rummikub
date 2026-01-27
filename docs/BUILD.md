# Build & Run (Windows / Linux)

This project vendors GLFW in `third_party/glfw` and builds it automatically.
Miniaudio is vendored in `third_party/miniaudio`.

## Linux
Dependencies (Ubuntu/Debian):
```
sudo apt install build-essential cmake libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev
```

Build and run:
```
cd "Projet_Rummikub/src"
make
./rummikub_gui
```

## Windows (MSYS2 MinGW)
Install MSYS2 + MinGW and CMake:
- MSYS2 shell: **MINGW64** (not MSYS).
- Packages: `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make`

Build and run:
```
cd /c/Users/<you>/Projet_Rummikub/src
make
./rummikub_gui.exe
```

If CMake complains about generator, force it:
```
make GLFW_CMAKE_GEN="MinGW Makefiles"
```

## Audio assets
Put the WAV files in `audio/` (next to the executable or project root):
- `bgm.wav`
- `click.wav`
- `invalid.wav`
- `draw.wav`
- `play.wav`
- `validate.wav`

If audio files are missing, the game still runs (no crash), but no sound.
