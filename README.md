# 🎨 Sprite Editor

A modern, feature-rich pixel-perfect sprite editor built with Qt C++ for creating and animating pixel art sprites with multiple frames.

## ✨ Features

### 🖌️ Drawing Tools
- **Pen Tool** - Draw individual pixels with selected color
- **Paint Bucket** - Fill contiguous areas with selected color using advanced flood fill algorithm
- **Eraser** - Remove pixels (sets them to white/background color)
- **Color Picker** - Choose colors with full RGBA support including alpha channel

### ↩️ Undo/Redo System
- **Per-Frame History** - Each frame maintains its own independent undo/redo history (up to 50 states)
- **Persistent Across Frames** - Switch between frames without losing your undo/redo state
- **Smart History** - History is preserved when navigating, cleared when deleting frames
- **Multiple Access Methods** - Buttons, menu items, and keyboard shortcuts

### 🎭 Image Manipulation
- **Horizontal Mirror** - Flip the current frame horizontally
- **Vertical Mirror** - Flip the current frame vertically
- **Undoable Operations** - All transformations can be undone

### 🎬 Animation Features
- **Multi-frame Support** - Create sprites with unlimited animation frames
- **Advanced Frame Management**:
  - Visual frame counter showing "Current / Total"
  - Add new frames (blank or copied from current)
  - Delete frames with confirmation
  - Navigate between frames with tabs
  - Drag and drop support (coming soon)
- **Live Preview** - Real-time animation preview with smooth scaling
- **Adjustable FPS** - Control animation speed from 1-60 FPS with visual slider
- **Auto-Play Controls** - Play/Pause button for animation preview

### 🎨 Canvas & UI
- **Flexible Sprite Dimensions** - Choose from 6 preset sizes:
  - 10x10 (XL pixels) - Great for large, blocky sprites
  - 16x16 (Large) - Classic retro game size
  - 25x25 (Medium) - Default balanced size
  - 40x40 (Small) - Detailed pixel art
  - 80x80 (Tiny) - High-detail sprites
  - 100x100 (Micro) - Maximum detail
- **Responsive Design** - UI scales beautifully in fullscreen mode
- **Modern Interface** - Clean, intuitive layout with color-coded sections
- **Visual Feedback** - Buttons change state based on availability

### 💾 File Management
- **Save Projects** - Save sprites in custom `.ssp` (Sprite Editor Project) format
- **Load Projects** - Load previously saved sprite projects
- **JSON-based Format** - Human-readable project files storing frame data and metadata

## ⌨️ Keyboard Shortcuts

### Drawing & Editing
- `Ctrl+Z` - Undo last action
- `Ctrl+Shift+Z` - Redo previously undone action

### Frame Management
- `Ctrl+N` - Add new frame
- `Del` - Delete current frame
- `Ctrl+Left Arrow` - Previous frame
- `Ctrl+Right Arrow` - Next frame

### File Operations
- `Ctrl+S` - Save project (via menu)
- `Ctrl+O` - Open project (via menu)

## 📋 System Requirements

- Qt 6.9 or higher (tested with Qt 6.9.0)
- C++17 compatible compiler
- macOS, Windows, or Linux
- OpenGL support for rendering

## 🚀 Installation

### Prerequisites
Make sure you have Qt development environment installed:
- Qt Creator (recommended IDE)
- Qt libraries and development tools (Qt 6.9+)
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
2. **Choose Sprite Dimensions** - Select from the dropdown in the PROJECT panel:
   - Available sizes: `10x10` to `100x100` pixels
   - Larger numbers = smaller individual pixels
3. **Click "New Sprite"** - Confirm to create a canvas with your chosen dimensions

### 🎨 Drawing Your Sprite

1. **Choose Colors** - Click the 🎨 color palette icon to open the color picker
   - Full RGBA support with alpha channel
   - Color preview updates in real-time
2. **Select a Tool** - Choose from the left toolbar:
   - 🖊️ **Pen** (default) - Click/drag to draw pixels
   - 🪣 **Paint Bucket** - Click to fill contiguous areas
   - 🧽 **Eraser** - Click/drag to erase pixels
3. **Draw on Canvas** - Click and drag on the main drawing area (center)
4. **Undo Mistakes** - Use `Ctrl+Z` to undo, `Ctrl+Shift+Z` to redo

### 🎬 Working with Frames

#### Adding Frames
1. Click the **"+ Add"** button (green) below the frame tabs
2. Choose to copy the current frame or start blank
3. New frame is automatically selected

#### Navigating Frames
- **Click frame tabs** at the bottom to switch between frames
- Use **Ctrl+Left/Right** arrow keys for quick navigation
- **Frame counter** shows your position (e.g., "2 / 5")

#### Deleting Frames
1. Select the frame you want to delete
2. Click the **"Delete"** button (red)
3. Confirm deletion (cannot delete the last frame)

### ↩️ Using Undo/Redo

**Each frame has its own undo/redo history:**
- Make changes on Frame 1 → Undo available
- Switch to Frame 2 → No undo (new frame)
- Make changes on Frame 2 → Undo available
- Switch back to Frame 1 → Original undo history still there!

