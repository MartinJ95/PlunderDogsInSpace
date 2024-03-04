#pragma once
#include "glm/glm/glm.hpp"

constexpr unsigned int SmallGridTileAmount = 25;
constexpr unsigned int MediumGridTileAmount = 50;
constexpr unsigned int LargeGridTileAmount = 75;

struct BaseTile
{

};

struct TileIndex
{
	TileIndex() : first(0), second(0) {}
	TileIndex(int First, int Second) : first(First), second(Second) {}
	int first, second;
};

template<typename T>
struct HorizontalGridSmall
{
public:
	HorizontalGridSmall(float MinBounds, float MaxBounds) : minBounds(MinBounds), maxBounds(MaxBounds)
	{
		minBounds.y = maxBounds.y = 0;
	}
	bool IsOnGrid(const glm::vec3& Point) const
	{
		if (Point.x < minBounds.x || Point.x > maxBounds.x || Point.z < minBounds.z || Point.z > maxBounds.z)
			return false;
		return true;
	}
	TileIndex GetTilePosition(const glm::vec3& Point) const
	{
		TileIndex indices;
		indices.first = ((maxBounds.z - Point.z) / (maxBounds.z - minBounds.z)) * SmallGridTileAmount;
		indices.second = ((maxBounds.x - Point.x) / (maxBounds.x - minBounds.x)) * SmallGridTileAmount;
		return TileIndex();
	}
	T& GetCurrentTile(const glm::vec3& Point)
	{
		TileIndex i = GetTilePosition(Point);
		return tiles
			[i.first]
		[i.second];
	}
	T& GetTile(const TileIndex& Index)
	{
		return tiles[Index.first][Index.second];
	}
	glm::vec3 GetPointOnTile(const glm::vec3& Point) const
	{
		TileIndex i = GetTilePosition(Point);
		return glm::vec3(
			minBounds.x + (((maxBounds.x - minBounds.x) / SmallGridTileAmount) * i.first),
			0,
			minBounds.z + (((maxBounds.z - minBounds.z) / SmallGridTileAmount) * i.second));
	}
	glm::vec3 GetPointOnTile(const TileIndex& Index) const
	{
		return glm::vec3(
			minBounds.x + (((maxBounds.x - minBounds.x) / SmallGridTileAmount) * Index.first),
			0,
			minBounds.z + (((maxBounds.z - minBounds.z) / SmallGridTileAmount) * Index.second));
	}
private:
	glm::vec3 minBounds, maxBounds;
	T tiles[SmallGridTileAmount][SmallGridTileAmount];
};