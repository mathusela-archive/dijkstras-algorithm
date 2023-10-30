#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

const auto CHAR_LIM = std::numeric_limits<unsigned int>().max();

class Node;

struct Edge {
    Node* node;
    int weight;
};

class Node {
private:
    std::vector<Edge> mConnectedNodes;
    unsigned int mShortestDistance = CHAR_LIM;
    bool mVisited = false;
public:
    auto get_connected() {
        return mConnectedNodes;
    }
    auto get_distance() {
        return mShortestDistance;
    }
    auto get_visited() {
        return mVisited;
    }
    void add_connected(Edge newEdge) {
        mConnectedNodes.push_back(newEdge);
    }
    void set_shortest_distance(unsigned int newDistance) {
        mShortestDistance = newDistance < mShortestDistance ? newDistance : mShortestDistance;
    }
    void set_visited(bool newVisited) {
        mVisited = newVisited;
    }
    void traverse_connected_nodes() {
        for (auto edge : mConnectedNodes) {
            if (!edge.node -> get_visited()) edge.node -> set_shortest_distance(mShortestDistance + edge.weight);
        }
    }
};

class Graph {
private: 
    auto quick_sort(std::vector<Node*> unsorted) {
        if (unsorted.size() <= 1) return unsorted;
        
        auto midpoint = unsorted[std::floor(unsorted.size() / 2.0)]->get_distance();
        
        std::vector<Node*> lowerList; std::vector<Node*> equalList; std::vector<Node*> higherList;

        for (auto element : unsorted) {
            if (element->get_distance() < midpoint) higherList.push_back(element);
            else if (element->get_distance() == midpoint) equalList.push_back(element);
            else lowerList.push_back(element);
        }

        auto sortedLower = quick_sort(lowerList);
        auto sortedHigher = quick_sort(higherList);

        auto combinedList = sortedLower; 
        combinedList.insert(combinedList.end(), equalList.begin(), equalList.end()); 
        combinedList.insert(combinedList.end(), sortedHigher.begin(), sortedHigher.end());

        return combinedList;
    }

    std::vector<Node*> mShortestDistances;

public:
    void run_dijkstras_algorithm() {
        while (mShortestDistances.size() > 0) {
            mShortestDistances = quick_sort(mShortestDistances);
            Node* topNode = mShortestDistances.back();
            topNode -> set_visited(true);
            topNode -> traverse_connected_nodes();
            mShortestDistances.pop_back();
        }
    }

    auto add_node_manual_reconnections(Node& newNode, std::vector<Node*> connectedNodes, std::vector<int> edgeLength) {
        Edge edge1to2;
        for (int i=0; i<connectedNodes.size(); i++) {
            edge1to2 = {&*connectedNodes[i], edgeLength[i]};
            newNode.add_connected(edge1to2);
        }
        mShortestDistances.push_back(&newNode);
    }

    auto add_node_auto_reconnections(Node& newNode, std::vector<Node*> connectedNodes, std::vector<int> edgeLength) {
        Edge edge1to2;
        Edge edge2to1;
        for (int i=0; i<connectedNodes.size(); i++) {
            edge1to2 = {connectedNodes[i], edgeLength[i]};
            edge2to1 = {&newNode, edgeLength[i]};
            newNode.add_connected(edge1to2);
            connectedNodes[i] -> add_connected(edge2to1);
        }
        mShortestDistances.push_back(&newNode);
    }

    auto set_start_node(Node& startNode) {
        startNode.set_shortest_distance(0);
    }

};

int main() {
    Graph graph;

    Node A; Node B; Node C; Node D; Node E; Node F;
    graph.add_node_auto_reconnections(A, std::vector<Node*> {&B, &C, &D}, std::vector<int> {5, 6, 2});
    graph.add_node_auto_reconnections(B, std::vector<Node*> {&E}, std::vector<int> {4});
    graph.add_node_auto_reconnections(C, std::vector<Node*> {&E, &F, &D}, std::vector<int> {4, 8, 2});
    graph.add_node_auto_reconnections(D, std::vector<Node*> {&F}, std::vector<int> {12});
    graph.add_node_auto_reconnections(E, std::vector<Node*> {&F}, std::vector<int> {3});

    graph.set_start_node(A);

    graph.run_dijkstras_algorithm();
    
    std::cout << "A: " << A.get_distance() << std::endl;
    std::cout << "B: " << B.get_distance() << std::endl;
    std::cout << "C: " << C.get_distance() << std::endl;
    std::cout << "D: " << D.get_distance() << std::endl;
    std::cout << "E: " << E.get_distance() << std::endl;
    std::cout << "F: " << F.get_distance() << std::endl;
   
    return 0;
}