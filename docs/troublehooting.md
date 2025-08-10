# :stethoscope: Troubleshooting

> This guide was written when dependency management was a much more manual process. This should not be relevant anymore now that we are using **vcpkg** to manage dependencies.

## LNK1104	cannot open file 'XXXXX.lib'

This signifies that a library cannot be found by the linker.

Check that the library has been built and that the required `.lib` file is present in one of the linker's "Additional Library Directories".

## LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs; use /NODEFAULTLIB:library

This signifies that a library is including a different C Runtime (CRT) from Open-Rival. Check the build settings of all libraries (C/C++ > Code Generation > Runtime Library).

Open-Rival uses "Multi-threaded Debug DLL (/MDd)" or "Multi-threaded DLL (/MD)", as this is the version [required by SDL](https://wiki.libsdl.org/SDL2/FAQWindows#when_using_visual_c_i_get_link_errors_relating_to_msvcrt.lib_or_libc). All libraries should be built with this same version.

## The code execution cannot proceed because XXXXX.dll was not found

This signifies a missing DLL file.

Check that the folder containing `Open-Rival.exe` also contains the [required DLLs](/docs/dependencies.md#dll-files), and that these are the correct DLLs for the current architecture (x86 or x64).

## The application was unable to start correctly 0xc00007b

This signifies a missing or incorrect DLL file.

Check that the folder containing `Open-Rival.exe` also contains the [required DLLs](/docs/dependencies.md#dll-files), and that these are the correct DLLs for the current architecture (x86 or x64).
