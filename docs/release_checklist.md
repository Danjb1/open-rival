# Release Checklist

The `dist` folder contains files that should be included with each release.

Many of these files need to be built and/or copied to this folder, and currently this is a manual process.

## Steps

1) Build the solution in "Release" mode.

1) Copy **Open-Rival.exe** to `dist`.

1) Copy **setup.exe** to `dist`.

1) Copy the [required DLL files](/docs/development_quickstart.md#dll-files) to `dist`.

1) Copy `Open-Rival/res/maps` and `Open-Rival/res/shaders` to `dist/res`.

1) Copy `setup/definitions` to `dist/setup`.

1) Ensure the version is correct in `readme.txt`.

1) Ensure the `dist/docs/changelog.txt` is up to date.

1) Tag the repo with the latest version.

1) Package it up as **open-rival_vX.X.X_x86.zip**.
