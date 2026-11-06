# 3D Game Engine in C++ — Team Plan

Nov 6, 2026 · @Stefan

## Overview

Two people build a small, modern 3D engine in C++20 together with one sample game that uses it, in roughly 6 months of part-time work. The game decides which features the engine gets: if the game doesn't need it, we don't build it.

**The engine will:**

- Run on Windows and Linux (macOS optional)
- Load 3D models and levels from glTF files made in Blender
- Render with physically based materials, lights, shadows and post-processing
- Simulate rigid-body physics and a character controller
- Play 3D positional audio
- Run gameplay code in Lua scripts, hot-reloaded without restarting
- Ship with a simple editor for placing objects and saving levels

**The engine will not (for now):** consoles or mobile, networking/multiplayer, a visual scripting system, terrain or open-world streaming, ray tracing, its own physics or audio engine.

We lean on proven open-source libraries for everything that isn't the core of the engine, so our time goes into architecture, rendering, tools and the game.

## Tech stack

C++20 with CMake, and open-source libraries for every area that isn't our core work. All of these are free and permissively licensed.

| Area | Library | Why we pick it |
| --- | --- | --- |
| Language | C++20 | Concepts, `std::span`, modules optional; supported by MSVC, GCC, Clang |
| Build & packages | [CMake](https://cmake.org) + [vcpkg](https://vcpkg.io) | One command builds everything, dependencies included |
| Window & input | [SDL3](https://wiki.libsdl.org/SDL3/FrontPage) | Cross-platform windows, keyboard, mouse, gamepads |
| Graphics API | [SDL3 GPU API](https://wiki.libsdl.org/SDL3/CategoryGPU) | One API over Vulkan, Direct3D 12 and Metal; far less code than raw Vulkan |
| Shaders | [Slang](https://shader-slang.org) | Write once, compile to SPIR-V, DXIL and Metal |
| Math | [GLM](https://github.com/g-truc/glm) | Standard graphics math types |
| Physics | [Jolt Physics](https://github.com/jrouwe/JoltPhysics) | Multithreaded, used in Horizon Forbidden West |
| Audio | [miniaudio](https://github.com/mackron/miniaudio) | Single file, 3D spatial sound |
| 3D models | [fastgltf](https://github.com/spnda/fastgltf) | Fast, modern glTF 2.0 loader |
| Textures | [stb\_image](https://github.com/nothings/stb), [KTX-Software](https://github.com/KhronosGroup/KTX-Software) | Load PNG/JPG; compressed GPU textures |
| Entity system | [EnTT](https://github.com/skypjack/entt) | Fast, widely used ECS |
| Scripting | [Lua](https://www.lua.org) + [sol2](https://github.com/ThePhD/sol2) | Gameplay code without recompiling |
| Editor & debug UI | [Dear ImGui](https://github.com/ocornut/imgui) + [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) | Panels, inspectors, move/rotate gizmos |
| Logging | [spdlog](https://github.com/gabime/spdlog) | Fast, formatted logging |
| Serialization | [nlohmann/json](https://github.com/nlohmann/json) | Human-readable level and settings files |
| Profiling | [Tracy](https://github.com/wolfpld/tracy) | Frame-by-frame CPU and GPU profiler |
| Testing | [Catch2](https://github.com/catchorg/Catch2) | Simple unit-test framework |
| GPU debugging | [RenderDoc](https://renderdoc.org) | Capture a frame and inspect every draw call |

**Graphics API decision:** start with SDL3's GPU API. Switch to raw Vulkan only if the team already knows graphics programming well and wants features SDL\_GPU doesn't offer. Its [example programs](https://github.com/TheSpydog/SDL_gpu_examples) are the best starting point.

## Architecture

The engine is one library made of layers; the game, the editor and the tests are separate programs that link it.

&#91;embedded content: engine architecture · 3 apps on 7 layers\]

Dependencies only point down: Core knows nothing about rendering, and the renderer knows nothing about gameplay. This keeps each part testable on its own and lets people work in parallel.

### Repo layout

```
engine/
  core/  platform/  render/  physics/  audio/  assets/  scene/  scripting/
editor/          # editor application
game/            # sample game that drives development
samples/         # tiny demos, one per feature
tests/           # unit tests + visual regression scenes
shaders/         # Slang shaders, compiled during the build
assets/          # models, textures, sounds (Git LFS)
tools/           # asset converters and scripts
docs/            # architecture notes and decisions
.github/workflows/  # CI
```

## Team roles

Each subsystem has one owner who designs it, reviews changes to it and keeps it working; anyone may contribute to it. One person also acts as technical lead with the final say on architecture questions.

| # | Role | Owns | First deliverable |
| --- | --- | --- | --- |
| 1 | Core & platform (tech lead) | Memory, logging, math wrappers, file system, job system, main loop, SDL3 window and input | Window opens, input events logged, frame timer |
| 2 | Render backend | GPU device, buffers, textures, pipelines, shader compilation | A coloured triangle on screen |
| 3 | Rendering features | Camera, materials, PBR lighting, shadows, post-processing, render graph | A lit, textured model |
| 4 | Asset pipeline | glTF import, texture compression, asset IDs, hot-reload of assets and shaders | Load a Blender model into memory |
| 5 | Physics & audio | Jolt integration, collision layers, character controller, miniaudio 3D sound | A box falling onto a floor |
| 6 | World & scripting | EnTT scene, transforms and hierarchy, level save/load, Lua bindings, events | Entities saved to and loaded from JSON |
| 7 | Editor & tools | ImGui editor app, scene view, inspector, gizmos, debug overlays | Inspector panel showing entity components |
| 8 | Build, QA & sample game | CMake, CI, test framework, visual tests, benchmarks, the sample game | CI building on Windows and Linux |

Rotate code reviews so every subsystem is understood by at least two people. If someone is unavailable for weeks, their reviewer takes over.

## Milestones and tasks

Five phases over about 6 months part time (about 3 months full time). Each phase ends with the whole team playing the build together and deciding what to fix next.

### Phase 1 — Foundation (weeks 1–3): a triangle on screen

- [ ] Agree on the architecture, coding standard and folder layout; write them down in the repo
- [ ] Create the Git repo with Git LFS, CMake project and vcpkg manifest
- [ ] Set up CI building on Windows and Linux for every pull request
- [ ] Open a window and read input with SDL3
- [ ] Logging, asserts and a frame timer in Core
- [ ] Render a triangle with SDL\_GPU and a compiled Slang shader
- [ ] Add Catch2 and the first math unit tests
- [ ] Integrate Tracy and Dear ImGui with an FPS overlay

### Phase 2 — Something to look at (weeks 4–8)

- [ ] Free-fly camera
- [ ] Load glTF meshes and textures from Blender exports
- [ ] Basic lighting (directional and point lights)
- [ ] EnTT scene with transform hierarchy
- [ ] Asset and shader hot-reload
- [ ] Job system for loading assets on worker threads
- [ ] First visual regression test scene in CI

### Phase 3 — Something to play (weeks 9–14)

- [ ] Jolt rigid bodies and collision shapes from meshes
- [ ] Character controller with walking and jumping
- [ ] 3D positional sound with miniaudio
- [ ] Lua scripting with hot-reload
- [ ] Save and load levels as JSON
- [ ] Sample game: walk around a level, push objects, trigger sounds

### Phase 4 — It looks good (weeks 15–22)

- [ ] PBR materials (metallic/roughness, as glTF defines them)
- [ ] Shadow maps for the main light
- [ ] HDR rendering, tone mapping and bloom
- [ ] Skeletal animation from glTF
- [ ] Editor: select, move, rotate, scale, inspector, save level
- [ ] Performance benchmark scene tracked in CI

### Phase 5 — Make a real game (week 23 onward)

- [ ] Build a complete small game level end to end
- [ ] Fix what the game exposes; profile and optimise hot spots
- [ ] Package builds that others can download and play
- [ ] Write getting-started docs for the engine

## Workflow

Small changes, merged often, always reviewed, always built by CI.

- **Version control:** Git on GitHub or GitLab. Large binary files (models, textures, audio) go through Git LFS.
- **Branches:** short-lived feature branches off `main`, merged within a few days. `main` must always build and run.
- **Pull requests:** every change goes through a PR with at least one reviewer; the subsystem owner reviews changes to their area.
- **CI checks before merge:** builds on Windows and Linux, unit tests, visual tests, clang-format and sanitizer builds. Red CI blocks the merge.
- **Code style:** a shared `.clang-format` and `.clang-tidy` in the repo, enforced by CI so nobody argues about formatting.
- **Planning:** a GitHub Projects board with columns To do, In progress, In review, Done. Two-week sprints.
- **Meetings:** a 30-minute weekly sync, plus a playtest at the end of every phase.
- **Communication:** one chat server (Discord or Slack) with a channel per subsystem.
- **Documentation:** architecture decisions written as short notes in a `docs/` folder, so the reasoning isn't lost.

## Testing strategy

Seven layers of testing, from automatic checks on every pull request to the whole team playing the game.

| Layer | What it checks | When it runs | Tools |
| --- | --- | --- | --- |
| Unit tests | Math, collision queries, serialization round-trips, asset loader with broken files | Every PR | Catch2 |
| Sanitizers | Memory errors, undefined behaviour, data races | Every PR | AddressSanitizer, UBSan, ThreadSanitizer |
| Graphics validation | Misuse of the GPU API | Every debug run | Vulkan / D3D12 validation layers via SDL\_GPU debug mode |
| Visual regression | Test scenes still render the same | Every PR | Headless render + screenshot compare against approved images |
| Performance | Frame time and memory of a stress scene | Nightly | Benchmark scene, Tracy captures |
| Sample game & demos | Features actually work together | Daily use | `game/` and `samples/` |
| Playtests | Fun, bugs, usability of the editor | End of each phase | The whole team |

**Visual regression in practice:** keep a small set of scenes (a lit cube, a shadow scene, a grid of PBR spheres, an animated character). CI renders each one to an image and compares it with the approved image within a small tolerance. If a change differs, a human looks at both images and either fixes the bug or approves the new image.

**Bug tracking:** every bug found gets an issue with steps to reproduce; a fixed bug gets a test so it can't come back.

## Learning resources

Everyone reads the "Everyone" list first; then each owner works through their own area.

| Area | Resource | Type | Who |
| --- | --- | --- | --- |
| Everyone | [Game Engine Architecture](https://www.gameenginebook.com) — Jason Gregory | Book | All 8 |
| Everyone | [Game Programming Patterns](https://gameprogrammingpatterns.com) — Robert Nystrom | Free online book | All 8 |
| Everyone | [learncpp.com](https://www.learncpp.com) | Free tutorial | Anyone rusty in C++ |
| Everyone | [Pro Git](https://git-scm.com/book/en/v2) | Free book | Anyone new to Git |
| Build & CI | [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/) | Free online book | 8 |
| Build & CI | [GitHub Actions docs](https://docs.github.com/en/actions) | Docs | 8 |
| Math | [3D Math Primer for Graphics and Game Development](https://gamemath.com) | Free online book | 1, 3, 5 |
| Graphics basics | [LearnOpenGL](https://learnopengl.com) | Free tutorial | 2, 3 (concepts carry over to any API) |
| SDL3 GPU | [SDL3 GPU API reference](https://wiki.libsdl.org/SDL3/CategoryGPU) and [SDL\_gpu\_examples](https://github.com/TheSpydog/SDL_gpu_examples) | Docs + code | 2 |
| Vulkan (if chosen) | [vkguide.dev](https://vkguide.dev) | Free tutorial | 2 |
| Vulkan (if chosen) | [Khronos Vulkan Guide](https://docs.vulkan.org/guide/latest/index.html) | Docs | 2 |
| Shaders | [Slang documentation](https://shader-slang.org/docs/) | Docs | 2, 3 |
| Rendering | [Physically Based Rendering (PBR book)](https://pbr-book.org) | Free online book | 3 |
| Rendering | [Real-Time Rendering resources](https://www.realtimerendering.com) | Book + link collection | 3 |
| Assets | [glTF 2.0 specification](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html) | Spec | 4 |
| Assets | [fastgltf docs](https://fastgltf.readthedocs.io/) | Docs | 4 |
| Physics | [Jolt Physics docs](https://jrouwe.github.io/JoltPhysics/) and its HelloWorld sample | Docs + code | 5 |
| Audio | [miniaudio manual](https://miniaud.io/docs/manual/index.html) | Docs | 5 |
| ECS | [EnTT wiki](https://github.com/skypjack/entt/wiki) | Docs | 6 |
| Scripting | [Programming in Lua](https://www.lua.org/pil/contents.html) and [sol2 docs](https://sol2.readthedocs.io) | Book + docs | 6 |
| Editor | [Dear ImGui wiki](https://github.com/ocornut/imgui/wiki) | Docs | 7 |
| Profiling | [Tracy manual](https://github.com/wolfpld/tracy/releases) (PDF with each release) | Docs | All |
| GPU debugging | [RenderDoc docs](https://renderdoc.org/docs/index.html) | Docs | 2, 3 |
| Inspiration | [The Cherno's Game Engine series](https://www.youtube.com/@TheCherno) | Video series | All |
| Inspiration | [Handmade Hero](https://handmadehero.org) | Video series | 1, 2 |

## Pitfalls and first week

Most student and hobby engines fail from scope, not from hard code.

**Pitfalls to avoid:**

- **Scope creep:** check every new feature against the "will not" list in the Overview.
- **Features nobody uses:** build only what the sample game needs right now.
- **Rewriting libraries:** don't write your own physics, audio or image loader.
- **Big-bang merges:** merge small changes every few days, not one huge branch at the end.
- **One-person knowledge:** if only one person understands a system, the project stops when they're busy.
- **Premature optimisation:** make it work, then measure with Tracy, then make it fast.
- **Over-abstracting the renderer:** support one graphics API well before thinking about a second.

**First-week checklist:**

- [ ] Everyone has C++20 compiler, CMake, Git, Git LFS, RenderDoc installed
- [ ] Repo created, everyone can clone, build and run an empty window
- [ ] Roles agreed and written into this doc
- [ ] Coding standard and `.clang-format` agreed
- [ ] Task board set up with the Phase 1 tasks
- [ ] Chat server and weekly meeting time set
- [ ] Everyone starts reading Game Programming Patterns and their area's resources
