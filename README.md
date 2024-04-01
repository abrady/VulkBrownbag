# Setup

* install Vulkan <https://vulkan.lunarg.com/>
* check this repo out
* `git submodule init` to get vcpkg
* run `.\vcpkg\bootstrap-vcpkg.bat`
* (optional) run `.\vcpkg\vcpkg.exe install`
* (optional) vcpkg\vcpkg.exe integrate install (I don't think this is necessary with setting it in the CmakeLists.txt)

# Running Samples

* Open the root directory in vs code
* Open command palette and run > `CMake: Configure`
  * pick appropriate compiler, amd64 probably, latest visual studio
  * if you can't find any, try opening VSCode from a Visual Studio command prompt.
    * `Start > Developer Command Prompt` will usually show you some options
