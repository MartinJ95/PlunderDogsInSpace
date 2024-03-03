#include "Grid.h"

template<typename T>
HorizontalGridSmall<T>::HorizontalGridSmall(float MinBounds, float MaxBounds) : minBounds(MinBounds), maxBounds(MaxBounds)
{
	minbounds.y = maxBounds.y = 0;
}

template<typename T>
bool HorizontalGridSmall<T>::IsOnGrid(const glm::vec3& Point)
{
	if (Point.x < minBounds.x || Point.x > maxBounds.x || Point.z < minBounds.z || Point.z > maxBounds.z)
		return false;
	return true;
}

template<typename T>
TileIndex HorizontalGridSmall<T>::GetTilePosition(const glm::vec3& Point)
{
	TileIndex indices;
	indices.first = ((maxBounds.z - Point.z) / (maxBounds.z - minBounds.z)) * SmallGridTileAmount;
	indices.second = ((maxBounds.x - Point.x) / (maxBounds.x - minBounds.x)) * SmallGridTileAmount;
	return TileIndex();
}

template<typename T>
T& HorizontalGridSmall<T>::GetCurrentTile(const glm::vec3& Point)
{
	TileIndex i = GetTilePosition(Point);
	return tiles
		[i.first]
	[i.second];
}

template<typename T>
glm::vec3 HorizontalGridSmall<T>::GetPointOnTile(const glm::vec3& Point)
{
	TileIndex i = GetTilePosition();
	return glm::vec3(
	minBounds.x + (((maxBounds.x - minBounds.x)/SmallGridTileAmount)*i.first),
	0,
		minBounds.z + (((maxBounds.z - minBounds.z) / SmallGridTileAmount) * i.second);
}
