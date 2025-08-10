# Release Checklist

## Steps

1. Build the solution in "Release" mode.
2. Run (`prepare_release`)[/tools/prepare_release] to copy the binaries and DLL files into the `dist` folder.
3. Copy any desired maps to `dist/res/maps`.
4. Ensure the version and known issues are up to date in `dist/README.txt`.
5. Ensure the `dist/docs/changelog.txt` is up to date.
6. Copy `dist` to a temporary folder, and test that everything works.
7. Package it up as **open-rival_vX.X.X_x64.zip**.
8. Tag the repo with the latest version.

> NOTE: The game cannot be run in "Debug" mode on machines without a development environment installed.
