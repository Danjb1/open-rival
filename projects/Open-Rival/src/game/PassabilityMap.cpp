#include "game/PassabilityMap.h"

#include <algorithm>
#include <array>
#include <cassert>

#include "utils/LogUtils.h"

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

void PassabilityMap::initReachability()
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            MapNode node(x, y);
            refreshReachability(node);
        }
    }
}

void PassabilityMap::refreshReachability(const MapNode& node)
{
    TilePassability tilePassability = getPassability(node);
    std::unordered_set<int> foundGroupIds;

    // Check each neighbor to see if we belong to any of their reachability groups
    std::vector<MapNode> neighbors = MapUtils::findNeighbors(node, *this);
    for (const MapNode& neighbor : neighbors)
    {
        if (areTilePassabilitiesEquivalent(tilePassability, getPassability(neighbor)))
        {
            // This tile belongs to the neighbor's reachability group
            int groupIndex = getReachabilityGroupForTile(neighbor);
            if (groupIndex == -1)
            {
                // Neighbor hasn't been initialized yet.
                // This happens when we check the north-east neighbor or the south-west neighbor from a lower tile.
                continue;
            }

            foundGroupIds.insert(groupIndex);
        }
    }

    if (foundGroupIds.empty())
    {
        // If we don't belong to the same group as any neighbor, start a new group
        TileGroup newGroup = { node };
        reachabilityGroups.push_back(newGroup);
    }
    else
    {
        int groupIndex = mergeReachabilityGroups(foundGroupIds);
        TileGroup& group = reachabilityGroups[groupIndex];
        group.insert(node);
    }
}

int PassabilityMap::mergeReachabilityGroups(const std::unordered_set<int>& foundGroupIds)
{
    if (foundGroupIds.size() == 1)
    {
        // Nothing to merge, just return the sole group ID
        return *foundGroupIds.cbegin();
    }

    // Put the group indices into a sorted list
    std::vector<int> sortedGroupIds(foundGroupIds.cbegin(), foundGroupIds.cend());
    std::sort(sortedGroupIds.begin(), sortedGroupIds.end());

    // Pick the first group - this is the one we will keep
    int firstGroupId = sortedGroupIds.front();
    TileGroup& firstGroup = reachabilityGroups[firstGroupId];

    // Merge all groups into the first group
    for (int i = 1; i < sortedGroupIds.size(); ++i)
    {
        const TileGroup& group = reachabilityGroups[sortedGroupIds[i]];
        firstGroup.insert(group.cbegin(), group.cend());
    }

    // Remove the other groups in reverse order (so as not to invalidate the indices)
    for (int i = static_cast<int>(sortedGroupIds.size()) - 1; i > 0; --i)
    {
        reachabilityGroups.erase(reachabilityGroups.begin() + sortedGroupIds[i]);
    }

    return firstGroupId;
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

int PassabilityMap::getReachabilityGroupForTile(const MapNode& node) const
{
    for (int i = 0; i < reachabilityGroups.size(); ++i)
    {
        const auto& group = reachabilityGroups[i];
        if (group.find(node) != group.cend())
        {
            return i;
        }
    }

    return -1;
}

TilePassability PassabilityMap::getPassability(const MapNode& pos) const
{
    return tilePassabilities[pos.y * width + pos.x];
}

void PassabilityMap::setPassability(const MapNode& pos, TilePassability passability)
{
    tilePassabilities[pos.y * width + pos.x] = passability;
    refreshReachability(pos);
}

void PassabilityMap::addPassability(const MapNode& pos, TilePassability flags)
{
    tilePassabilities[pos.y * width + pos.x] |= flags;
    refreshReachability(pos);
}

void PassabilityMap::removePassability(const MapNode& pos, TilePassability flags)
{
    // Use bitwise AND with the inverted flags to remove the specified flags
    tilePassabilities[pos.y * width + pos.x] &= ~flags;
    refreshReachability(pos);
}

bool PassabilityMap::isReachable(const MapNode& a, const MapNode& b) const
{
    const auto& aGroup = getReachabilityGroupForTile(a);
    const auto& bGroup = getReachabilityGroupForTile(b);
    return aGroup == bGroup;
}

}  // namespace Rival
