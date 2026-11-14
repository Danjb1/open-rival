#include "game/PassabilityMap.h"

#include <algorithm>  // std::erase_if, std::find, std::max_element
#include <stdexcept>

namespace Rival {

PassabilityMap::PassabilityMap(int width, int height, TilePassability defaultTilePassability)
    : width(width)
    , height(height)
    , tilePassabilities(width * height, defaultTilePassability)
{
}

PassabilityMap::PassabilityMap(int width, int height, const std::vector<TilePassability>& tilePassability)
    : width(width)
    , height(height)
    , tilePassabilities(tilePassability)
{
}

/**
 * Assigns every tile to a reachability group.
 *
 * This uses a flood fill to find each connected region of tiles with equivalent passability.
 */
void PassabilityMap::initReachability()
{
    tileGroups.assign(tilePassabilities.size(), noGroup);
    groups.clear();
    emptyGroups.clear();

    std::vector<MapNode> nodesToVisit;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const MapNode start(x, y);
            if (tileGroups[getTileIndex(start)] != noGroup)
            {
                // Already part of a group
                continue;
            }

            // Start a new group and flood fill from here
            const int group = createGroup();
            moveTileToGroup(getTileIndex(start), group);
            nodesToVisit.push_back(start);

            while (!nodesToVisit.empty())
            {
                const MapNode node = nodesToVisit.back();
                nodesToVisit.pop_back();
                const TilePassability passability = getPassability(node);

                for (const MapNode& neighbor : MapUtils::findNeighbors(node, *this))
                {
                    const int neighborIndex = getTileIndex(neighbor);
                    if (tileGroups[neighborIndex] == noGroup
                            && areTilePassabilitiesEquivalent(passability, tilePassabilities[neighborIndex]))
                    {
                        moveTileToGroup(neighborIndex, group);
                        nodesToVisit.push_back(neighbor);
                    }
                }
            }
        }
    }

    isReachabilityInitialized = true;
}

bool PassabilityMap::areTilePassabilitiesEquivalent(TilePassability a, TilePassability b) const
{
    if ((a & TilePassability::Water) != (b & TilePassability::Water))
    {
        // Only 1 tile is water
        return false;
    }

    if ((a & unpathableFlags) != (b & unpathableFlags))
    {
        // Only 1 tile is unpathable
        return false;
    }

    // Both tiles have equivalent passability, so they belong to the same reachability group
    return true;
}

int PassabilityMap::getTileIndex(const MapNode& pos) const
{
    return pos.y * width + pos.x;
}

bool PassabilityMap::isInBounds(const MapNode& pos) const
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

TilePassability PassabilityMap::getPassability(const MapNode& pos) const
{
    return tilePassabilities[getTileIndex(pos)];
}

void PassabilityMap::setPassability(const MapNode& pos, TilePassability passability)
{
    updatePassability(pos, passability);
}

void PassabilityMap::addPassability(const MapNode& pos, TilePassability flags)
{
    updatePassability(pos, getPassability(pos) | flags);
}

void PassabilityMap::removePassability(const MapNode& pos, TilePassability flags)
{
    // Use bitwise AND with the inverted flags to remove the specified flags
    updatePassability(pos, getPassability(pos) & ~flags);
}

void PassabilityMap::updatePassability(const MapNode& pos, TilePassability newPassability)
{
    TilePassability& passability = tilePassabilities[getTileIndex(pos)];
    const TilePassability oldPassability = passability;
    passability = newPassability;

    if (!isReachabilityInitialized)
    {
        // Reachability will be calculated for all tiles when it is initialized
        return;
    }

    if (areTilePassabilitiesEquivalent(oldPassability, newPassability))
    {
        // Most changes (e.g. units moving around) have no effect on reachability
        return;
    }

    // This tile now belongs to a different group (e.g. a building has been placed or destroyed)
    removeTileFromGroup(pos);
    addTileToGroup(pos);
}

/**
 * Removes a tile from its current reachability group.
 *
 * This may split the group in two (or more), if the tile was the only connection between parts of the group.
 */
void PassabilityMap::removeTileFromGroup(const MapNode& pos)
{
    const int group = tileGroups[getTileIndex(pos)];
    moveTileToGroup(getTileIndex(pos), noGroup);

    // Find the neighbors that were in the same group; these may no longer be connected to each other
    std::vector<MapNode> neighborsInGroup;
    for (const MapNode& neighbor : MapUtils::findNeighbors(pos, *this))
    {
        if (tileGroups[getTileIndex(neighbor)] == group)
        {
            neighborsInGroup.push_back(neighbor);
        }
    }

    splitGroupIfDisconnected(group, neighborsInGroup);
}

/**
 * Adds a tile to the reachability group of its neighbors with equivalent passability.
 *
 * If the tile connects multiple groups, these are merged.
 */
