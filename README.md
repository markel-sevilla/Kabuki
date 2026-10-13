# Kabuki
Kabuki is an OpenGL 3.0 based lightweight library written in C++20/23. The main idea is to abstract OpenGL while giving enough modularity to create new features using the rendering system.

## Features

- Modern OpenGL
- Shader management
- Vertex and Index Buffers
- Vertex Array Objects
- Texture loading
- Camera abstraction
- Deferred rendering API
- Lighting and shadows
- Decals
- PBR
- Ambient Occlussion
- Debug Rendering
- Picking with Kd Trees and GJK
- BVHs
- Header-only utilities where appropriate

## Dependencies

- OpenGL
- Vulkan
- GLFW
- GLAD
- GLM
- stb

## Building
// TODO

## Example
// TODO

## Project Structure
//TODO

## Roadmap

- [X] Window creation
- [X] Debug logger
- [X] Model loading
- [X] KShader abstraction
- [X] KMesh class
- [X] KTexture Class
- [X] KVertex Class
- [X] KMaterial Class
    - [ ] PBR materials
- [X] KPipeline system:
    -   [X] Specific KRender class to differentitate between rendering modes:
        -   []  Deferred renderer
        -   []  Fwd renderer
        -   []  Inst renderer
        -   []  RayT renderer
    -   [X] KPass system
    -   [X] KBuffer attachment
    -   [X] KAttachment system
- [ ] Gltf Mesh/Scene loading:
    -   [X] Basic loading
    -   [ ] Add multithreading 
- [ ] General lighting effects:
    - [X] Point Light 
    - [ ] Dir Light
    - [ ] Ambient Light
    - [ ] Ambient Occ
- [ ] Texture Mapping effects:
    - [ ] Decals
    - [ ] CubeMaps
    - [ ] Normal mapping
- [ ] Picking with KD-Trees and GJK
- [ ] Compute shaders
- [ ] Post-release documentation
- [ ] Unit tests:
    - [X]  Testing framework make pipeline
    - [ ]  Basic testing for classes (Continuous task)
    - [ ]
- [ ] Clean up warnings
## Contributing

Contributions are welcome!

Feel free to open an issue or submit a pull request.

## Next issue

Testing framework

## License

MIT License
