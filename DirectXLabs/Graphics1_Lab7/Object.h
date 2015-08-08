#pragma once

#include "Includes.h"

struct VertexOBJ
{
	XMFLOAT3 xyz;
	XMFLOAT2 uv;
	XMFLOAT3 nrm;
};

class Object
{
	ID3D11Buffer*				pVertBuffer			= nullptr;		// Vertex Buffer
	ID3D11Buffer*				pIndexBuffer		= nullptr;		// Index Buffer
	ID3D11Buffer*				pWorldBuffer		= nullptr;
	ID3D11Texture2D*			pTexture			= nullptr;
	ID3D11SamplerState*			pSamplerState		= nullptr;

	
public:

	ID3D11InputLayout*			pInputLayout		= nullptr;		// Object's input layout CREATE EXTERNALLY

	ID3D11VertexShader*			pVShader			= nullptr;		// This object's personal vertex shader CREATE EXTERNALLY
	ID3D11PixelShader*			pPShader			= nullptr;		// This object's personal pixel shader CREATE EXTERNALLY
	ID3D11ShaderResourceView*	pShaderResource		= nullptr;		// Resource required by shader (usually some sort of texture)
	// Convert to vertex structure
	vector<VertexOBJ> mesh;

	VERTEX*						pVertices			= nullptr;		// The actual vertices so we can fill our vertex buffer
	unsigned int*				pIndices			= nullptr;		// The actual indices so we can fill our index buffer
	unsigned int				numVertices			= 0;			// The number of vertices
	unsigned int				numIndices			= 0;			// The number of indices

	//XMMATRIX					worldMatrix;
	M_4x4						worldMatrix;
	//D3D11_INPUT_ELEMENT_DESC	inputDescription;

	void Initialize();
	void Render();
	void PaintObject(unsigned int arrSize, unsigned int height, unsigned int width,
		unsigned int mipLvls, const unsigned int *texture, const unsigned int *offsets);
	void Shutdown();
	void LoadModelFromFile(const char* file);
};

void Object::Initialize()
{
	// Initialize World Matrix
	//worldMatrix = XMMatrixIdentity();
	//worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 3.0f) * worldMatrix;
	Translate(worldMatrix, 0.0f, 0.0f, 3.0f);

	// Build Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX) * numVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = pVertices;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&vertexBufferDesc, &subData, &pVertBuffer);

	// Build Index Buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = 4 * numIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);

	//worldMatrix = XMMatrixIdentity();
	//worldMatrix = XMMatrixTranslation(3.0f, 0.0f, 0.0f) * worldMatrix;

	// Build World Matrix Buffer
	D3D11_BUFFER_DESC worldBuffDesc;
	ZeroMemory(&worldBuffDesc, sizeof(worldBuffDesc));
	worldBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	worldBuffDesc.ByteWidth = sizeof(XMMATRIX);
	worldBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	worldBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA worldSub;
	worldSub.pSysMem = &worldMatrix;
	worldSub.SysMemPitch = 0;
	worldSub.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&worldBuffDesc, &worldSub, &pWorldBuffer);
}

void Object::Render()
{
	devContext->VSSetConstantBuffers(0, 1, &pWorldBuffer);

	// Update

	// Update world matrix buffer
	D3D11_MAPPED_SUBRESOURCE worldMap;
	ZeroMemory(&worldMap, sizeof(worldMap));

	devContext->Map(pWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldMap);
	memcpy(worldMap.pData, &worldMatrix, sizeof(worldMatrix));
	devContext->Unmap(pWorldBuffer, 0);

	unsigned int stride = sizeof(VERTEX);
	unsigned int offset = 0;
	devContext->IASetVertexBuffers(0, 1, &pVertBuffer, &stride, &offset);
	devContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);

	devContext->VSSetShader(pVShader, NULL, 0);
	devContext->PSSetShader(pPShader, NULL, 0);
	devContext->PSSetShaderResources(0, 1, &pShaderResource);

	devContext->IASetInputLayout(pInputLayout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devContext->DrawIndexed(numIndices, 0, 0);
}

void Object::PaintObject(unsigned int arrSize, unsigned int height, unsigned int width,
						   unsigned int mipLvls, const unsigned int *texture, const unsigned int *offsets)
{
	D3D11_TEXTURE2D_DESC textDesc;
	ZeroMemory(&textDesc, sizeof(textDesc));
	textDesc.ArraySize = arrSize;
	textDesc.Height = height;
	textDesc.Width = width;
	textDesc.MipLevels = mipLvls;
	textDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textDesc.SampleDesc.Count = 1;

	D3D11_SUBRESOURCE_DATA* textData = new D3D11_SUBRESOURCE_DATA[mipLvls];
	ZeroMemory(textData, sizeof(*textData));
	for (unsigned int i = 0; i < mipLvls; i++)
	{
		textData[i].pSysMem = &texture[offsets[i]];
		textData[i].SysMemPitch = 4 * (width >> i);
	}

	theDevice->CreateTexture2D(&textDesc, textData, &pTexture);
	theDevice->CreateShaderResourceView(pTexture, NULL, &pShaderResource);

	delete[] textData;
	textData = nullptr;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	theDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
}

void Object::Shutdown()
{
	SAFE_RELEASE(pVertBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pWorldBuffer);
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pSamplerState);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pVShader);
	SAFE_RELEASE(pPShader);
	SAFE_RELEASE(pShaderResource);
}

void Object::LoadModelFromFile(const char* file)
{
	// Temp variables to store values
	vector<unsigned int> vertexIndices, uvIndices, normIndices;
	vector<XMFLOAT3> vertices;
	vector<XMFLOAT2> tcoords;
	vector<XMFLOAT3> norms;

	FILE* fin = fopen(file, "r");

	while (true)
	{
		char lineheader[128];
		int res = fscanf(fin, "%s", lineheader);

		if (strcmp(lineheader, "v") == 0)
		{
			XMFLOAT3 tempVert;
			fscanf(fin, "%f %f %f\n", &tempVert.x, &tempVert.y, &tempVert.z);
			vertices.push_back(tempVert);
		}
		else if (strcmp(lineheader, "vt") == 0)
		{
			XMFLOAT2 tempCoord;
			fscanf(fin, "%f %f\n", &tempCoord.x, &tempCoord.y);
			tcoords.push_back(tempCoord);
		}
		else if (strcmp(lineheader, "vn") == 0)
		{
			XMFLOAT3 tempNorm;
			fscanf(fin, "%f %f %f\n", &tempNorm.x, &tempNorm.y, &tempNorm.z);
			norms.push_back(tempNorm);
		}
		else if (strcmp(lineheader, "f") == 0)
		{
			string vert1, vert2, vert3;
			unsigned int vertIndex[3], uvIndex[3], normIndex[3];
			int matches = fscanf(fin, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				vertIndex[0], uvIndex[0], normIndex[0],
				vertIndex[1], uvIndex[1], normIndex[1],
				vertIndex[2], uvIndex[2], normIndex[2]);

			vertexIndices.push_back(vertIndex[0]);
			vertexIndices.push_back(vertIndex[1]);
			vertexIndices.push_back(vertIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normIndices.push_back(normIndex[0]);
			normIndices.push_back(normIndex[1]);
			normIndices.push_back(normIndex[2]);
		}

		if (res == EOF)
			break;
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertI = vertexIndices[i];

		VertexOBJ outVert;

		outVert.xyz = vertices[vertI - 1];
		outVert.uv = tcoords[vertI - 1];
		outVert.nrm = norms[vertI - 1];

		mesh.push_back(outVert);
	}
}