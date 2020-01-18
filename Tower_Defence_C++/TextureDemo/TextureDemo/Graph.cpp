#include "Graph.h"

#include <cstdlib>
#include <stdlib.h> 
#include <iostream>	//used for debug purposes
using namespace std;

//class used to compare 2 nodeTuple structs
class compareNode
{
public:
	int operator() (const QNode &n1, const QNode &n2)
	{
		return n1.cost > n2.cost;
	}
};

//main constructor
//takes the width, height of graph, as well as a gameobject used to render each node.
Graph::Graph(int nodeWidth, int nodeHeight, GameObject nodeSprite,GLuint l,GLuint p, GLuint pp) : nodeObj(nodeSprite) {
	//initializes the 2d nodes array and nodeMap
	nodes = std::vector<std::vector<Node>>();
	nodeMap = std::map<int, Node*>();

	land = l;
	placementSpace = p;
	plantform = pp;

	//data for setting node positions on, screen. This works best for a 40x30 graph
	//TODO change these values based on graph size.
	float x_b = -4.4f;
	float y_b = 4.4f;
	float movementX = 0.22f;
	float movementY = -0.3f;

	//fills the 2d nodes array with nodes.
	for (int i = 0; i < nodeHeight; i++) {
		std::vector<Node> nodeRow = std::vector<Node>();

		for (int j = 0; j < nodeWidth; j++) {
			//creates each node, starting at (-4.4, 4.4), topLeft, going down, right
			//with x distance .22, y distance .3
			Node newNode = Node(x_b + j* movementX, y_b + i* movementY);
			nodeRow.push_back(newNode);

		}
		nodes.push_back(nodeRow);
	}

	//connects node to each other to form a 4-connected graph with random edge weights
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			
			//Draw the game map
			if (nodes.at(i).at(j).getId() < 40 || nodes.at(i).at(j).getId() > 1159) { nodes.at(i).at(j).setObstacle(); }
			if (nodes.at(i).at(j).getId() >= 121 && nodes.at(i).at(j).getId() < 135) { nodes.at(i).at(j).setObstacle(); }
			if (nodes.at(i).at(j).getId() >= 138 && nodes.at(i).at(j).getId() < 156) { nodes.at(i).at(j).setObstacle(); }
			if (nodes.at(i).at(j).getId() >= 161 && nodes.at(i).at(j).getId() < 175) { nodes.at(i).at(j).setObstacle(); }
			if (nodes.at(i).at(j).getId() >= 178 && nodes.at(i).at(j).getId() < 196) { nodes.at(i).at(j).setObstacle(); }
			if (nodes.at(i).at(j).getId() >= 280 && nodes.at(i).at(j).getId() < 960) {
				if (nodes.at(i).at(j).getId() % 40 == 2 || nodes.at(i).at(j).getId() % 40 == 3 || nodes.at(i).at(j).getId() % 40 == 4) {}
				else if (nodes.at(i).at(j).getId() % 40 == 19 || nodes.at(i).at(j).getId() % 40 == 20 || nodes.at(i).at(j).getId() % 40 == 21) {}
				else if (nodes.at(i).at(j).getId() % 40 == 34 || nodes.at(i).at(j).getId() % 40 == 35 || nodes.at(i).at(j).getId() % 40 == 36) {}
				else if (nodes.at(i).at(j).getId() > 280 && nodes.at(i).at(j).getId() < 460 && nodes.at(i).at(j).getId() % 40 == 13) {}
				else if (nodes.at(i).at(j).getId() > 453 && nodes.at(i).at(j).getId() < 460) {}
				else if (nodes.at(i).at(j).getId() > 484 && nodes.at(i).at(j).getId() < 490) {}
				else if (nodes.at(i).at(j).getId() > 524 && nodes.at(i).at(j).getId() < 530) {}
				else if (nodes.at(i).at(j).getId() > 490 && nodes.at(i).at(j).getId() < 980 && (nodes.at(i).at(j).getId() % 40 == 8 || nodes.at(i).at(j).getId() % 40 == 9)) {}
				else if (nodes.at(i).at(j).getId() > 729 && nodes.at(i).at(j).getId() < 740) {}
				else if (nodes.at(i).at(j).getId() > 769 && nodes.at(i).at(j).getId() < 780) {}
				else if (nodes.at(i).at(j).getId() > 460 && nodes.at(i).at(j).getId() < 470) {}
				else if (nodes.at(i).at(j).getId() > 500 && nodes.at(i).at(j).getId() < 510) {}
				else if (nodes.at(i).at(j).getId() < 666 && nodes.at(i).at(j).getId() % 40 == 30) {}
				else if (nodes.at(i).at(j).getId() > 666 && nodes.at(i).at(j).getId() < 680) {}
				else if (nodes.at(i).at(j).getId() > 666 && (nodes.at(i).at(j).getId() % 40 == 27 || nodes.at(i).at(j).getId() % 40 == 28)) {}
				else if (nodes.at(i).at(j).getId() < 680 && nodes.at(i).at(j).getId() % 40 == 39) {}
				else { nodes.at(i).at(j).setObstacle(); }
			}

			if (nodes.at(i).at(j).getId() >= 1044 && nodes.at(i).at(j).getId() < 1056) { nodes.at(i).at(j).setObstacle(); }
			if (nodes.at(i).at(j).getId() >= 1060 && nodes.at(i).at(j).getId() < 1080 && nodes.at(i).at(j).getId() != 1070 && nodes.at(i).at(j).getId() != 1071 && nodes.at(i).at(j).getId() != 1077 && nodes.at(i).at(j).getId() != 1078) { nodes.at(i).at(j).setObstacle(); }



			//if there exists a node to the right of the current node, link them together
			if (j + 1 < nodes.at(i).size()) {
				int randWeight = 10 + (rand() % 6);	//creates a random weight between 10-15

				Node *n1 = &nodes.at(i).at(j);		//referncec to current node in graph.
				Node *n2 = &nodes.at(i).at(j + 1);	//reference to node to the left of the current node.

				n1->addNode(*n2, randWeight);			//links both nodes together
			}

			//if there exists a node below the current node, link them together
			if (i + 1 < nodes.size()) {
				int randWeight = 10 + (rand() % 6);	//creates a random weight between 10-15

				Node *n1 = &nodes.at(i).at(j);		//referncec to current node in graph.
				Node *n2 = &nodes.at(i + 1).at(j);	//node below the current node.

				n1->addNode(*n2, randWeight);			//links both nodes together
			}
		}
	}

	//adds all nodes to map with nodeId as key and a pointer to the node
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			nodeMap.insert(std::pair<int, Node*>(nodes.at(i).at(j).getId(), &nodes.at(i).at(j)));		
		}
	}

	//set Obstacle cost
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			if (nodes.at(i).at(j).getObstacle()) {
				nodes.at(i).at(j).setEcost(10000);
			}
			else {
				nodes.at(i).at(j).setEcost(1);
			}
		}
	}

	//sets the start/end nodes to the top_left and bottom_right nodes.
	setStart(40);
	setEnd(580);//44
	for (int i = 0; i < 128; i++) { PATH[i] = -1; }
	pathfind();
}

