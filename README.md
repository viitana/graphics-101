# graphics-101

graphics-101 is a minimal & standalone OpenGL test application build for demo purposes.

> **If you are taking part in the code-along presentation, make sure you are able to build and run the project beforehand. Upon launching, you should see an empty grey window which you can close with ESC.**
>
> **See the build instructions below! :)**

## Building

First, clone the repo! Next, follow the instructions based on your platform:

### Windows

1. On Windows, this project uses `clang` (i.e. the `LLVM` toolchain) to compile (the project is not MSVC-compatible). If you don't already have it installed, do:
    ```bash
    # In the installer, make sure to select to add LLVM to the system PATH (current or all users)
    winget install -i LLVM.LLVM
    ```
    Alternatively, you can also install without the command-line. See [releases.llvm.org](https://releases.llvm.org)
2. To build, do
    ```bash
    # Note: If you just installed the compiler, you may need to restart your terminal
    clang++ main.cpp -I includes -w -o demo.exe
    ```
3. Done! The build process should ouput a `demo.exe` binary which you can run.

### macOS

1. If you do not have `clang`, install it with Brew:
    ```bash
    brew install llvm
    ```
    Alternatively, if you have Xcode, you might already have a bundled version of clang. To check, do
    ```bash
    xcodebuild -find clang++
    # This will print out the path to your clang compiler if installed
    ```
2. To build, do
    ```bash
    clang++ main.cpp -lm -lGL -I includes -o demo
    # If you are using Xcode clang, replace ´clang++´ with the full path to the compiler
    ```
3. Done! The build process should ouput a `demo` binary which you can run.

### linux

1. Install the `GCC` and/or `clang/LLVM` toolchain. On Debian systems (e.g. Ubuntu), the `build-essential` package is recommended  (installs GCC):
    ```bash
    sudo apt install build-essential
    ```
2. To build, do
    ```bash
    # For GCC
    g++ main.cpp -lX11 -lm -lGL -I includes -o demo
    # For clang/LLVM
    clang++ main.cpp -lX11 -lm -lGL -I includes -o demo
    ```
3. Done! The build process should ouput a `demo` binary which you can run.
