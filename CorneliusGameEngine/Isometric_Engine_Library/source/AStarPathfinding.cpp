//Project includes.
#include "AStarPathfinding.h"
#include "Logging.h"
#include "Timer.h"

//Library includes.
#include <algorithm>
#include <assert.h>

std::vector<Vector2Int> CorneliusEngine::AStarPathfinding::FindPath(CorneliusEngine::AStarPosition* a_startPos, CorneliusEngine::AStarPosition* a_goalPos, std::vector<Vector2Int>& outCheckedPositions)
{

	Timer("Find Path Timer");
	std::vector<AStarPosition*> checkedPositions; //This list will store all the positions we've checked once we're done with them.

	//Setup start pos and priority list.
	a_startPos->totalCostToStart = 0;
	std::vector<AStarPosition*> priorityList;
	priorityList.push_back(a_startPos);

	//Keep going till path is found to the start pos.
	AStarPosition* currentNode = a_startPos;
	int distanceToGoalCurrent = (a_goalPos->position - currentNode->position).SqrMagnitude();
	while (currentNode != nullptr)
	{

		//Go through all the connections to the current post and ensure they're in the priority list.
		for (const auto& pair : currentNode->connections) {
			//Get the values.
			AStarPosition* node = pair.first;
			float movementCost = pair.second;

			// If connection hasn't been added to the priority list yet, then add it.
			bool inPriorityList = ListHasValue(node, priorityList);
			bool inCheckedList = ListHasValue(node, checkedPositions);
			if (!inPriorityList && !inCheckedList) {
				priorityList.push_back(node);
			}

			float newCostToStart = node->connections[currentNode] + distanceToGoalCurrent * distanceToGoalCurrent;
			int distanceToGoalSqr = (a_goalPos->position - node->position).SqrMagnitude();
			distanceToGoalSqr = distanceToGoalSqr * distanceToGoalSqr;
			if (node->connectingNodeToStart != nullptr) {
				//Make cost to start equal to the movement cost between the current node and this one PLUS the other nodes cost to start.
				if (currentNode->totalCostToStart != node->START_COST) {
					newCostToStart = newCostToStart + currentNode->totalCostToStart;
					newCostToStart = newCostToStart + distanceToGoalSqr;
				}
				if (newCostToStart < node->totalCostToStart) {
					//The new path to this node is shorter, update the shortest path marker.
					node->connectingNodeToStart = currentNode;
					node->totalCostToStart = newCostToStart;
				}
			}
			else {
				node->connectingNodeToStart = currentNode;
				node->totalCostToStart = newCostToStart + currentNode->totalCostToStart + distanceToGoalSqr;
			}
		}

		/*for (int i = 0; i < priorityList.size(); i++) {
			DjikstraPosition* node = priorityList[i];

		}*/

		if (priorityList.size() > 0) {
			SortPositions(priorityList);
		}

		//Current node has been checked, add it to checked list and remove from priority list.
		checkedPositions.push_back(currentNode);
		RemoveValue(currentNode, priorityList);

		//Update the current node
		if (currentNode != a_goalPos) {
			//Sort the priority list and then set the new current node to the one with the smallest cost to the start pos.
			if (priorityList.size() > 0) {
				//SortPositions(priorityList);
				currentNode = priorityList[0];
				distanceToGoalCurrent = (a_goalPos->position - currentNode->position).SqrMagnitude();
			}
		}
		else {
			//No more positions to check, so we can't explore any further.
			currentNode = nullptr;
		}
	}

	//Construct the path. 
	std::vector<Vector2Int> pathToGoal;
	AStarPosition* pathNode = a_goalPos;
	while (pathNode != nullptr) {
		pathToGoal.push_back(pathNode->position);

		if (pathNode == a_startPos) {
			break;
		}
		else {
			pathNode = pathNode->connectingNodeToStart;
		}
	}

	//Reverse the list.
	std::reverse(pathToGoal.begin(), pathToGoal.end());

	//Ensure it has the start pos in.
	bool hasStartPos = false;
	for (int i = 0; i < pathToGoal.size(); i++) {
		if (pathToGoal[i] == a_startPos->position) {
			hasStartPos = true;
			break;
		}
	}
	if (!hasStartPos) {
		CorneliusEngine::Log("Did not find valid route to goal node.");
	}

	//Output how many positions we checked.
	outCheckedPositions.clear();
	for (int i = 0; i < checkedPositions.size(); i++) {
		outCheckedPositions.push_back(checkedPositions[i]->position);
	}
	//std::string positionsCheckedString = std::to_string(checkedPositions.size()) + " positions checked.";
	//CorneliusEngine::Log(positionsCheckedString);

	//Return.
	return pathToGoal;
}

void CorneliusEngine::AStarPathfinding::SortPositions(std::vector<CorneliusEngine::AStarPosition*>& a_positonsToSort)
{
	for (int i = 0; i < a_positonsToSort.size(); i++) {
		for (int j = 0; j < a_positonsToSort.size() - 1; j++) {
			AStarPosition* posOne = a_positonsToSort[j];
			AStarPosition* posTwo = a_positonsToSort[j + 1];
			if (posOne->totalCostToStart > posTwo->totalCostToStart) {
				//Make the swap.
				a_positonsToSort[j] = posTwo;
				a_positonsToSort[j + 1] = posOne;
				int a = 0;
			}
		}
	}
}

bool CorneliusEngine::AStarPathfinding::ListHasValue(AStarPosition* a_value, std::vector<AStarPosition*>& a_list)
{
	for (int i = 0; i < a_list.size(); i++) {
		if (a_list[i] == a_value) {
			return true;
		}
	}

	//If not found, return false.
	return false;
}

void CorneliusEngine::AStarPathfinding::RemoveValue(AStarPosition* a_value, std::vector<AStarPosition*>& a_list)
{
	assert(a_value != nullptr);

	// Remove all occurrences of ptrToRemove
	a_list.erase(std::remove(a_list.begin(), a_list.end(), a_value), a_list.end());

}

void CorneliusEngine::AStarPosition::MakeConnection(AStarPosition* a_newConnection, float a_movementCostMultiplier)
{
	//Check if the connection is already made, if not then make it.
	if (a_newConnection != nullptr && !connections.contains(a_newConnection)) {
		//Make the connection.
		connections[a_newConnection] = CalculateMovementCost(a_newConnection, a_movementCostMultiplier);
	}

	//Ensure the other Position connects back to this one.
	//if (a_newConnection != nullptr && !a_newConnection->connections.contains(this)) {
	//	a_newConnection->MakeConnection(this, a_movementCostMultiplier);
	//}
}

float CorneliusEngine::AStarPosition::CalculateMovementCost(AStarPosition* a_other, float a_movementCostMultiplier)
{
	float distance = (a_other->position - this->position).MagnitudeFloat();

	return distance * a_movementCostMultiplier;
}
