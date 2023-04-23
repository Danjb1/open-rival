# Net Code

> The general approach to multiplayer is described [here](multiplayer.md), but this document describes the code in more detail.

## Overview

- The relay server may be started by the host, or it could (in theory) be run as a standalone program.
- Clients - including the host - connect to the relay server before starting the game.
- At the end of each tick, clients send all newly-issued commands to the relay server in the form of a `GameCommandPacket`.
    - If no commands were issued, an empty `GameCommandPacket` is sent to denote "no input".
- The relay server simply forwards all received packets onto all *other* players.
- When a client receives a packet, it gets deserialized by a PacketFactory and stored in a queue.
- At the start of each tick, the `GameState` polls all received packets and looks for a registered `PacketHandler` for each of them.
- The `GameCommandPacketHandler` schedules other players' commands for the appropriate tick.
- If the `GameState` arrives at a tick for which not all other players' commands have been received, the game will pause until the message arrives.
