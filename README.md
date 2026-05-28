# LearnOpenGL - Zen C Implementation

> **Purpose:** This repository is for **educational and learning purposes only**. The goal is to study and implement modern OpenGL concepts using the Zen C programming language, following the [LearnOpenGL](https://learnopengl.com/) tutorial series.

A collection of LearnOpenGL tutorials and examples implemented in **Zen C** programming language.

## Project Structure

```
├── src/                    # Source code
│   ├── main.zc            # Main entry point
│   ├── utils/             # Utility functions
│   └── 1-getting-started/ # Tutorial implementations
├── libs/                   # External libraries
│   ├── glad/              # OpenGL loader
│   ├── glfw/              # Window management
│   ├── glm/               # Mathematics library
│   └── stb_image/         # Image loading
├── shaders/                # GLSL shader files
├── textures/               # Texture assets
└── build.ps1              # Build script (Windows PowerShell)
```

## Prerequisites

- **Zen C Language**: [Zen C GitHub Repository](https://github.com/zen-c/zen)
- CMake (version 3.10 or higher)
- OpenGL compatible graphics driver
- A C compiler (GCC, Clang, or MSVC)

**Note:** This project has only been tested on **Windows**. It should work on Linux and macOS as well, but these platforms have not been tested yet.

## Building

### Windows (PowerShell)

```powershell
.\build.ps1
```

This will:
1. Create a `build` directory
2. Copy shaders and textures to the build folder
3. Compile the source code using Zen C
4. Output the executable to `build/app.exe`

### Manual Build

The build configuration is specified in the source file headers. To build manually:

```bash
zc build ./src/main.zc
```

Make sure to copy the `shaders` and `textures` folders to the `build` directory before running.

## Running

After building, run the executable:

```bash
cd ./build
app.exe
```

## Available Tutorials

The project includes implementations of various LearnOpenGL tutorials. To switch between tutorials, uncomment the corresponding import in `src/main.zc`:

- **Hello Window** - Basic window creation and clearing
- **Hello Triangle** - Rendering your first triangle
- **Shaders** - GLSL shader programs and uniforms
- **Textures** - Loading and applying textures
- **Transformations** - 3D transformations with matrices
- **Coordinate Systems** - 3D coordinate systems and depth testing

## Progress Checklist

Track your progress through the LearnOpenGL tutorial series:

<details>
<summary><strong>📚 Getting Started</strong> (7/7)</summary>

- [x] Hello Window
- [x] Hello Triangle
- [x] Shaders
- [x] Textures
- [x] Transformations
- [x] Coordinate Systems
- [x] Camera

</details>

<details>
<summary><strong>💡 Lighting</strong> (6/6)</summary>

- [x] Colors
- [ ] Basic Lighting
- [ ] Materials
- [ ] Lighting Maps
- [ ] Light Casters
- [ ] Multiple Lights

</details>

<details>
<summary><strong>🎭 Model Loading</strong> (3/3)</summary>

- [ ] Assimp
- [ ] Mesh
- [ ] Model

</details>

<details>
<summary><strong>🚀 Advanced OpenGL</strong> (6/6)</summary>

- [ ] Depth Testing
- [ ] Stencil Testing
- [ ] Blending
- [ ] Framebuffers
- [ ] Cubemaps
- [ ] Advanced Data

</details>

<details>
<summary><strong>✨ Advanced Lighting</strong> (10/10)</summary>

- [ ] Advanced Lighting
- [ ] Shadow Mapping
- [ ] Point Shadows
- [ ] Normal Mapping
- [ ] Parallax Mapping
- [ ] HDR
- [ ] Bloom
- [ ] Deferred Shading
- [ ] SSAO
- [ ] PBR

</details>

<details>
<summary><strong>🛠️ In Practice</strong> (6/6)</summary>

- [ ] Debugging
- [ ] Text Rendering
- [ ] Instancing
- [ ] Geometry Shader
- [ ] Variable Refresh Rate
- [ ] ARB Bindless Texture

</details>

## Controls

Standard OpenGL controls apply depending on the tutorial:
- **ESC** or close window to exit
- Tutorial-specific controls are documented in individual source files

## Dependencies

- [GLFW](https://www.glfw.org/) - Window and input management
- [GLAD](https://glad.dav1d.de/) - OpenGL function loader
- [GLM](https://github.com/g-truc/glm) - OpenGL Mathematics library
- [stb_image](https://github.com/nothings/stb) - Image loading library

## License

This project follows the LearnOpenGL tutorial series. Check individual source files for specific licensing information.

## Resources

- [LearnOpenGL](https://learnopengl.com/) - Original tutorial series
- [Zen C Documentation](https://zenc.dev/) - Zen C language reference
