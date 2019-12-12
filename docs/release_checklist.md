# Release Checklist

The `dist` folder contains files that should be included with each release.

Many of these files need to be built and/or copied to this folder, and currently this is a manual process.

## Steps

1) Build the solution

1) Copy **Open-Rival.exe** to `dist`

1) Copy the other executables to `dist/setup`

1) Copy **glew32.dll** and **SDL2.dll** to `dist`

1) Copy `res/maps` and `res/shaders` to `dist/res`

1) Copy `texture-builder/definitions` to `dist/setup`

1) Ensure the version is correct in `readme.txt`

1) Tag the repo with the latest version

1) Zip it up!
