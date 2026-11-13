# --- Test Headers ---
# These are header files that exist only in the test project.
set(OPEN_RIVAL_TEST_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/mocks/MockAudioSystem.h
    ${CMAKE_CURRENT_LIST_DIR}/include/utils/EntityTestUtils.h
)

# --- Test Sources ---
# These are .cpp files that exist only in the test project, including the
# tests themselves.
set(OPEN_RIVAL_TEST_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/setup/LoggingSetup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestAnimationComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestAttackComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestCamera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestCollectionUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestEntity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMapUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMousePicker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestRenderUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestSpatialIndex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestSpritesheet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestWalkerComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestWorld.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/EntityTestUtils.cpp
)

# Organise files into folders (filters)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Source Files" FILES ${OPEN_RIVAL_TEST_SOURCES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Source Files" FILES ${OPEN_RIVAL_TEST_HEADERS})

# Creates the test executable.
# All of the game code comes from Open-Rival-lib.
add_executable(Open-Rival-test
    ${OPEN_RIVAL_TEST_SOURCES}
    ${OPEN_RIVAL_TEST_HEADERS}
)

target_include_directories(Open-Rival-test PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Find dependencies
find_package(Catch2 3 CONFIG REQUIRED)

# Link dependencies
target_link_libraries(Open-Rival-test PRIVATE
    Open-Rival-lib
    Catch2::Catch2WithMain
    project_warnings
)

# Pre-compiled headers (shared with the library to avoid building them twice)
target_precompile_headers(Open-Rival-test REUSE_FROM Open-Rival-lib)

# Register each test case with CTest.
# PRE_TEST defers discovery until the tests are run, so the build doesn't need
# to run the test executable (which requires its DLLs to have been copied).
include(Catch)
catch_discover_tests(Open-Rival-test DISCOVERY_MODE PRE_TEST)
