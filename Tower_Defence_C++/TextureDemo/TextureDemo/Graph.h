#pragma once

#include "Node.h"
#include "Shader.h"
#include "GameObject.h"
#include "Window.h"

#include <stack>
#include <queue>
#include <map>
#include <algorithm>

//struct used to rank various nodes in the priority_queue
struct QNode{
	Node *node;
	int cost;
};

class Graph {
public:
	//constructor. 
	Graph(int nodeWidth, int nodeHeight, GameObject node, GLuint l, GLuint p, GLuint pp);


	//Loops through array and prints out associated data for each node.
	void printData();

	//gets mouse input, updates start and end position using that information
	void update();

	//returns the id of the node at the screen coordinates. If no node exists, it will return -1
	int Graph::getNodeIdFromCoords(double x, double y);

	//returns the id of a node from an x/y coordinate
	int selectNode(double x, double y);

	//returns a reference to the node with the supplied id.
	Node& getNode(int id);

	//renders all the nodes in the graph
	void render(Shader &shader);

	//creates and marks a path from start to end
	void pathfind();

	//Find next node id
	int getNEXT(int i);

	int* Graph::getPath();

	//setters
	inline void setStart(int nodeId) { startNodeId = nodeId; }
	inline void setEnd(int nodeId) { endNodeId = nodeId; }
	inline void popRoute() { route.pop(); }
	inline void setTotalCost(float f) { totalCost -= f; }

	inline void setST(GLuint T) { startTex = T; }
	inline void setET(GLuint T) { endTex = T; }

	//getters
	inline int getStartId() { return startNodeId; }
	inline int getEndId() { return endNodeId; }
	inline std::stack<Node*>  getRoute() { return route; }
	inline float getTotalCost() { return totalCost; }


private:
	//node sprite used to draw each node.
	GameObject nodeObj;
	GLuint land;
	GLuint placementSpace;
	GLuint plantform;
	GLuint startTex;
	GLuint endTex;
	//id of start and endnode
	int startNodeId, endNodeId;
	bool Flag;

	int PATH[128];
	
	std::stack<Node*> route;
	float totalCost;
	//2d vector containing all the nodes in the graph
	std::vector<std::vector<Node>> nodes;
	//map used for easy access to nodes via id
	std::map<int, Node*> nodeMap;

	int spawnTime = 520;
};