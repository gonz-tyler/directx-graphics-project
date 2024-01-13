#include "TexturedCubeNode.h"
#include "TexturedGeometry.h"
#include "WICTextureLoader.h"

bool TexturedCubeNode::Initialise()
{
    _device = DirectXFramework::GetDXFramework()->GetDevice();
    _deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
    if (_device.Get() == nullptr || _deviceContext.Get() == nullptr)
    {
        return false;
    }
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
    {
        return false;
    }
    BuildVertexNormals();
    BuildGeometryBuffers();
    BuildShaders();
    BuildVertexLayout();
    BuildConstantBuffer();
    BuildTexture(_textureFile);
    return true;

}

void TexturedCubeNode::Render()
{
    //ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

    // Set shaders, vertex buffers, constant buffers, etc.
    // Draw the cube using the _cumulativeWorldTransformation matrix as its position in world space.

    // Set the camera position, focal point, and up vector
    DirectXFramework::GetDXFramework()->SetCameraPosition(Vector3(0.0f, 0.0f, -10.0f));
    DirectXFramework::GetDXFramework()->SetCameraFocalPoint(Vector3(0.0f, 0.0f, 0.0f));
    DirectXFramework::GetDXFramework()->SetCameraUpVector(Vector3(0.0f, 1.0f, 0.0f));

    // Set the view and projection transformations
    Matrix viewTransformation = DirectXFramework::GetDXFramework()->GetViewTransformation();
    Matrix projectionTransformation = DirectXFramework::GetDXFramework()->GetProjectionTransformation();

    CBuffer constantBuffer;
    constantBuffer.WorldViewProjection = _cumulativeWorldTransformation * viewTransformation * projectionTransformation;
    constantBuffer.World = _cumulativeWorldTransformation;
    constantBuffer.AmbientLightColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    //constantBuffer.AmbientLightColour = _materialColour;
    
    constantBuffer.DirectionalLightVector = Vector4(-1.0f, -1.0f, 1.0f, 0.0f);
    constantBuffer.DirectionalLightColour = Vector4(Colors::LightYellow);

    constantBuffer.EyePosition = Vector3(0.0f, 0.0f, -10.0f);
    constantBuffer.SpecularPower = 100.0f;

    constantBuffer.MaterialColour =  _materialColour;

    // Update the constant buffer
    _deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
    _deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &constantBuffer, 0, 0);
    _deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

    // Set the texture to be used by the pixel shader
    _deviceContext->PSSetShaderResources(0, 1, _texture.GetAddressOf());

    // Now render the cube
    // Specify the distance between vertices and the starting point in the vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // Set the vertex buffer and index buffer
    _deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
    _deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // Specify the layout of the polygons
    _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Specify the layout of the input vertices
    _deviceContext->IASetInputLayout(_layout.Get());

    // Specify the vertex and pixel shaders
    _deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
    _deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);

    // Now draw the cube
    _deviceContext->DrawIndexed(ARRAYSIZE(texIndices), 0, 0);
}

