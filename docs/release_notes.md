# HyperArt v1.1.0 Release Notes

Welcome to **HyperArt 1.1.0**! 

This release introduces native multi-window support and significant performance optimizations, allowing you to explore and compare multiple hyperbolic designs simultaneously with ease.

### Key Features & Enhancements
- **Multi-Window Architecture**: We've completely refactored the UI to support multiple concurrent windows. Use `File > New Window` or `File > Open` to spawn independent design canvases.
- **High-Efficiency Rendering**: To support multiple simultaneous animations, we've optimized the C++ rendering engine. Key improvements include frame-rate stabilization (30 FPS) and viewport constant caching, ensuring smooth performance even with thousands of elements across multiple windows.

---

# HyperArt v1.0.0 Release Notes

Welcome to **HyperArt 1.0.0**! 

This release represents a massive milestone: a complete structural modernization of the user interface from legacy Qt Widgets to a high-performance **Qt Quick (QML)** architecture powered by Qt 6.8 LTS. We have achieved absolute feature parity with the legacy app while unlocking dramatically improved performance and a sleek, responsive interface.

### Supported features
- **High-Performance Rendering**: The Poincare disk math engine is now natively integrated into a hardware-accelerated QML surface using Framebuffer Object (FBO) caching. This guarantees flawlessly smooth window resizing for complex geometries.
- **Topological Timeline Animation**: Resurrected the algorithmic pattern playback! You can scrub forward or backward step-by-step, or simply hit **Play** to visualize exactly how hyperbolic tessellations mathematically grow from the center outward.
- **Dynamic Layer Controls**: A new interactive right-side drawer allows you to surgically toggle visibility for individual concentric geometry layers. 
- **Native OS Integration**: Keyboard actions now map to standard global menu shortcuts (`Ctrl+S`, `Space`, `Left/Right` arrow keys). Furthermore, the application now dynamically responds to your system's `Light Mode` and `Dark Mode` OS themes!
- **Raster Image Export**: A newly bridged `Save As...` native file dialog handles exporting your custom viewport rendering directly to high-resolution `.png` or `.jpeg` files.

---

### Builds
A pre-compiled **macOS (.app)** build is provided in this release for immediate playback! 
* _Security Note for Mac Users_: Because this is an unsigned version from an unregistered developer, macOS Gatekeeper may warn you upon opening it. To bypass this safely, simply `Control-click` (or right-click) the application icon and select **Open** to add it to your trusted apps.

*For instructions on compiling HyperArt seamlessly across **Linux**, **Windows**, or from source, please consult our thoroughly documented [INSTALL.md](INSTALL.md).*

---

### Using the app
- Download our repository's [designs](https://github.com/ajitomatix/hyperart/tree/main/designs) directory and save it locally (e.g., in your Downloads folder or directly alongside the binary).
- You can open any native `.had` (HyperArt Design) configuration file from this directory using the primary `File > Open` menubar dialog.
- Once loaded, use the main window playback controls to scrub through the pattern visualization, or click `Toggle Layers` to examine the internal rings!
