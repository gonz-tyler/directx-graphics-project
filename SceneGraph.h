#pragma once
#include "SceneNode.h"
#include "CustomLinkedNode.h"

class SceneGraph : public SceneNode
{
public:
	SceneGraph() : SceneNode(L"Root") {};
	SceneGraph(wstring name) : SceneNode(name) {};
	~SceneGraph(void) {};

	virtual bool Initialise(void);
	virtual void Update(const Matrix& worldTransformation);
	virtual void Render(void);
	virtual void Shutdown(void);

	void Add(SceneNodePointer node);
	void Remove(SceneNodePointer node);
	SceneNodePointer Find(wstring name);

private:
	std::vector<SceneNodePointer> children;
};

typedef shared_ptr<SceneGraph>			 SceneGraphPointer;
