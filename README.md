# HyperArt
HyperArt is a program to view and create repeating hyperbolic patterns (tessalations/tilings) using [algorithms](https://www.d.umn.edu/~ddunham/) developed by [Dr Douglas Dunham](https://scse.d.umn.edu/faculty-staff/douglas-dunham). It generates hyperbolic tessalations projected onto a Poincaré disk. 

The end result are patterns that are similar to those created by the venerable Dutch artist M. C. Escher. 

While this may not have any immediate commercial application, it is very interesting from a mathematical and artistic viewpoint.

# What patterns can this software generate? 
HyperArt has a HyperArt Designs (.had) file which is in XML format. While a HyperArt Designer is sorely needed, many .had files are included in the 'designs' directory included in this repository. These jpg and png files were generated by HyperArt from the corresponding .had files. 

Here are a few samples... 
### P104RF66
![p104rf66](docs/exported_images/p104rf66.png)

### Circle Limit 1
![cl1](docs/exported_images/cl1.png)

### Circle Limit 2
![cl2](docs/exported_images/cl2.png)

### Circle Limit 3
![cl3](docs/exported_images/cl3.png)

### Circle Limit 4
![cl4](docs/exported_images/cl4.png)

### Leaf
![leaf](docs/exported_images/leaf.png)

### F-star 54
![fstar54](docs/exported_images/fstar54.png)

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
