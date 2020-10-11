# Multiplayer Strategy

 - Use a client-server architecture with lockstep model.

 - Commands are executed 1 turn AFTER the turn when the server sends them.
    - This gives clients time to receive them.
    - This means there could be a maximum delay of 2 turns (300ms) between a command being issued and it being executed.

    > In single-player mode, skip the client-server aspect entirely and process commands on the client as they are issued (?)

 - To minimise the perceived delay, clients should receive some immediate feedback, e.g. unit voices.
    - We could use client-side prediction but then we would have to contend with discrepancies arising.

 - If a client has not received a turn from the server by the time that turn arrives, they have to pause and wait for it (?)

 - If the server receives a command too late for it to be included in the turn it SHOULD HAVE belonged to, it should be queued for the following turn.

 - Replays can be implemented easily by saving the commands for each turn.

---

## Example

Timestep = 15ms (60 fps)
Turn interval = 150ms (10 frames)

These numbers can be fine-tuned as required.

```
t = 0
ClientA issues MoveCommand
ClientB issues BuildCommand
ClientC issues AttackCommand

t = 45
Server receives and queues MoveCommand

t = 60
Server receives and queues BuildCommand

t = 150 (turn 1)
Server sends queued commands to all clients

t = 175
Server receives AttackCommand - too late for turn 1! Command gets queued for the next turn.

t = 215
ClientA receives turn 1 commands from server
ClientB receives turn 1 commands from server

t = 300 (turn 2)
Server executes turn 1 commands
ClientA executes turn 1 commands
ClientB executes turn 1 commands
ClientC pauses until it receives turn 1 commands, then executes them.
```

 - In this example, ClientC may experience frequent stutters due to a poor connection, and their commands may be frequently delayed. This should not impact the other players.

 - The server should be able to detect lagging clients by looking at the timestamps on the commands it receives.
    - We could also send pings periodically to gauge connection quality.