//Loops through array and prints out associated data for each node.
void Graph::printData() {
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			cout << i << ", " << j << " - " << nodes.at(i).at(j).getId() << " - " << &nodes.at(i).at(j);
			cout << " - " << nodes.at(i).at(j).getEdges().size() << endl;
		}
	}
}

//gets mouse input, updates start and end position using that information
void Graph::update() {
	spawnTime -= 1;
	if (spawnTime == 0) {
		int x = rand() % 4 + 1;
		if (x == 1) { setStart(40); }
		if (x == 2) { setStart(79); }
		if (x == 3) { setStart(1119); }
		if (x == 4) { setStart(1159); }
		spawnTime = rand() % 960 + 360;
		pathfind();
	}


}

//returns the id of the node at the screen coordinates. If no node exists, it will return -1
int Graph::getNodeIdFromCoords(double _x, double _y) {
	float start_x = -4.4f;
	float start_y = -4.4f;
	float movementX = 0.22f;
	float movementY = 0.3f;

	start_x -= movementX * .5;
	start_y -= movementY * .5;

	int x = (int)((_x - start_x) / movementX);
	int y = (int)((_y - start_y) / movementY);

	//check if the node is 
	if (y >= nodes.size() || y < 0) {
		return -1;
	}
	else if (x >= nodes.at(y).size() || x < 0) {
		return -1;
	}
	else {
		return nodes.at(y).at(x).getId();
	}
}

//returns the id of the node at the mouse coordinates
int Graph::selectNode(double x, double y) {
	unsigned int window_width_g = 1060;
	unsigned int window_height_g = 720;

	//if the mouse is outside the window, return -1
	if (x < 0 || x > window_width_g || y < 0 || y > window_height_g) {
		return -1;
	}
	else {
		float cursor_x_pos = (x / (float)(window_width_g / 2)) - 1.0f;	//transforms cursor position to screen coordinates
		float cursor_y_pos = (y / (float)(window_height_g / 2)) - 1.0f;

		cursor_x_pos /= 0.2;
		cursor_y_pos /= 0.2;	//transforms cursor position based on screen scale.

		return getNodeIdFromCoords(cursor_x_pos, cursor_y_pos);

	}
}