void TexturedCubeNode::BuildGeometryBuffers()
{
    _device = DirectXFramework::GetDXFramework()->GetDevice();
    // Add code to build the cube geometry

    // Create vertex buffer
    D3D11_BUFFER_DESC vertexBufferDescriptor = { 0 };
    vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * ARRAYSIZE(texVertices);
    vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDescriptor.CPUAccessFlags = 0;
    vertexBufferDescriptor.MiscFlags = 0;
    vertexBufferDescriptor.StructureByteStride = 0;

    // Now set up a structure that tells DirectX where to get the
    // data for the vertices from
    D3D11_SUBRESOURCE_DATA vertexInitialisationData = { 0 };
    //vertexInitialisationData.pSysMem = &vertices;
    vertexInitialisationData.pSysMem = &texVertices;

    // and create the vertex buffer
    ThrowIfFailed(_device->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

    // Setup the structure that specifies how big the index 
    // buffer should be
    D3D11_BUFFER_DESC indexBufferDescriptor = { 0 };
    indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDescriptor.ByteWidth = sizeof(UINT) * ARRAYSIZE(texIndices);
    indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDescriptor.CPUAccessFlags = 0;
    indexBufferDescriptor.MiscFlags = 0;
    indexBufferDescriptor.StructureByteStride = 0;

    // Now set up a structure that tells DirectX where to get the
    // data for the indices from
    D3D11_SUBRESOURCE_DATA indexInitialisationData;
    //indexInitialisationData.pSysMem = &indices;
    indexInitialisationData.pSysMem = &texIndices;

    // and create the index buffer
    ThrowIfFailed(_device->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void TexturedCubeNode::BuildVertexLayout()
{
    ThrowIfFailed(_device->CreateInputLayout(textureVertexDesc, ARRAYSIZE(textureVertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void TexturedCubeNode::BuildShaders()
{
    //ComPtr<ID3D11Device> device = DirectXFramework::GetDXFramework()->GetDevice();

    DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
    shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ComPtr<ID3DBlob> compilationMessages = nullptr;

    //Compile vertex shader
    HRESULT hr = D3DCompileFromFile(ShaderFileName,
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        VertexShaderName, "vs_5_0",
        shaderCompileFlags, 0,
        _vertexShaderByteCode.GetAddressOf(),
        compilationMessages.GetAddressOf());

    if (compilationMessages.Get() != nullptr)
    {
        // If there were any compilation messages, display them
        MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
    }
    // Even if there are no compiler messages, check to make sure there were no other errors.
    ThrowIfFailed(hr);
    ThrowIfFailed(_device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));

    // Compile pixel shader
    hr = D3DCompileFromFile(ShaderFileName,
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        PixelShaderName, "ps_5_0",
        shaderCompileFlags, 0,
        _pixelShaderByteCode.GetAddressOf(),
        compilationMessages.GetAddressOf());

    if (compilationMessages.Get() != nullptr)
    {
        // If there were any compilation messages, display them
        MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
    }
    ThrowIfFailed(hr);
    ThrowIfFailed(_device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
}

void TexturedCubeNode::BuildConstantBuffer()
{
    // Create the constant buffer
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(CBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void TexturedCubeNode::BuildVertexNormals()
{
    int vertexContributingCount[ARRAYSIZE(texVertices)];
    for (size_t i = 0; i < ARRAYSIZE(texVertices); i++)
    {
        vertexContributingCount[i] = 0;
    }
    // loop through each polygon
    for (size_t i = 0; i < ARRAYSIZE(texIndices); i += 3)
    {
        // calculate normal for each polygon
        // get indices of vertex that makes up polygon
        int index0 = texIndices[i];
        int index1 = texIndices[i + 1];
        int index2 = texIndices[i + 2];

        // construct vector a and b
        Vector3 a = texVertices[index1].Position - texVertices[index0].Position;
        Vector3 b = texVertices[index2].Position - texVertices[index0].Position;

        // calculate normal vector from vector a and b using a x b
        Vector3 polygonNormal = a.Cross(b);


        // add polygon normal to vertex normal for each of the 3 vertices
        texVertices[index0].Normal += polygonNormal;
        vertexContributingCount[index0]++;
        texVertices[index1].Normal += polygonNormal;
        vertexContributingCount[index1]++;
        texVertices[index2].Normal += polygonNormal;
        vertexContributingCount[index2]++;
    }

    // normalize resulting vertex normals
    for (size_t i = 0; i < ARRAYSIZE(texVertices); i++)
    {
        if (vertexContributingCount[i] > 0)
        {
            texVertices[i].Normal /= static_cast<float>(vertexContributingCount[i]);
            texVertices[i].Normal.Normalize();
        }

    }
}
void TexturedCubeNode::Shutdown()
{
    // Required because we called CoInitialize above
    CoUninitialize();
}

void TexturedCubeNode::BuildTexture(wstring textureFile)
{
    // Note that in order to use CreateWICTextureFromFile, we 
    // need to ensure we make a call to CoInitializeEx in our 
    // Initialise method (and make the corresponding call to 
    // CoUninitialize in the Shutdown method).  Otherwise, 
    // the following call will throw an exception
    ThrowIfFailed(CreateWICTextureFromFile(_device.Get(),
        _deviceContext.Get(),
        textureFile.c_str(),
        nullptr,
        _texture.GetAddressOf()
    ));
}






