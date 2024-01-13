#pragma once
#include "SceneNode.h"
#include <vector>
#include "DirectXFramework.h"
#include "GeometricObject.h"


class ModelNode : public SceneNode
{
public:
    ModelNode(wstring name, wstring modelName) : ModelNode(name, modelName, Vector4(0.25f, 0.25f, 0.25f, 1.0f)) {};
    ModelNode(wstring name, wstring modelName,const Vector4 materialColour) : SceneNode(name) {_modelName = modelName, _materialColour = materialColour; };

    bool Initialise();
    void Render();

    //std::wstring                    _modelName;


private:
    ComPtr<ID3D11Device>            _device;
    ComPtr<ID3D11DeviceContext>     _deviceContext;

    ComPtr<ID3D11Buffer>            _vertexBuffer;
    ComPtr<ID3D11Buffer>            _indexBuffer;

    ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
    ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
    ComPtr<ID3D11VertexShader>      _vertexShader;
    ComPtr<ID3D11PixelShader>       _pixelShader;
    ComPtr<ID3D11InputLayout>       _layout;
    ComPtr<ID3D11Buffer>            _constantBuffer;

    Vector4                         _materialColour;

    wstring                    _modelName;

    std::vector<ObjectVertexStruct> vertices;
    std::vector<UINT> indices;

    void BuildGeometryBuffers();
    void BuildShaders();
    void BuildVertexLayout();
    void BuildConstantBuffer();

    void BuildVertexNormals();
    void GenerateGeometry();

};
