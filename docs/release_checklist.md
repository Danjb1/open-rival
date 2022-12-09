# Release Checklist

The `dist` folder contains files that should be included with each release.

Many of these files need to be built and/or copied to this folder, and currently this is a manual process.

## Steps

1. Build the solution in "Release" mode.
2. Copy **Open-Rival.exe** to `dist`.
3. Copy **setup.exe** to `dist`.
4. Copy the [required DLL files](/docs/development_quickstart.md#dll-files) to `dist`.
5. Copy `data`, `icons` and `shaders` from `Open-Rival/res/` to `dist/res`.
6. Copy any desired maps from `Open-Rival/res/maps` to `dist/res/maps`.
7. Copy `setup/setup/definitions` to `dist/setup`.
8. Ensure the version and known issues are up to date in `dist/README.txt`.
9. Ensure the `dist/docs/changelog.txt` is up to date.
10. Copy `dist` to a temporary folder, and test that everything works.
11. Package it up as **open-rival_vX.X.X_x86.zip**.
12. Tag the repo with the latest version.
