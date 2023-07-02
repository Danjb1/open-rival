# Release Checklist

The `dist` folder contains files that should be included with each release.

Many of these files need to be built and/or copied to this folder, and currently this is a manual process.

## Steps

1. Build the solution in "Release" mode.
2. Copy **Open-Rival.exe** to `dist`.
3. Copy **setup.exe** to `dist`.
4. Copy the [required DLL files](/docs/development_quickstart.md#dll-files) to `dist`.
5. Copy any desired maps to `dist/res/maps`.
6. Ensure the version and known issues are up to date in `dist/README.txt`.
7. Ensure the `dist/docs/changelog.txt` is up to date.
8. Copy `dist` to a temporary folder, and test that everything works.
9. Package it up as **open-rival_vX.X.X_x86.zip**.
10. Tag the repo with the latest version.