**Ways to Undo/Redo:**
- **Buttons** - Blue "Undo" and "Redo" buttons in EDIT panel (left side)
- **Menu** - Edit → Undo / Redo
- **Keyboard** - `Ctrl+Z` / `Ctrl+Shift+Z`
- **Auto-disable** - Buttons gray out when no actions to undo/redo

### 🎭 Animation Preview

1. **Preview Window** - Top-right panel shows scaled preview of current frame
2. **FPS Control** - Adjust slider (1-60 FPS) to control animation speed
3. **Play Animation**:
   - Click "Play / Pause" button to start/stop
   - Animation cycles through all frames
   - Speed controlled by FPS slider
4. **Real-time Updates** - Preview updates as you draw

### 🔄 Image Manipulation

- **Mirror Horizontally** - View menu → Horizontal Mirror... (or use shortcut)
- **Mirror Vertically** - View menu → Vertical Mirror... (or use shortcut)
- **Undoable** - All transformations can be undone with `Ctrl+Z`

### 💾 Saving and Loading

1. **Save Project**:
   - File menu → Save File...
   - Choose location and filename
   - Automatically adds `.ssp` extension
   - Saves all frames, dimensions, and metadata

2. **Load Project**:
   - File menu → Load File...
   - Select a `.ssp` file
   - All frames loaded, undo history reset
   - First frame displayed

### 🖥️ Fullscreen Mode

- Enter fullscreen (OS-specific shortcut)
- **UI automatically scales** to utilize screen space
- Canvas, preview, and all controls grow proportionally
- Exit fullscreen to return to normal size

## 📄 File Format

Projects are saved in `.ssp` (Sprite Editor Project) format - a JSON file containing:

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
          {"r": 0, "g": 255, "b": 0, "a": 255}
        ]
      ]
    }
  ]
}
```

- **height/width**: Canvas dimensions (always 400x400 or 500x500)
- **frames**: Array of frame objects
- **pixels**: 2D array of RGBA color values
- Human-readable and easy to parse

## ⚙️ Technical Details

### 🏗️ Architecture
- **Model-View Architecture**: `DrawingArea` class serves as the model, `MainWindow` as the view/controller
- **Qt Signals/Slots**: Extensive use of Qt's signal-slot mechanism for component communication
- **Per-Frame State Management**: Each frame maintains independent undo/redo history using `std::map`
- **Image Processing**: Uses `QImage` for pixel manipulation and `QPixmap` for display
- **Responsive Scaling**: Dynamic geometry updates in `resizeEvent` for fullscreen support

### 📁 Key Classes
- **`MainWindow`**: Main application window, UI controller, and event coordinator
- **`DrawingArea`**: Core drawing canvas, sprite data model, and undo/redo manager
- **Tool System**: Enum-based tool selection with extensible architecture

### 🧮 Algorithms
- **Flood Fill**: Optimized Breadth-First Search (BFS) for paint bucket tool
  - Handles color matching correctly
  - Prevents filling when clicking same color
  - Scales image for performance during BFS
- **Undo/Redo**: Map-based per-frame history with 50-state limit
  - O(1) frame switching without losing history
  - Automatic cleanup on frame deletion
  - History preserved across frame navigation
- **Coordinate Mapping**: Automatic scaling between widget and image coordinates for fullscreen
- **Frame Animation**: Timer-based frame cycling with adjustable FPS

### 🎨 UI Features
- **Styled Components**: Custom Qt stylesheets for modern appearance
- **State-based Buttons**: Visual feedback (enabled/disabled/hover/pressed states)
- **Auto-connect Slots**: Qt naming convention for automatic signal connections
- **Dynamic Layouts**: Responsive positioning with scale factors

## 🎯 Project Structure

```
sprite-editor/
├── main.cpp              # Application entry point
├── mainwindow.cpp/.h     # Main window class (UI controller)
├── drawingarea.cpp/.h    # Core drawing canvas (model)
├── mainwindow.ui         # UI layout file (Qt Designer format)
├── resources.qrc         # Resource file for icons and assets
├── SpriteEditor.pro      # Qt project configuration file
└── README.md             # This file
```

## 🚀 Recent Updates

### Version 2.0 Features
- ✅ **Undo/Redo System** - Full per-frame history with 50-state limit
- ✅ **Improved Frame Management** - Visual controls, frame counter, better navigation
- ✅ **Keyboard Shortcuts** - Complete shortcut system for all operations
- ✅ **Fullscreen Scaling** - Responsive UI that adapts to window size
- ✅ **Modern UI Design** - Color-coded sections, better visual hierarchy
- ✅ **Fixed Paint Bucket** - Now works correctly on all colors
- ✅ **Smart Button States** - Auto-enable/disable based on availability
- ✅ **Removed Select Tool** - Simplified tool set (was redundant)

## 🐛 Known Issues

- macOS: May need to run `fix_agl.sh` after qmake to fix AGL framework deprecation
- First-time users should start with medium-sized sprites (25x25) for best experience

## 📝 License

[Add your license here]

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

---

**Built with ❤️ using Qt C++**
