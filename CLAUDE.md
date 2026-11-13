# Open-Rival

Faithful, modernized reimplementation of the 1998 RTS game Rival Realms.

## Rules

- Don't commit to Git directly.
- Respect the [contributing guidelines](docs/CONTRIBUTING.md).
- In general, focus on the [Open-Rival](projects/Open-Rival) subproject and the accompanying [unit tests](projects/Open-Rival-test).
- Any new files should be added to the file list in [projects/Open-Rival/CMakeLists.txt](projects/Open-Rival/CMakeLists.txt) (or regenerate it with `tools/gen_project_files.py`), so that they are built into `Open-Rival-lib`, which is shared with the test project.
- Unless instructed otherwise, keep changes narrow and precise.
