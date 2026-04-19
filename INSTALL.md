# Building and Installation

HyperArt is built using [CMake](https://cmake.org) and requires the **Qt 6** framework (specifically 6.8 or newer) along with its Widgets and Xml components. 

Below are the instructions to prepare your environment and build the project on macOS, Windows, and Linux.

> [!IMPORTANT]
> **Architecture Update**: The legacy desktop UI (`hyperart_qtwidgets`) is now completely frozen and deprecated. Development has moved entirely to the next-generation `hyperart_qtquick` application, which provides superior functionality, performance, dynamic theming, and an identical underlying mathematical core.

---

## macOS

On macOS, you have two primary options for setting up your environment: using Homebrew (recommended) or the official Qt installer.

### Option 1: Homebrew (Recommended)

Homebrew is the easiest method since it automatically manages paths and dependencies for CMake, Ninja, and Qt.

1. **Install Dependencies**  
   Open your terminal and install the required packages:
   ```bash
   brew install cmake ninja qt
   ```

2. **Configure the Build**  
   Using CMake, configure the build directory to use Ninja and the Qt installation from Homebrew. From the root `hyperart` directory, run:
   ```bash
   cmake -G Ninja -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
   ```

3. **Build the Project**  
   ```bash
   cmake --build build
   ```

4. **Run the Application**  
   To launch the modern QtQuick app (Recommended):
   ```bash
   open build/hyperart_qtquick/HyperArt.app
   ```
   To launch the legacy QtWidgets app (Deprecated):
   ```bash
   open build/hyperart_qtwidgets/HyperArtLegacy.app
   ```

### Option 2: Qt Direct Installer

1. **Install Qt**  
   Download the [Qt Online Installer](https://www.qt.io/download-qt-installer) and install **Qt 6.8** (or the latest version) for macOS.
2. **Install CMake**  
   You can choose to install CMake either via the Qt installer or manually via [cmake.org](https://cmake.org/download/).
3. **Configure and Build**  
   Open your terminal and build the project, ensuring you point CMake to your specific Qt installation path (replace `<QT_PATH>` with your installation directory, e.g., `~/Qt/6.8.0/macos`):
   ```bash
   cmake -S . -B build -DCMAKE_PREFIX_PATH="<QT_PATH>"
   cmake --build build
   ```

---

## Windows

On Windows, the easiest way is to use the official Qt tools setup.

1. **Install Qt and Build Tools**  
   Download the [Qt Online Installer](https://www.qt.io/download-qt-installer). During installation, make sure to select:
   * **Qt 6.8** core framework (e.g., using the MSVC or MinGW compiler components).
   * **CMake** and **Ninja** (these can usually be installed as part of the Qt tools).
   
2. **Open the Developer Terminal**  
   Open the Qt-provided command prompt, which automatically sets up your environment variables (e.g., "Qt 6.8.0 (MSVC 2019 64-bit)" from your Start Menu).

3. **Configure the Build**  
   Run the following commands from the root `hyperart` folder:
   ```cmd
   cmake -G Ninja -S . -B build
   ```

4. **Build and Run**  
   ```cmd
   cmake --build build
   ```
   To launch the modern QtQuick app (Recommended):
   ```cmd
   .\build\hyperart_qtquick\HyperArt.exe
   ```
   To launch the legacy QtWidgets app (Deprecated):
   ```cmd
   .\build\hyperart_qtwidgets\HyperArtLegacy.exe
   ```

---

## Linux

On Linux, it's highly recommended to use your distribution's native package manager to install the required Qt 6 development packages.

### Ubuntu / Debian-based

1. **Install Dependencies**  
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential cmake ninja-build qt6-base-dev libgl1-mesa-dev
   ```

2. **Configure and Build**  
   From the root `hyperart` directory:
   ```bash
   cmake -G Ninja -S . -B build
   cmake --build build
   ```

3. **Run the Application**  
   To launch the modern QtQuick app (Recommended):
   ```bash
   build/hyperart_qtquick/HyperArt
   ```
   To launch the legacy QtWidgets app (Deprecated):
   ```bash
   build/hyperart_qtwidgets/HyperArtLegacy
   ```

### Fedora / Red Hat-based

1. **Install Dependencies**  
   ```bash
   sudo dnf install gcc-c++ cmake ninja-build qt6-qtbase-devel default-libGL-devel
   ```

2. **Configure and Build**  
   ```bash
   cmake -G Ninja -S . -B build
   cmake --build build
   ```

3. **Run the Application**  
   To launch the modern QtQuick app (Recommended):
   ```bash
   build/hyperart_qtquick/HyperArt
   ```
   To launch the legacy QtWidgets app (Deprecated):
   ```bash
   build/hyperart_qtwidgets/HyperArtLegacy
   ```
