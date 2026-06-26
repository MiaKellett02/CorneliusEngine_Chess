#pragma once

//Project includes.
#include "Vector2.h"

//Library includes.
#include <map>
#include <vector>
#include <limits>

//Class definitions.
namespace CorneliusEngine {

	class AStarPosition {
	public:
		//Variables.
		Vector2Int position;
		std::map<AStarPosition*, float> connections; // Key = position, value = cost to move to that position.
		// Djikstra position stored as pointer but not owned by this object.

		const float START_COST = 10000000;
		float totalCostToStart = START_COST;
		AStarPosition* connectingNodeToStart = nullptr;

		//Functions,
		void MakeConnection(AStarPosition* a_newConnection, float a_movementCostMultiplier);
		float CalculateMovementCost(AStarPosition* a_other, float a_movementCostMultiplier);
	};
}

namespace CorneliusEngine {

	class AStarPathfinding {
	public:
		static std::vector<Vector2Int> FindPath(CorneliusEngine::AStarPosition* a_startPos, CorneliusEngine::AStarPosition* a_goalPos, std::vector<Vector2Int>& outCheckedPositions);

		static void SortPositions(std::vector<CorneliusEngine::AStarPosition*>& a_positonsToSort);

	private:
		static bool ListHasValue(AStarPosition* a_value, std::vector<AStarPosition*>& a_list);
		static void RemoveValue(AStarPosition* a_value, std::vector<AStarPosition*>& a_list);
	};
}