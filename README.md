# Getting the project running
This readme currently assumes you're using a 64-bit Windows machine.

## Vulkan API
To install the Vulkan API on your machine, go to https://vulkan.lunarg.com/ and download version `1.2.162.1` for your machine.

## GLFW
Download the 64-bit binaries from https://www.glfw.org/download.html and unzip them in the folder: `Visual Studio 2019\Libraries\`
It's also recommended to rename the root folder to just "glfw", but this is not necessary.

## GLM
Download the latest version of GLM here: https://github.com/g-truc/glm/releases
Also unzip this folder in `Visual Studio 2019\Libraries\` 

## Visual Studio
Now we need to to tell Visual Studio to include these libraries.
Go to `Project` -> `Vulkan Properties`
Make sure you you have selected `All Configurations`.

Go to `C/C++` -> `General` and edit `Additional Include Directories` 
Make sure you include an entry for where `Visual Studio 2019\Libraries\glm` and `Visual Studio 2019\Libraries\glfw\include` are located.
Also add the library folder in the project structure. This will include additional libraries that need to be imported.

Now go to `Linker` -> `General` and edit `Additional Library Directories`
Here you need to make sure vulkan and glfw are included properly.
If you installed the correct version in the default location, Vulkan should already be done.
For glfw you will have to include an entry for your `Visual Studio 2019\Libraries\glfw\lib-vc2019`

Finally, while optional, it's recommended to install the extension `GLSL Language Integration`.
This will mostly be needed for syntax higlighting, but is also usefull for a few other things.

# Credits
Original tutorial used: https://vulkan-tutorial.com
