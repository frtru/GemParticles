# GemParticles - Particles sandbox

This is a basic core-profile compliant OpenGL particle sandbox I used to create various visual effects and learning 3D concepts.

## Updates and important mentions:
* [2021/08/04] I decided to stop using this code or maintaining it. It was created with the intention of learning and not building a reusable framework for particles.
* [2018/11/05] None of the following stats are using multi-threading. I might implement threaded stages in the system's pipeline. WIP
* [2018/11/05] I'm currently working on GPU particles with Curl noise...
* Big thanks to [Mr. Lariviere](https://github.com/larivierec) for the help!
 
## Description

The code itself is pretty simple and follows a templated data-oriented design. The ParticlesSystem class is type-bound to the particle structure. It also contains a renderer, dynamics and emitters and a particle pool. For each experiment that I want to make I create a new cmake library project that is built in the GemParticlesApp executable project and by passing the project name as an argument to the executable you can select which experiment you want to load. 
 
## Highlights 
 
Here are some highlights that I managed to create with this system. Please note that, at the time of taking those screenshots, I had an i5-3570k CPU and Geforce GTX 660 GPU.

| Lit particles *or* particles with 200 dynamic CPU lights | 500000 CPU particles attracted toward a location with hot and cold colors | and some rain... (1 million CPU particles) |
|----------------------------------------------|---------------------------------------------------|------------------------------------------------------|
| ![](gifs_and_screenshots/lit_particles.gif)  | ![](gifs_and_screenshots/attractor_2_cropped.gif) | ![](gifs_and_screenshots/heavy_rain_cropped_big.gif) |

## Launching or installing the project
The project uses CMake files for portability and the [Conan 2 package manager](https://conan.io) to manage the dependencies. On Linux (verified on Ubuntu 24.04 with gcc 13 and CMake 3.28):
1. `scripts/bootstrap.sh` installs Conan into `.venv` if needed and generates the `conan-release`, `conan-debug` and `conan-relwithdebinfo` CMake presets (dependencies are always built in Release).
2. `cmake --preset conan-release && cmake --build --preset conan-release` (or `conan-debug` / `conan-relwithdebinfo`).
3. `scripts/run.sh lit_particles` launches a project (`attractor`, `light`, `lit_particles`, `rain` or `skybox`; the first one when omitted).
   * `--nvidia` renders on a discrete NVIDIA GPU through PRIME offload, `--cfg Debug` runs another build.
   * `N` switches to the next project while the app runs.
   * `src/main/config.json` (copied next to the executable) holds the app settings (graphics backend, window, default project) and lists the app's components: `DebugOverlay`, `NextProjectKey`, `ScriptedRun`.
   * `--config other.json` runs with another config, e.g. a scripted run: `{ "type": "ScriptedRun", "frames": 300, "screenshot": "out.png" }` exits after 300 frames and saves the last one.
   * Debug builds create a GL debug context; driver errors and warnings go to the log window and stderr.

## External libraries
I'm using various external libraries in this project which are listed in the file conanfile.txt 
* ImGui
* GLM
* Glew
* GLFW
* STB
* OpenGL

## Utilities
I pretty much built this from scratch looking up efficient design patterns and tutorials. So you'll find a handful of utilities that I built to help me create stuff more easily. Most of them are named as *modules*.
* Light module
* Shader module
* Texture module
* Timer

And last but not least, some wrapper utilities I built around ImGui examples. This includes
* A property editor
* A logger

## Extending the project
If you want to create your own project using this "*engine*" or the project structure you can
1. Add the project to the list of sub-directories in [projects cmakelists.txt file](https://github.com/frtru/GemParticles/blob/master/src/projects/cmakelists.txt) 
2. Register the project in the [project dictionary](https://github.com/frtru/GemParticles/blob/master/src/projects/project_dictionary.cc) (if you use the REGISTER_STD_PROJECT you'll have to define the `init`, `run` and `terminate` functions in your project)
3. Add your own cmakelists.txt and sub-directory in the projects/ folder
4. Refer to the other projects' blueprints or *_project.cc files 
5. Run the GemParticlesApp (main) executable with the name of your project passed as an argument. 
