# Project Solution Layout

The Project solution layout is designed to keep source files, support files and build files organized and separated. Its basic concepts date back to 1995 (small portions of the Util library also date back to that time.) The underlying design principle has always been to ensure that unit testing is easier *and* uses the same compilation as the application.

By 2013, it became apparent that most of an application's code could be built and tested as libraries. Save for two changes, the current design was used in 2013 for a cross-platform Qt application. The two big changes since then were to move all project directories under a single Src directory and to separate the unit tests into its own directory tree.

> A large project I worked on in 2015 reinforced this view; in order to work with the Visual Studio testing framework, most of the application was recompiled as a DLL. While this worked, there was a continued concern that some code paths were different between the DLL and the application (until I fixed it, the projects had mismatched compiler settings, so the code *was* different between the two.) It also doubled the time of an already very long build.

> One advantage of having a single solution which builds so many projects is that if a library changes in a way which would render a high level project uncompilable, this is caught immediately.

Typically, there are three main "controlled" directories at the base level, though more may be added as needed: **Docs**, **Install**, **Src** and **ThirdParty**. There are three temporary directories: **Build**, **.vs**, **Debug** and, if used, the **.git** directory.

IDEs and source control may create additional temporary directories.

## Docs

Contains documents related to the solution and/or application. For example, it may contain a help project. This is kept separate from **Src** since a writer may need access to it and not the other directories. It also keeps directories, especially the base directory, from being littered with files.

## Install

Contains installation scripts and files. Since the installation for projects varies widely, the contents of this directory haven't been standardized. Currently, NSIS is planned to be used for installers.

## Build

Created by Visual Studio when doing a build. It contains all intermediate build objects, final libraries and executables (including DLLs). To do a clean build, simply delete this directory!

> With cross-platform builds, this directory is also created by CMake, which tends to create even more temporary files and is where the generated make files reside.

Under Build there will be a list of target platforms with Debug and Release directories under each:

**Build/\<target\>/Debug**

**Build/\<target\>/Release**

The final executables, including DLLs unique to the project, will be in a **bin** directory underneath each of these. The project will copy stored DLLs and *some* resources into this same directory. In some cases, to debug from this folder, the active directory needs to be one containing config and/or registration files and possibly large assets. The application should make accommodation for these being in different folders.

One unresolved issue is dealing with large files which may be required for testing. Typically, the unit tests generate the files needed and writes them to temporary folders. However, in some circumstances, this may not be practical or possible. If this happens, for now store the original files under the associated test project directory and then have Visual Studio copy them to **bin/test**. 

> For Windows, target is Win64.

> At one company, we decided to put a **TestFiles** directory in the base directory (parallel to Src.) If this solution is used, the test startup code would be modified to find and set this folder as a global variable.

## .vs

Created by Visual Studio and contains Intellisense and other files. This is *not* saved in source control and can, and should , be periodically deleted.

> Visual Studio Intellisence sometimes stops working, especially when doing heavy refactoring. Closing and restarting Visual Studio often fixes the problem. If not, close the solution, navigate into the .vs directory and then down to a folder containing .suo and .db files. Delete *all* but the .suo file. (If you don't want to preserve session settings and some user options, such as breakpoints, delete the entire .vs folder.)

## .git

Where Git keeps all its information. It should not be deleted.

## Debug

Visual Studio has a longstanding quirk where it sometimes automatically creates a Debug or Release directory in the same directory as the solution. This directory will be empty and can be safely deleted. *If anything does show up in this directory, this is likely a project paths bug which needs to be resolved.*

## Src

As its name signifies, the **Src** directory contains the solution's source code. In turn, it contains the sub-directories: **Apps**, **Exp**, **Libs**, **Tests** and **Utils**.

> For the applications that use MFC, if the appropriate runtimes are not installed on a system, the VC_redist.x64.exe installer must be run. (Once installed, it will be updated with Windows updates.)

If an application uses Qt, the Qt dlls will be included with the application installer.

### Src/Apps

Contains applications which will be "shipped" or used internally. Currently the contents are:

**Src/Apps/CoreFit** is project started to support my daughter's business and can be used as the basis for a Qt application. This uses an outdated programming style and would need to  be updated.

### Src/Exp

Contains experimental utilities. This allows code experiments to be done with all the solution's libraries available. This may mean trying out new C++ concepts, trying to determine how a specific function works in isolation or when developing a new class or function (a form of testing driven development; if the interface isn't settled, writing a formal unit test too early can be frustrating.) Code in this directory should *not* be code reviewed unless by specific request. This directory currently contains **AppExp**, **QtExp**, **Timed** and **UtilExp**.

> Unless otherwise specified, the truly experimental portions of these projects may change at any time. If an experiment needs to be revisited, one option is to make a copy of the file with a descriptive name. 

**Src/Exp/AppExp** is a Qt app which links to AppLib. AppLib may require extra administration and setup.

**Src/Exp/QtExp** is a generic Qt app. Its intent is to test basic Qt concepts. Do NOT link this with AppLib.

**Src/Exp/Timed** is a console app used to benchmark code snippets.

**Src/Exp/UtilExp** is a console app used for general (non-Qt) testing.

### Src/Libs

Contains all source code that can be unit tested without user intervention, and most third party libraries and/or source. This directory contains two key libraries, **Util** and **CoreLib**.

