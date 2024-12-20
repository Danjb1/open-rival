# Running Rival Realms

## Introduction

Rival Realms does not work properly on modern Windows; a strange game-breaking bug occurs that prevents units from healing while resting. Fortunately, this can be fixed by running the game through [DxWnd](https://sourceforge.net/projects/dxwnd/).

## Download

Rival Realms can be freely downloaded [here](https://www.old-games.com/download/5514/rival-realms).

## DxWnd Configuration

Two configuration files are provided [here](docs/dxwnd) that can be imported into DxWnd directly (one for fullscreen, one for windowed). See the **Notes** tab in DxWnd for more information about the fixes.

## Multiplayer

For multiplayer, allow the game and the DirectPlay Helper through Windows Firewall (or disable the firewall). Windows should prompt you to install DirectPlay when you try to start a multiplayer game.

The host must configure port forwarding on their router for port 47624 (DirectPlay server port).

Crucially, the host MUST disable UPnP on their router! Otherwise the player joining will not be able to see the host's game.

## Scenario Editor

The scenario editor still works, but it will crash if the window is bigger than the current level. The solution is to resize the window.
