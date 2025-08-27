# 🎨 Sprite Editor

A pixel-perfect sprite editor built with Qt C++ that allows you to create and animate pixel art sprites with multiple frames.

## ✨ Features

### 🖌️ Drawing Tools
- **Pen Tool** - Draw individual pixels with selected color
- **Paint Bucket** - Fill contiguous areas with selected color using flood fill algorithm
- **Eraser** - Remove pixels (sets them to white/background color)
- **Select Tool** - Select contiguous areas of the same color

### 🎭 Image Manipulation
- **Horizontal Mirror** - Flip the current frame horizontally
- **Vertical Mirror** - Flip the current frame vertically
- **Color Picker** - Choose colors with full RGBA support including alpha channel

### 🎬 Animation Features
- **Multi-frame Support** - Create sprites with multiple animation frames
- **Frame Management** - Add, delete, and navigate between frames
- **Live Preview** - Real-time animation preview at scaled size
- **True Size Preview** - View animation at actual pixel dimensions
- **Adjustable FPS** - Control animation speed from 1-60 FPS

### 💾 File Management
- **Save Projects** - Save sprites in custom `.ssp` (Sprite Editor Project) format
- **Load Projects** - Load previously saved sprite projects
- **JSON-based Format** - Human-readable project files storing frame data and metadata

## 📋 System Requirements

- Qt 5.x or higher (tested with Qt 6.x)
- C++17 compatible compiler
- CMake or qmake build system

## 🚀 Installation

### Prerequisites
Make sure you have Qt development environment installed:
- Qt Creator (recommended IDE)
- Qt libraries and development tools
- C++ compiler (GCC, Clang, or MSVC)

### Building from Source

1. **Clone the repository**
   ```bash
   git clone <your-repo-url>
   cd sprite-editor
   ```

2. **Open in Qt Creator**
   ```bash
   # Using Qt Creator GUI
   # File -> Open File or Project -> Select SpriteEditor.pro
   ```

3. **Or build using qmake from command line**
   ```bash
   qmake SpriteEditor.pro
   make
   ```

4. **Run the executable**
   ```bash
   ./SpriteEditor
   ```

## 🎯 Usage

### Getting Started

1. **Launch the Application** - Run the SpriteEditor executable
2. **Choose Sprite Dimensions** - On startup, select your desired sprite size:
   - Available sizes: `10x10`, `16x16`, `25x25`, `40x40`, `80x80`, `100x100` pixels
   - This determines the canvas resolution for your sprite

3. **Select a Drawing Tool** - Choose from the toolbar on the left:
   - 🖊️ Pen (default) - for precise pixel drawing
   - 🪣 Paint Bucket - for filling areas
   - 🧽 Eraser - for removing pixels
   - 📐 Select Tool - for area selection

### 🎨 Drawing Your Sprite

1. **Choose Colors** - Click the 🎨 color palette icon to open the color picker
2. **Draw on Canvas** - Click and drag on the main drawing area
3. **Use Tools**:
   - **🖊️ Pen**: Click/drag to draw individual pixels
   - **🪣 Paint Bucket**: Click on an area to flood fill with current color
   - **🧽 Eraser**: Click/drag to erase pixels (makes them white)
   - **📐 Select**: Click to select contiguous areas of the same color

### 🎬 Working with Frames

1. **Add New Frame** - Click the "➕ Add Frame" button in the frame toolbar
   - Choose to copy the current frame or start with a blank frame
2. **Navigate Frames** - Click on frame tabs at the bottom to switch between frames
3. **Delete Frame** - Select a frame and click "❌ Delete Frame" (confirmation required)

### 🎭 Animation Preview

1. **Preview Window** - The top-right panel shows a scaled preview of your sprite
2. **True Size Preview** - Below that shows the sprite at actual pixel dimensions
3. **Control Animation**:
   - Click "▶️ play/pause" to start/stop animation preview
   - Adjust FPS slider (1-60 FPS) to control animation speed
4. **Frame Navigation** - Use the frame tabs to edit individual frames

### 🔄 Image Manipulation

- **↔️ Mirror Horizontally** - View menu → Horizontal Mirror
- **↕️ Mirror Vertically** - View menu → Vertical Mirror
- These operations affect the currently active frame

### 💾 Saving and Loading

1. **Save Project**:
   - File menu → Save File
   - Choose location and filename (automatically adds `.ssp` extension)
   - Saves all frames and project metadata in JSON format

2. **Load Project**:
   - File menu → Load File
   - Select a `.ssp` file to load
   - All frames will be loaded and the first frame will be displayed

## 📄 File Format

Projects are saved in `.ssp` (Sprite Editor Project) format, which is a JSON file containing:

```json
{
  "height": 400,
  "width": 400,
  "frames": [
    {
      "frame_Index": 0,
      "pixels": [
        [
          {"r": 255, "g": 0, "b": 0, "a": 255},
          ...
        ]
      ]
    }
  ]
}
```

Each pixel stores RGBA color values, making it easy to export or convert to other formats.

## ⚙️ Technical Details

### 🏗️ Architecture
- **Model-View Architecture**: `DrawingArea` class serves as the model, `MainWindow` as the view/controller
- **Qt Signals/Slots**: Extensive use of Qt's signal-slot mechanism for component communication
- **Image Processing**: Uses `QImage` for pixel manipulation and `QPixmap` for display

### 📁 Key Classes
- **`MainWindow`**: Main application window and UI controller
- **`DrawingArea`**: Core drawing canvas and sprite data model
- **Custom Tools**: Enum-based tool system with extensible architecture

### 🧮 Algorithms
- **Flood Fill**: Breadth-First Search (BFS) algorithm for paint bucket and select tools
- **Pixel Grid**: Automatic pixel sizing based on canvas dimensions
- **Frame Animation**: Timer-based frame cycling for preview

## 🚧 Future Enhancements

- [ ] Export to common image formats (PNG, GIF, etc.)
- [ ] Undo/redo system
- [ ] More drawing tools (line, rectangle, circle)
- [ ] Layer support
- [ ] Onion skinning for animation
- [ ] Keyboard shortcuts
- [ ] Customizable grid overlay

---

### 🎯 Project Structure
```
sprite-editor/
├── main.cpp              # Application entry point
├── mainwindow.cpp/.h     # Main window class
├── drawingarea.cpp/.h    # Core drawing canvas
├── mainwindow.ui         # UI layout file
├── resources.qrc         # Resource file for icons
├── SpriteEditor.pro      # Qt project file
└── README.md            # This file
```
