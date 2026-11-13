# Release Checklist

## Steps

1. Set the version number in the root `CMakeLists.txt`.
2. Build the solution in "Release" mode.
3. Run (`prepare_release`)[/tools/prepare_release] to copy the binaries and DLL files into the `dist` folder.
4. Copy any desired maps to `dist/res/maps`.
5. Ensure the version and known issues are up to date in `dist/README.txt`.
6. Ensure the `dist/docs/changelog.txt` is up to date.
7. Copy `dist` to a temporary folder, and test that everything works.
8. Package it up as **open-rival_vX.X.X_x64.zip**.
9. Tag the repo with the latest version.

> NOTE: The game cannot be run in "Debug" mode on machines without a development environment installed.
