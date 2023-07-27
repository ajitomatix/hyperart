# HyperArt
HyperArt is a program to view and create repeating hyperbolic patterns (tessalations/tilings) using [algorithms](https://www.d.umn.edu/~ddunham/) developed by [Dr Douglas Dunham](https://scse.d.umn.edu/faculty-staff/douglas-dunham). It generates hyperbolic tessalations projected onto a Poincaré disk. 

The end result are patterns that are similar to those created by the venerable Dutch artist M. C. Escher. 

While this may not have any immediate commercial application, it is very interesting from a mathematical and artistic viewpoint. 

## History
### 2005
The original Qt3 version was published by me on [sourceforge](https://hyperart.sourceforge.net). That CVS(!) repository still lives, but is now in readonly mode. 

### 2021-2022
My good friend Dr. Josha Jacobs ([joshicola](https://github.com/joshicola) on Github), ported it to Qt5 and [migrated it over to Github](https://github.com/joshicola/hyperart)!

### 2023
In this latest reincarnation, I have ported it to **Qt6.5** with following notable changes:
* Using the CMake build system instead of QMake (Qt .pro files).
* Removed document/view system based on unmaintained QDocument classes.
* Made the diagrams scale with window resizes. 
* Fixed export as jpeg/png image.

Many things aren't working or need to improve:
* C++ code is a mix of old and new (C++17) styles. e.g. Could use 'auto' in a lot more places.
* Zoom in/out/fit functionality is removed for now as the diagrams now scale with viewport changes.
* Animation functinality (stepping through layer generation) is disabled currently. 