void PassabilityMap::addTileToGroup(const MapNode& pos)
{
    const TilePassability passability = getPassability(pos);

    // Find the distinct groups of all equivalent neighbors
    std::vector<int> neighborGroups;
    for (const MapNode& neighbor : MapUtils::findNeighbors(pos, *this))
    {
        const int neighborGroup = tileGroups[getTileIndex(neighbor)];
        if (neighborGroup != noGroup && areTilePassabilitiesEquivalent(passability, getPassability(neighbor))
                && std::find(neighborGroups.cbegin(), neighborGroups.cend(), neighborGroup) == neighborGroups.cend())
        {
            neighborGroups.push_back(neighborGroup);
        }
    }

    int group = noGroup;
    if (neighborGroups.empty())
    {
        // Not connected to any equivalent tiles
        group = createGroup();
    }
    else
    {
        // Keep the largest group, and merge the others into it, so that we move as few tiles as possible
        group = *std::max_element(neighborGroups.cbegin(), neighborGroups.cend(), [&](int a, int b) {
            return groups[a].size() < groups[b].size();
        });

        for (int neighborGroup : neighborGroups)
        {
            if (neighborGroup != group)
            {
                mergeGroups(neighborGroup, group);
            }
        }
    }

    moveTileToGroup(getTileIndex(pos), group);
}

/**
 * Checks whether the given nodes (all from the same group) are still connected to one another, and splits the group if
 * not.
 *
 * In the common case (e.g. a building placed in an open area), the nodes are still connected via some nearby path, so
 * this terminates quickly. Only a genuine split requires a whole region to be explored.
 */
void PassabilityMap::splitGroupIfDisconnected(int group, std::vector<MapNode> nodes)
{
    // Loop until all nodes in the group have been visited
    while (nodes.size() > 1)
    {
        // Starting with the first node, we are trying to find all *other* nodes
        std::unordered_set<MapNode> nodesToFind(nodes.cbegin() + 1, nodes.cend());
        std::unordered_set<int> visitedIndices = { getTileIndex(nodes.front()) };
        std::vector<MapNode> nodesToVisit = { nodes.front() };

        // Search all connected nodes until we run out of options (flood fill)
        while (!nodesToVisit.empty() && !nodesToFind.empty())
        {
            // Grab the next node from nodesToVisit
            const MapNode node = nodesToVisit.back();
            nodesToVisit.pop_back();

            // Look at all neighbors to see if they are connected
            for (const MapNode& neighbor : MapUtils::findNeighbors(node, *this))
            {
                const int neighborIndex = getTileIndex(neighbor);

                if (tileGroups[neighborIndex] != group)
                {
                    // Neighbor is not connected (belongs to a different group)
                    continue;
                }

                if (const bool wasInserted = visitedIndices.insert(neighborIndex).second)
                {
                    // We have found a new connected node (not seen before)
                    nodesToFind.erase(neighbor);
                    nodesToVisit.push_back(neighbor);
                }
            }
        }

        if (nodesToFind.empty())
        {
            // Every node in the group was found via the flood fill, which means they are all still connected
            return;
        }

        // We have explored the entire region connected to the first node, and it does not include all of the nodes.
        // This region has become separated from the rest of the group, so move it to a new group.
        const int newGroup = createGroup();
        for (int tileIndex : visitedIndices)
        {
            moveTileToGroup(tileIndex, newGroup);
        }

        // Remove the region we just visited from our node list, now that it has been dealt with.
        // The loop will repeat if any nodes remain, in case the group has been split more than once.
        std::erase_if(nodes, [&](const MapNode& node) { return visitedIndices.contains(getTileIndex(node)); });
    }
}

/**
 * Moves all tiles from one group into another.
 */
void PassabilityMap::mergeGroups(int fromGroup, int intoGroup)
{
    // Take a copy, since moving tiles modifies the group
    const std::vector<int> tilesToMove(groups[fromGroup].cbegin(), groups[fromGroup].cend());
    for (int tileIndex : tilesToMove)
    {
        moveTileToGroup(tileIndex, intoGroup);
    }
}

/**
 * Moves a tile into a different group (or `noGroup`).
 *
 * This is the only place where group membership should be modified, to keep `tileGroups` and `groups` in sync.
 */
void PassabilityMap::moveTileToGroup(int tileIndex, int newGroup)
{
    const int oldGroup = tileGroups[tileIndex];
    if (oldGroup != noGroup)
    {
        groups[oldGroup].erase(tileIndex);

        // Keep track of empty groups rather than removing them, as removal would impact other group IDs.
        // Empty groups are reused by `createGroup` to stop them accumulating.
        if (groups[oldGroup].empty())
        {
            emptyGroups.push_back(oldGroup);
        }
    }

    tileGroups[tileIndex] = newGroup;
    if (newGroup != noGroup)
    {
        groups[newGroup].insert(tileIndex);
    }
}

/**
 * Gets the ID of an empty group, reusing an existing group if possible.
 */
int PassabilityMap::createGroup()
{
    if (!emptyGroups.empty())
    {
        const int group = emptyGroups.back();
        emptyGroups.pop_back();
        return group;
    }

    groups.emplace_back();
    return static_cast<int>(groups.size()) - 1;
}

bool PassabilityMap::isReachable(const MapNode& a, const MapNode& b) const
{
    if (!isReachabilityInitialized)
    {
        throw std::runtime_error("Reachability has not been initialized");
    }

    if (!isInBounds(a) || !isInBounds(b))
    {
        return false;
    }

    return tileGroups[getTileIndex(a)] == tileGroups[getTileIndex(b)];
}

}  // namespace Rival
