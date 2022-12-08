# Multiplayer

## Introduction

Online multiplayer is a complicated part of any game, but it is something that needs to be carefully planned from the early stages of development.

There is a superb guide to all of the important concepts [here](https://meseta.medium.com/netcode-concepts-part-3-lockstep-and-rollback-f70e9297271).

## Lockstep Model

For RTS games, lockstep is a very commonly-used mechanism to keep clients in-sync. The general premise is that before every tick, every client waits to receive instructions from every other client. This ensures that the game runs in an identical manner on every client, because each of them is processing instructions sequentially in a deterministic manner. As an added bonus, this means that replays can be implemented easily by saving the commands for each turn.

Whenever a client issues a command (e.g. "Place Building"), the command should be scheduled for 'n' ticks in the future. This gives all other clients time to receive the command, so that they can all execute it in the same game tick without needing to wait. If a message is delayed, then all clients have to wait for the message to arrive before continuing.

> Note that a message might just be "no input", if a client has not issued any commands.

The one downside to this approach is perceived input lag, which can be mitigated using prediction (e.g. placing a "fake" building immediately and replacing it with the "real" building later), and rollback (if the building cannot be built when the time comes, the fake building must be deleted).

## Tick Rate

The notion of "tick rate" is a little confusing as there are really 3 separate values at play here:

### (Multiplayer) Input Tick Rate

This is the rate at which commands issued by players are processed. For example, if this is set to 10, then new commands from all players will be expected 10 times per second.

**If this is too high** the game uses more network traffic and the risk of delayed messages is higher.

**If this is too low** the game may feel slow to respond to user input.

### Logic Tick Rate

This is the rate at which the game logic is updated. This may be the same as the input tick rate, but it could also be a multiple of the input tick rate; for example, if this was double the input tick rate then every other frame would be executed with no additional input processing.

This may be desirable to make the timing of actions in the game more fine-grained.

**If this is too high** the game uses more CPU power, and may be spending unnecessary cycles when there is nothing to update.

**If this is too low** the game may feel less fluid.

### Refresh Rate (Framerate)

This is the rate at which the display is refreshed. If we implement interpolation correctly, then this also governs how smoothly entities can move across the map.

**If this is too high** (i.e. it exceeds the monitor's refresh rate), then the game may perform unnecessary work.

**If this is too low** the game will feel "stuttery".

## Topology

Since we are using the lockstep model, it makes sense to use a relay server. In other words, clients connect to a server, but the server does not run the game logic; it just passes messages between clients. This is simple to implement, and easier for players to set up than P2P connections.

A client can also play the role of the relay server, to remove the need for a dedicated server.

Since the game logic is running directly on the client, this also makes single-player games much simpler to implement. They can work in exactly the same way as multiplayer, but commands can be executed in the tick immediately after they are issued.

## Protocol

For a lockstep model, TCP makes sense because *all* packets require reliable, in-order delivery.

However, UDP still offers a few advantages...

1. NAT traversal / hole punching is easier, should we want to establish direct connections between clients. Although we are using a relay server, this is likely to be running on a client machine, so hole punching could still be valuable.

    > More info [here](https://tailscale.com/blog/how-nat-traversal-works/).

2. Although we require reliable, in-order delivery, there are some tricks we could use with UDP to add tolerance for dropped packets and thus increase reliability. For example, if a player is not issuing any commands, we could send packets that look like this:

        Packet #46: No commands issued for 1 tick
        Packet #47: No commands issued for 2 ticks
        Packet #48: No commands issued for 3 ticks

    This way, as long as Packet #48 is received, we can still continue with the game, even if Packets #46 and #47 are lost.

    > This is arguably a premature optimisation, but nonetheless warrants consideration due to the importance of the decision of which protocol to use.

If we opt for UDP, we should use a library to provide support for reliable messaging, as this is a non-trivial problem and there are established "reliable UDP" protocols that ought to be followed.

In either case, we should add a layer of abstraction to separate the networking internals from the game logic, so that we can change our mind later on if needs be.
