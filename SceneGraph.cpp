#include "SceneGraph.h"

bool SceneGraph::Initialise(void)
{
	// initialise code for scene graph


	//root = std::make_shared<CustomLinkedNode>(nullptr, nullptr);
	// initialise child nodes
	for (const auto& child : children) {
		if (!child->Initialise()) {
			return false; // if any child fails init return false
		}
	}
	return true;
}

void SceneGraph::Update(const Matrix& worldTransformation)
{
	// update code for scene graph
	SceneNode::Update(worldTransformation);

	// update child nodes recursively
	for (const auto& child : children) {
		child->Update(_cumulativeWorldTransformation);
	}
}

void SceneGraph::Render(void)
{
	// render code for scene graph
	// render child nodes
	for (const auto& child : children) {
		child->Render();
	}
	
}

void SceneGraph::Shutdown(void)
{
	// shutdown code for scene graph
	// shutdown child nodes
	for (const auto& child : children) {
		child->Shutdown();
	}
}

void SceneGraph::Add(SceneNodePointer node)
{
	// add child node to linked structure
	//std::shared_ptr<CustomLinkedNode> newNode = std::make_shared<CustomLinkedNode>(node, root);
	children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node)
{
	// remove child node from linked structure
	children.erase(
		std::remove_if(children.begin(), children.end(),
			[node](const SceneNodePointer& child) { return child == node; }),
		children.end()
	);
}

SceneNodePointer SceneGraph::Find(wstring name)
{
	// find child node by name
	for (const auto& child : children) {
		if (child->GetName() == name) {
			return child;
		}

		// recursive to call to find on child nodes
		SceneNodePointer result = child->Find(name);
		if (result != nullptr) {
			return result;
		}
	}
	return nullptr;
}

