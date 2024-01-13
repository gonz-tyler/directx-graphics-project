#pragma once
#include "SceneNode.h"
#include <vector>
#include <memory>

class CustomLinkedNode
{
public:
	std::shared_ptr<SceneNode> data;
	std::weak_ptr<CustomLinkedNode> parent;
	std::vector<std::shared_ptr<CustomLinkedNode>> children;

	CustomLinkedNode(std::shared_ptr<SceneNode> node, std::shared_ptr<CustomLinkedNode> parentNode)
		: data(node), parent(parentNode) {}

	~CustomLinkedNode() {
		children.clear(); // automatically release child nodes due to shared pointer
	}
};