#include "DirectXApp.h"
#include "CubeNode.h"
#include "TexturedCubeNode.h"
#include "ModelNode.h"


DirectXApp app;
shared_ptr<CubeNode> cubeNode;
shared_ptr<TexturedCubeNode> texCubeNode;

void DirectXApp::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
    shared_ptr<CubeNode> cubeNode = std::make_shared<CubeNode>(L"Cube1", Vector4(0.5f, 0.25f, 0.25f, 1.0f)); //Vector4(0.5f, 0.25f, 0.25f, 1.0f));
    cubeNode->SetWorldTransform(Matrix::CreateTranslation(Vector3(-2.0f, 0, 0)));  // Set initial position
    sceneGraph->Add(cubeNode);
    shared_ptr<TexturedCubeNode> texCubeNode = std::make_shared<TexturedCubeNode>(L"Cube2", L"woodbox.bmp", Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    texCubeNode->SetWorldTransform(Matrix::CreateTranslation(Vector3(2.0f, 0, 0)));  // Set initial position
    sceneGraph->Add(texCubeNode);
    shared_ptr<ModelNode> modelNode = std::make_shared<ModelNode>(L"Cube3", L"sphere", Vector4(0.5f, 0.25f, 1.0f, 1.0f)); //Vector4(0.25f, 0.5f, 0.25f, 1.0f));
    modelNode->SetWorldTransform(Matrix::CreateTranslation(Vector3(0, 0, -2.0f)));  // Set initial position z=-2.0f
    sceneGraph->Add(modelNode);
    cubeNode = std::make_shared<CubeNode>(L"Cube4", Vector4(0.25f, 0.5f, 0.25f, 1.0f)); // Vector4(0.25f, 0.5f, 0.25f, 1.0f));
    cubeNode->SetWorldTransform(Matrix::CreateTranslation(Vector3(0, 0, 2.0f)));  // Set initial position
    sceneGraph->Add(cubeNode);
    SceneGraphPointer centreSceneGraph = make_shared<SceneGraph>(L"Centre");
    sceneGraph->Add(centreSceneGraph);
    texCubeNode = std::make_shared<TexturedCubeNode>(L"Cube5", L"woodbox.bmp", Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    texCubeNode->SetWorldTransform(Matrix::CreateTranslation(Vector3(0, 1.0f, 0)));  // Set initial position
    centreSceneGraph->Add(texCubeNode);
    cubeNode = std::make_shared<CubeNode>(L"Cube6", Vector4(0.25f, 0.25f, 0.5f, 1.0f)); //Vector4(0.25f, 0.25f, 0.5f, 1.0f));
    cubeNode->SetWorldTransform(Matrix::CreateTranslation(Vector3(0, -1.0f, 0)));  // Set initial position
    centreSceneGraph->Add(cubeNode);

    _rotationAngle = 0;
    _yOffset = 0.0f;
    _isGoingUp = false; 
}

void DirectXApp::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
    _rotationAngle += 0.02f;
    sceneGraph->SetWorldTransform(Matrix::CreateRotationY(_rotationAngle + XM_PI / 180.0f));
    if (_isGoingUp)
    {
        _yOffset += 0.05f;
        if (_yOffset > 3.0f)
        {
            _isGoingUp = false;
        }
    }
    else
    {
        _yOffset -= 0.05f;
        if (_yOffset < -3.0f)
        {
            _isGoingUp = true;
        }
    }
    SceneNodePointer centre = sceneGraph->Find(L"Centre");
    centre->SetWorldTransform(Matrix::CreateTranslation(Vector3(0, _yOffset, 0)));
}


