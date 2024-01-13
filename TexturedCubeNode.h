#pragma once
#include "SceneNode.h"
#include <vector>
#include "DirectXFramework.h"


class TexturedCubeNode : public SceneNode
{
public:
    TexturedCubeNode(wstring name)
        : TexturedCubeNode(name, L"woodbox.bmp", Vector4(1.0f, 1.0f, 1.0f, 1.0f)) {}
    TexturedCubeNode(wstring name, const wstring textureFile, const Vector4 materialColour) 
        : SceneNode(name), _textureFile(textureFile), _materialColour(materialColour) {}

    bool Initialise();
    void Render();


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

    ComPtr<ID3D11ShaderResourceView> _texture;

    Vector2                         _textureCoordinates;
    wstring                         _textureFile;

    Vector4                         _materialColour;


    void BuildGeometryBuffers();
    void BuildShaders();
    void BuildVertexLayout();
    void BuildConstantBuffer();

    void BuildVertexNormals();
    void Shutdown();
    void BuildTexture(wstring textureFile);

};