//renders all the nodes in the graph
void Graph::render(Shader &shader) {

	Node S = getNode(startNodeId);
	nodeObj.setPosition(glm::vec3(S.getX(), S.getY(), 0.0f));
	nodeObj.setTexture(startTex);
	nodeObj.render(shader, 2);

	Node E = getNode(endNodeId);
	nodeObj.setPosition(glm::vec3(E.getX(), E.getY(), 0.0f));
	nodeObj.setTexture(endTex);
	nodeObj.render(shader, 2);


	//goes through each node and renders it, using the provided gameObject
	for (int j = 0; j < nodes.size(); j++) {
		
		if (nodes.size() == 0) {	//if there are no nodes, do nothing.
			return;
		}

		for (int i = 0; i < nodes.at(j).size(); i++) {
			Node currentNode = nodes.at(j).at(i);
			nodeObj.setPosition(glm::vec3(currentNode.getX(), currentNode.getY(), 0.0f));
			if (currentNode.getId() == startNodeId || currentNode.getId() == endNodeId) {
				continue;
			}
			else if (currentNode.getECost() == 666) {
				nodeObj.setTexture(plantform);
			}
			else if (currentNode.getObstacle() == true) {
				nodeObj.setTexture(placementSpace);
			}else{
				nodeObj.setTexture(land);
			}
			glBindTexture(GL_TEXTURE_2D, nodeObj.getTexture());

			nodeObj.render(shader);
		}

	}
}

//returns a reference to the node with the supplied id.
Node& Graph::getNode(int id) {
	
	//because we also store the graph as a map with the id as the key, we can easily reference each node.
	return *nodeMap.at(id);
}


//using zombie-key based approach to Djikstra's algorithm
void Graph::pathfind() {

	//priority queue used in pathfinding.
	//it is created using the NodeTuple struct with a min compare function called compareNode
	priority_queue <QNode, vector<QNode>, compareNode> pq;

	//sets the costs of all nodes to infinity. reset all nodes to be off-path
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			nodes.at(i).at(j).setCost(INT_MAX);
			nodes.at(i).at(j).setOnPath(false);
		}
	}

	//The startnode is added to the pq with cost 0
	QNode startNode = {&getNode(startNodeId), 0};
	pq.push(startNode);

	
	//now that the pq is setup, we can start the algorithm
	//keep in mind that the as the QNode struct has a pointer to the corresponding node
	//some function calls will use pointer syntax (->) 
	
	
	while (!pq.empty()) {
		//get the current lowest-cost node in pq
		QNode lowest = pq.top();

		//if the current node is the end node, done!
		if (lowest.node->getId() == endNodeId) {
			break;
		}

		//OPEN NODE
		vector<Edge> neighbours = lowest.node->getEdges();
		for (int i = 0; i < neighbours.size(); i++) {

			//compute cost to get to neighbouring node
			//cost = the cost to get the corrent node + cost to traverse the edge
			
			Node *n = &getNode(lowest.node->getOtherNode(neighbours.at(i)).getId());
			//Node *n = &(lowest.node->getOtherNode(neighbours.at(i)));
			
			int nodeCost = lowest.cost + neighbours.at(i).cost;

			//if current node cost is higher than calculated, update node, and add QNode to queue			
			if (n->getCost() > nodeCost) {
				n->setCost(nodeCost);	
				n->setPrev(lowest.node);

				QNode updatedNode = {n, nodeCost };
				pq.push(updatedNode);
				
			}
		}


		pq.pop();	//REMOVE NODE FROM QUEUE
	}

	//queue is done, go in reverse from END to START to determine path
	Node* currentNode = getNode(endNodeId).getPrev();

	int i = 0;
	//while the current node isn't null, or the end, mark the current node as on the path
	for (int i = 0; i < 128; i++) {
		PATH[i] = -1;
	}

	while (currentNode != NULL && currentNode->getId() != startNodeId) {
		PATH[127 - i] = currentNode->getId();
		currentNode->setOnPath(true);
		route.push(currentNode);
		totalCost += currentNode->getCost();
		currentNode->setVisited();
		currentNode = currentNode->getPrev();
		i++;
		// Create a list here to store the path !!!
	}
	PATH[127 - i] = startNodeId;

}


int* Graph::getPath() {
	return PATH;
}

int Graph::getNEXT(int CU) {
	for (int i = 127; i > 0; i -= 1) {
		if (PATH[i] == CU) {
			return PATH[i + 1];
		}
	}
}