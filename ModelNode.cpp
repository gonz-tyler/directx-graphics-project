#include "ModelNode.h"
#include "ModelGeometry.h"

//CubeNode::CubeNode(wstring name, Vector4 color) : SceneNode(name), _materialColor(color)
//{
    // init members
//    _eyePosition = Vector3(0.0f, 0.0f, -5.0f);
//    _focalPointPosition = Vector3(0.0f, 0.0f, 0.0f);
//    _upVector = Vector3(0.0f, 1.0f, 0.0f);
//}

bool ModelNode::Initialise()
{
    _device = DirectXFramework::GetDXFramework()->GetDevice();
    _deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
    if (_device.Get() == nullptr || _deviceContext.Get() == nullptr)
    {
        return false;
    }
    GenerateGeometry();
    BuildVertexNormals();
    BuildGeometryBuffers();
    BuildShaders();
    BuildVertexLayout();
    BuildConstantBuffer();
    return true;

}

void ModelNode::GenerateGeometry()
{
    if (_modelName == L"cube") {
        // Generate cube
        ComputeBox(vertices, indices, Vector3(2.0f, 2.0f, 2.0f));
    }
    else if (_modelName == L"sphere") {
        // Generate sphere
        ComputeSphere(vertices, indices, 2.0f, 64);
    }
    else if (_modelName == L"cylinder") {
        // Generate cylinder
        ComputeCylinder(vertices, indices, 4.0f, 2.0f, 64);
    }
    else if (_modelName == L"cone") {
        // Generate cone
        ComputeCone(vertices, indices, 2.0f, 4.0f, 64);
    }
    else if (_modelName == L"teapot") {
        // Generate teapot
        ComputeTeapot(vertices, indices, 2.0f);
    }
    else {
        // Default generate cube if none of the words match
        ComputeBox(vertices, indices, Vector3(2.0f, 2.0f, 2.0f));
    }
}

void ModelNode::Render()
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
    constantBuffer.SpecularPower = 50.0f;

    constantBuffer.MaterialColour = _materialColour;// Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    // Update the constant buffer
    _deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
    _deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &constantBuffer, 0, 0);
    _deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

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
    _deviceContext->DrawIndexed((indices.size()), 0, 0);
}

void ModelNode::BuildGeometryBuffers()
{
    _device = DirectXFramework::GetDXFramework()->GetDevice();

    // Create vertex buffer
    D3D11_BUFFER_DESC vertexBufferDescriptor = { 0 };
    vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * (vertices.size());
    vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDescriptor.CPUAccessFlags = 0;
    vertexBufferDescriptor.MiscFlags = 0;
    vertexBufferDescriptor.StructureByteStride = 0;

    // Now set up a structure that tells DirectX where to get the
    // data for the vertices from
    D3D11_SUBRESOURCE_DATA vertexInitialisationData = { 0 };
    //vertexInitialisationData.pSysMem = &vertices;
    vertexInitialisationData.pSysMem = vertices.data();

    // and create the vertex buffer
    ThrowIfFailed(_device->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

    // Setup the structure that specifies how big the index 
    // buffer should be
    D3D11_BUFFER_DESC indexBufferDescriptor = { 0 };
    indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDescriptor.ByteWidth = sizeof(UINT) * (indices.size());
    indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDescriptor.CPUAccessFlags = 0;
    indexBufferDescriptor.MiscFlags = 0;
    indexBufferDescriptor.StructureByteStride = 0;

    // Now set up a structure that tells DirectX where to get the
    // data for the indices from
    D3D11_SUBRESOURCE_DATA indexInitialisationData;
    //indexInitialisationData.pSysMem = &indices;
    indexInitialisationData.pSysMem = indices.data();

    // and create the index buffer
    ThrowIfFailed(_device->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void ModelNode::BuildVertexLayout()
{
    ThrowIfFailed(_device->CreateInputLayout(modelVertexDesc, ARRAYSIZE(modelVertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void ModelNode::BuildShaders()
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

void ModelNode::BuildConstantBuffer()
{
    // Create the constant buffer
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(CBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void ModelNode::BuildVertexNormals()
{
    std::vector<int> vertexContributingCount(vertices.size(), 0);

    // loop through each polygon
    for (int i = 0; i < (indices.size()); i += 3)
    {
        // calculate normal for each polygon
        // get indices of vertex that makes up polygon
        int index0 = indices[i];
        int index1 = indices[i + 1];
        int index2 = indices[i + 2];

        // construct vector a and b
        Vector3 a = vertices[index1].Position - vertices[index0].Position;
        Vector3 b = vertices[index2].Position - vertices[index0].Position;

        // calculate normal vector from vector a and b using a x b
        Vector3 polygonNormal = a.Cross(b);


        // add polygon normal to vertex normal for each of the 3 vertices
        vertices[index0].Normal += polygonNormal;
        vertexContributingCount[index0]++;
        vertices[index1].Normal += polygonNormal;
        vertexContributingCount[index1]++;
        vertices[index2].Normal += polygonNormal;
        vertexContributingCount[index2]++;
    }

    // normalize resulting vertex normals
    for (size_t i = 0; i < (vertices.size()); i++)
    {
        if (vertexContributingCount[i] > 0)
        {
            vertices[i].Normal /= static_cast<float>(vertexContributingCount[i]);
            vertices[i].Normal.Normalize();
        }

    }
}

