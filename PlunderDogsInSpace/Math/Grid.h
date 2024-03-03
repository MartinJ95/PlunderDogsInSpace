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
	int first, second;
};

template<typename T>
struct HorizontalGridSmall
{
public:
	HorizontalGridSmall(float MinBounds, float MaxBounds);
	bool IsOnGrid(const glm::vec3& Point);
	TileIndex GetTilePosition(const glm::vec3& Point);
	T& GetCurrentTile(const glm::vec3& Point);
	glm::vec3 GetPointOnTile(const glm::vec3& Point);
private:
	glm::vec3 minBounds, maxBounds;
	T tiles[SmallGridTileAmount][SmallGridTileAmount];
};