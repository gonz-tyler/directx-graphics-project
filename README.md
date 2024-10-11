#DirectX Graphics Project
##Project Overview
This project is a DirectX 11-based graphics application developed in C++ as part of a university course. The application demonstrates the rendering of various 3D models, organized into a scenegraph structure, with dynamic lighting using shaders written in HLSL (.hlsl).

##Features
- **Scenegraph-based Rendering**: Models are organized into a hierarchical scenegraph, allowing for complex transformations and object management.
- **Dynamic Lighting**: The project features dynamic lighting, including directional, point, and spotlights, calculated in real-time using custom shaders.
- **Shaders (HLSL)**: Custom vertex and pixel shaders are used to apply lighting, material properties, and other graphical effects.
- **Multiple Models**: The scene contains multiple 3D models, each with different materials, textures, and transformations.
##Technologies Used
- **DirectX 11**: The core graphics API for rendering.
- **C++**: The programming language used to implement the application.
- **HLSL (High-Level Shading Language)**: Used to write vertex and pixel shaders for lighting and other effects.
- **Visual Studio**: Used for development and debugging.
## Key Files
- **DirectXApp.cpp**: Sets up the window, initializes DirectX, and starts the rendering loop.
- **SceneGraph.cpp**: Defines the hierarchical structure used to manage models and transformations.
- **GeometricObject.cpp**: Contains definitions and functionalities for geometric objects used in the scene.
- **ModelNode.cpp**: Manages individual model nodes within the scenegraph, including transformations and rendering.
- **CubeNode.cpp**: Specifically handles the properties and behaviors of cube objects in the scene.
- **TexturedCubeNode.cpp**: Extends the `CubeNode` functionality to include texture mapping.
- **Framework.cpp**: Provides the foundational framework for DirectX initialization and rendering logic.
- **WICTextureLoader.cpp**: Implements functionality for loading textures using the Windows Imaging Component (WIC).
- **SimpleMath.cpp**: Utility functions for basic mathematical operations used throughout the project.
- **Shaders (.hlsl)**:
  - **shader.hlsl**: Applies lighting calculations to the models.
  - **textureShader.hlsl**: Applies lighting calculations and material properties to the models.
##Setup Instructions
###Prerequisites
- Visual Studio 2022 or higher with the following components:
- Desktop Development with C++
- DirectX SDK (if not already included in your Visual Studio installation)
###Build and Run
- Clone the repository:
```bash
git clone https://github.com/gonz-tyler/directx-graphics-project.git
cd directx-graphics-project
```
- Open the solution file (.sln) in Visual Studio.
- Build the project by selecting Build > Build Solution (Ctrl+Shift+B).
- Run the application by pressing F5 or selecting Debug > Start Debugging.
##Future Improvements
- Add support for more advanced lighting techniques (e.g., shadows, global illumination).
- Implement post-processing effects (e.g., bloom, depth of field).
- Add a user interface for real-time scene manipulation.
- Optimize the scenegraph structure for larger scenes.
- Add Controls
  - W/A/S/D: Move the camera.
  - Mouse: Rotate the camera.
  - Q/E: Move the camera up and down.
  - F1/F2/F3: Toggle different lighting effects (e.g., directional, point, spotlights).
Credits
University project developed by Gonzalo Escolar Tyler.
Models and texture files sourced from Dr. Wayne Rippin.
License
This project is licensed under the MIT License. See the LICENSE file for details.
