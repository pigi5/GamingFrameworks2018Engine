# GamingFrameworks2018Engine
Game Engine for Dr. Fendt's Fall 2018 Gaming Frameworks

# Configuring SFML
SFML is a library used throughout our application.  
[This tutorial covers setting it up with Visual Studio](https://www.sfml-dev.org/tutorials/2.5/start-vc.php).

# Configuring yaml-cpp
We use [YAML](https://en.wikipedia.org/wiki/YAML) for data files and [yaml-cpp](https://github.com/jbeder/yaml-cpp) for our cpp implementation of a yaml interpreter.

The "How to Build" section of yaml-cpp's README.md explains how to make the yaml-cpp solution, but first -- we are using Visual Studio 2017 to build, and it doesn't seem to work well with VS2017, so you need to do a little extra work:

* Download the yaml 0.6.0 source code and unzip.  
* Go to \include\yaml-cpp\exceptions.h and change the whole preprocessor block that conditionally defines YAML_CPP_NOEXCEPT (should have a comment above it explaining that it's for compatibility with older version of Visual Studio) to just one line: `#define YAML_CPP_NOEXCEPT noexcept` 
* Go to \src\exceptions.cpp and do the same thing.  
* Now follow the rest of the "How to Build" tutorial. The cmake command should look like: `cmake -G "Visual Studio 15 2017" ..`  
* Following those instructions will create a .sln file that you can open in Visual Studio 2017.  
* Switch to the Release branch and build the solution.  
* You may get some "errors" that say something like "warning interpreted as error". You should be able to ignore them if the following is true:  
* You should now now have the file libyaml-cppmd.lib in the /Release directory.  
* You can now add the /include directory to your additional include directories, the /Release directory to your additional library directories, and libyaml-cppmd.lib to your additional dependencies in Visual Studio for the project.

# Configuring wxWidgets
We use wxWidgets to create the engine GUI elements

* Download wxWidgets [here](https://www.wxwidgets.org/downloads/).
* Download the 3.1.1 version
* Use your IDE of choice to open the relevant project in the "build" folder given by wxWidgets.
* Change to release, and change all projects to use Multi-Byte Character Set, which is located in the General section of Properties.
* Build the project
* Add the /include folder and the /include/msvc folder to your project's include directory
* Add the /lib/vc_lib folder to your project's additional library directory