**Src/Libs/Util** is a general purpose, low level library which, with few exceptions, depends only on C++, the CRT and the OS's SDK (for Windows, the Windows SDK.) The main exceptions are classes to help with Json, XML, compression and encryption. It must be testable with a console application without any user-interactive elements.

**Src/Libs/CoreLib** contains as much code as is practicable from the **CoreFit** application. The goal, again, is that it contains non-interactive, unit testable code.

> Originally, this was intended to be a rather thin layer. Turns out it can contain most of the core logic of an application. With dependency injection, even what would otherwise be user-interactive code can be unit tested.

Due to a design limitation of Visual Studio, the created object directories do not mirror the directory layout of these libraries. Each file in an individual library must have a unique name.

> By experience, every source file in a solution should have a unique name, else Visual Studio Intellisense may bring up the wrong file.

> Note: Files with the following header are in the public domain. This header allows the free version of PVS-Studio to do static analysis on these files:
> // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
> // PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

### Src/Tests

Contains unit tests. The unit tests currently use the *Catch 2* test framework (https://github.com/catchorg/Catch2). It contains a library called *TestHelper*, which contains some helpful functionality, especially temporary directory and file classes. (Beneath *TestHelper*, there is a *TestHelperTest* project, which is used to test *TestHelper*, but is typically only run when *TestHelper* changes.)

The other directories/projects under **Tests** mirror the non-3rd party directories under **Libs**. In turn each of these directories mirrors the layout of the associated library's directory structure. Each .h file in the library should have a test file using the format \<stem>.tests.cpp. 

> Some unit tests cannot get full coverage of the class and/or function being tested since verifying the exact output is problematic or impossible. In these cases, the tests can still ensure the class and/or function doesn't crash. For example. `SysInfo/GetSystemMemory()` doesn't return predictable results, but it can verify that it does return a non-zero value and doesn't crash while doing so. In this same example, `nullptr` can be passed for the single parameter to verify that the function handles this case.

### Src/Utils

Contains utilities which will be used internally, almost exclusively by developers. Some are temporary and may be disconnected from the solution, but are kept in case they are needed in the future. Currently the contents are:

## ThirdParty

Contains third party libraries *except* Qt. Some libraries need to be built. For most, header-only libraries are favored for simplicity. 

> Sometimes an OpenSSL library will be here. If a solution is purely Qt, the Qt version can probably be used. This has not been tested since all the solutions so far had some non-Qt projects.

**ThirdParty/gsl** is a very trimmed down copy of the everlastingly crappy Guidelines Support Library. It is here solely to support *span*. C++20 has a replacement, but has breaking changes with other libraries so is currently being ignored.

**ThirdParty/lz4** is an extremely fast lossless compression algorithm. (This is built as part of the solution.)

**ThirdParty/OpenSSL** is a toolkit for TSL and SSL protocols and a general-purpose cryptography library. 

**ThirdParty/RapidJSON** is a JSON parse and generator. (A header only library.) This is slightly modified from the distribution by adding a series of defines at the top of rapidjson.h. *(Experience found that these defines were being set inconsistently, causing problems.)*

### Creating New Projects

There are three ways to create a new native C++ project: use the Visual Studio wizard, copy an existing project of a similar type or have Joe do it for you.

#### Creating a new project using the Visual Studio wizard:

1. Open the solution.
2. Create a new project using the Visual Studio wizard.
3. Close the solution and use a comparison tool to compare the created *.vcxproj* with one using similar requirements.
4. Remove ALL references to x86 builds.
5. Copy over x64 settings.
6. Open the *App.sln* file and remove all lines containing x86 from the *Global* section.

> Removing all references to x86 may sound like busy work, but they save a world of pain when changing project settings (and you have x86 selected by accident.) I've had projects mysteriously start to fail only to find out that an x86 build is getting mixed in with the x64 projects.

#### Creating a new project using copy:

Copy an existing project which uses the same type of target and then change GUID in the *.vcxproj* file to something unique. (Online GUID generators can be used for this.) In the latter, case, you may also need to change some other strings in the *.vcxproj* file.

### Software Development Standards (brief)

This is a minimalist list of standards. A more comprehensive document may follow.

1. Use C++17 (C++20 made a blunder with char8_t and broke C++17 with several third party libraries, especially Qt.)
2. Set tabs to four spaces and use spaces.
3. The "Allman" style of formatting is used--this is the default for Visual Studio.
4. Lines should not exceed 128 characters, with 160 characters being a hard limit.
5. Minus leading whitespace, comment blocks should not exceed 90 characters per line.
6. For basic variable types where size must be known, use those in cstdint.
7. Use UpperCamelCase for function, static methods, namespace and enumerated value names.
8. Classes should be ordered with constructor/destructor at top, methods and then member data.
   1. Method names should use lowerCamelCase.
   2. Member data names should start with an underscore. *(Note: The C++ rule about using leading underscores applies only to items within the global namespace.)*
   3. If the class can’t be default trivially copyable, either add a copy constructor *and* assignment operator or explicitly delete them.
9. Structs should be ordered with data at top and no underscores. 
   1. Structs **must** be trivially copyable. (If you have to add a copy constructor and assignment operator, it must be changed to a class.)
10. Use smart pointers and make_unique/make_shared.
    1. *new or delete should not be used except when Qt requires it.*

### 