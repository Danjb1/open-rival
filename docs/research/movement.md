# Movement

Research relating to troop movement will be added here.

We don't necessarily need to emulate the original game's behaviour exactly - probably we can improve on it.

## Groups

It is difficult to make heads or tails of the group pathfinding logic.

Generally, units try to get as close as they can to the chosen destination. If the destination tile is blocked, they will stop in a neighbouring tile.

## Patrolling

When a unit's patrol destination is blocked, they will change their patrol destination to the nearest tile en route to the destination. Thus, it is possible to shrink a unit's patrol path incrementally by repeatedly blocking the destination.
