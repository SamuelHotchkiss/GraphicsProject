#pragma once

#include "Includes.h"
#include "DDSTextureLoader.h"
#include <thread>

struct VertexOBJ
{
	XMFLOAT3 xyz;
	XMFLOAT2 uv;
	XMFLOAT3 nrm;

	bool operator==(VertexOBJ vert);

	VertexOBJ(float x = 0.0f, float y = 0.0f, float z = 0.0f,
		float u = 0.0f, float v = 0.0f,
		float nX = 0.0f, float nY = 0.0f, float nZ = 0.0f)
	{
		xyz.x = x; xyz.y = y; xyz.z = z;
		uv.x = u; uv.y = v;
		nrm.x = nX; nrm.y = nY; nrm.z = nZ;
	}

};

bool VertexOBJ::operator==(VertexOBJ vert)
{
	if (vert.xyz.x == xyz.x &&
		vert.xyz.y == xyz.y &&
		vert.xyz.z == xyz.z &&
		vert.uv.x == uv.x &&
		vert.uv.y == uv.y &&
		vert.nrm.x == nrm.x &&
		vert.nrm.y == nrm.y &&
		vert.nrm.z == nrm.z)
		return true;

	return false;
}

class Object
{
	ID3D11Buffer*				pVertBuffer = nullptr;		// Vertex Buffer
	ID3D11Buffer*				pIndexBuffer = nullptr;		// Index Buffer
	ID3D11Resource*				pTexture = nullptr;
	ID3D11SamplerState*			pSamplerState = nullptr;

	//thread* modelThread = nullptr;
	//thread* textureThread = nullptr;


	void PaintObject(const wchar_t* file);
	void LoadModelFromFile(const wchar_t* file);
	unsigned int BuildIndices(VertexOBJ);

	static void WeaveModelThread(void*, const wchar_t*);
	static void WeaveTextureThread(void*, const wchar_t*);
public:

	ID3D11InputLayout*			pInputLayout = nullptr;		// Object's input layout CREATE EXTERNALLY

	ID3D11VertexShader*			pVShader = nullptr;		// This object's personal vertex shader CREATE EXTERNALLY
	ID3D11PixelShader*			pPShader = nullptr;		// This object's personal pixel shader CREATE EXTERNALLY
	ID3D11GeometryShader*		pGShader = nullptr;		// This object's personal geometry shader CREATE EXTERNALLY

	ID3D11ShaderResourceView*	pShaderResource = nullptr;		// Resource required by shader (usually some sort of texture)
	ID3D11ShaderResourceView*	pOtherShaderResource = nullptr;

	vector<VertexOBJ> mesh;
	vector<unsigned int> meshIndices;

	bool instanced = false;

	const VertexOBJ*			pVertices = nullptr;		// The actual vertices so we can fill our vertex buffer
	const unsigned int*			pIndices = nullptr;		// The actual indices so we can fill our index buffer
	unsigned int				numVertices = 0;			// The number of vertices
	unsigned int				numIndices = 0;			// The number of indices

	M_4x4						worldMatrix;
	//M_4x4*						worldMatrices = nullptr;
	M_4x4						worldMatrices[4];
	unsigned int				numInstances = 1;

	void Initialize(const wchar_t* modelFile, const wchar_t* textFile);
	void Render();
	void Shutdown();
};

void Object::Initialize(const wchar_t* modelFile = nullptr, const wchar_t* textFile = nullptr)
{

	// (modelFile)
	//LoadModelFromFile(modelFile);
	//if (textFile)
		//PaintObject(textFile);

	if (modelFile && textFile)
	{
		thread ModelThread(WeaveModelThread, this, modelFile);
		thread TextureThread(WeaveTextureThread, this, textFile);

		ModelThread.join();
		TextureThread.join();
	}
	else if (textFile && !modelFile)
		PaintObject(textFile);

	// Build Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(VertexOBJ) * numVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	if (modelFile)
		subData.pSysMem = &mesh[0];
	else
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
	if (modelFile)
		indexData.pSysMem = &meshIndices[0];
	else
		indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);

	D3D11_SAMPLER_DESC samplerDescription;
	ZeroMemory(&samplerDescription, sizeof(samplerDescription));
	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescription.MinLOD = -FLT_MAX;
	samplerDescription.MaxLOD = FLT_MAX;
	samplerDescription.MaxAnisotropy = 1;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;

	theDevice->CreateSamplerState(&samplerDescription, &pSamplerState);

	//Build Instance Buffer
	if (instanced)
	{
		//worldMatrices = new M_4x4[numInstances];

		Translate(worldMatrices[0], 0.0f, 0.0f, 3.0f);
		Translate(worldMatrices[1], 2.0f, 0.0f, 0.0f);
		Translate(worldMatrices[2], -2.0f, 0.0f, 0.0f);
		Translate(worldMatrices[3], 0.0f, 2.0f, 0.0f);

		D3D11_BUFFER_DESC instDesc;
		ZeroMemory(&instDesc, sizeof(instDesc));
		instDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		instDesc.Usage = D3D11_USAGE_DYNAMIC;
		instDesc.ByteWidth = sizeof(M_4x4) * numInstances;
		instDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA instSub;
		instSub.pSysMem = &worldMatrices;
		instSub.SysMemPitch = 0;
		instSub.SysMemSlicePitch = 0;

		theDevice->CreateBuffer(&instDesc, &instSub, &instancedBuffer);
	}
}

void Object::Render()
{
	devContext->PSSetSamplers(0, 1, &pSamplerState);

	// Update world matrix buffer
	D3D11_MAPPED_SUBRESOURCE worldMap;
	ZeroMemory(&worldMap, sizeof(worldMap));

	if (!instanced)
	{
		devContext->VSSetConstantBuffers(0, 1, &pWorldBuffer);

		devContext->Map(pWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldMap);
		memcpy(worldMap.pData, &worldMatrix, sizeof(M_4x4));
		devContext->Unmap(pWorldBuffer, 0);
	}
	else
	{
		devContext->VSSetConstantBuffers(0, 1, &instancedBuffer);

		devContext->Map(instancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldMap);
		memcpy(worldMap.pData, &worldMatrices, sizeof(M_4x4) * numInstances);
		devContext->Unmap(instancedBuffer, 0);
	}

	unsigned int stride = sizeof(VertexOBJ);
	unsigned int offset = 0;
	devContext->IASetVertexBuffers(0, 1, &pVertBuffer, &stride, &offset);
	devContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);

	devContext->GSSetConstantBuffers(0, 1, &pWorldBuffer);
	devContext->GSSetShader(pGShader, NULL, 0);

	devContext->VSSetShader(pVShader, NULL, 0);
	devContext->PSSetShader(pPShader, NULL, 0);

	devContext->PSSetShaderResources(0, 1, &pShaderResource);

	devContext->IASetInputLayout(pInputLayout);
	if (!pGShader)
		devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else
		devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	if (!instanced)
		devContext->DrawIndexed(numIndices, 0, 0);
	else
		devContext->DrawIndexedInstanced(numIndices, numInstances, 0, 0, 0);
}

void Object::PaintObject(const wchar_t* file)
{
	CreateDDSTextureFromFile(theDevice, file, &pTexture, &pShaderResource);
}


void Object::Shutdown()
{
	//delete[] worldMatrices;
	//worldMatrices = nullptr;

	SAFE_RELEASE(pVertBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pSamplerState);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pVShader);
	SAFE_RELEASE(pPShader);
	SAFE_RELEASE(pShaderResource);
	SAFE_RELEASE(pGShader);
	SAFE_RELEASE(pOtherShaderResource);
}

void Object::LoadModelFromFile(const wchar_t* file)
{
	// Temp variables to store values
	vector<unsigned int> vertexIndices, uvIndices, normIndices;
	vector<XMFLOAT3> positions;
	vector<XMFLOAT2> tcoords;
	vector<XMFLOAT3> norms;

	//FILE* fin = fopen(file, "r");
	//
	//while (true)
	//{
	//	char lineheader[128];
	//	int res = fscanf(fin, "%s", lineheader);
	//
	//	if (strcmp(lineheader, "v") == 0)
	//	{
	//		XMFLOAT3 tempVert;
	//		fscanf(fin, "%f %f %f\n", &tempVert.x, &tempVert.y, &tempVert.z);
	//		vertices.push_back(tempVert);
	//	}
	//	else if (strcmp(lineheader, "vt") == 0)
	//	{
	//		XMFLOAT2 tempCoord;
	//		fscanf(fin, "%f %f\n", &tempCoord.x, &tempCoord.y);
	//		tcoords.push_back(tempCoord);
	//	}
	//	else if (strcmp(lineheader, "vn") == 0)
	//	{
	//		XMFLOAT3 tempNorm;
	//		fscanf(fin, "%f %f %f\n", &tempNorm.x, &tempNorm.y, &tempNorm.z);
	//		norms.push_back(tempNorm);
	//	}
	//	else if (strcmp(lineheader, "f") == 0)
	//	{
	//		string vert1, vert2, vert3;
	//		unsigned int vertIndex[3], uvIndex[3], normIndex[3];
	//		int matches = fscanf(fin, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
	//			vertIndex[0], uvIndex[0], normIndex[0],
	//			vertIndex[1], uvIndex[1], normIndex[1],
	//			vertIndex[2], uvIndex[2], normIndex[2]);
	//
	//		vertexIndices.push_back(vertIndex[0]);
	//		vertexIndices.push_back(vertIndex[1]);
	//		vertexIndices.push_back(vertIndex[2]);
	//		uvIndices.push_back(uvIndex[0]);
	//		uvIndices.push_back(uvIndex[1]);
	//		uvIndices.push_back(uvIndex[2]);
	//		normIndices.push_back(normIndex[0]);
	//		normIndices.push_back(normIndex[1]);
	//		normIndices.push_back(normIndex[2]);
	//	}
	//
	//	if (res == EOF)
	//		break;
	//}
	//
	//for (unsigned int i = 0; i < vertexIndices.size(); i++)
	//{
	//	unsigned int vertI = vertexIndices[i];
	//
	//	VertexOBJ outVert;
	//
	//	outVert.xyz = vertices[vertI - 1];
	//	outVert.uv = tcoords[vertI - 1];
	//	outVert.nrm = norms[vertI - 1];
	//
	//	mesh.push_back(outVert);
	//}

	ifstream fin;
	fin.open(file);
	if (fin.is_open())
	{
		char buffer[256] = { 0 };

		while (true)
		{
			fin >> buffer;

			if (fin.eof())
				break;

			if (strcmp(buffer, "v") == 0)
			{
				float x, y, z;
				fin >> x >> y >> z;
				positions.push_back(XMFLOAT3(x, y, z));
			}
			else if (strcmp(buffer, "vn") == 0)
			{
				float x, y, z;
				fin >> x >> y >> z;
				norms.push_back(XMFLOAT3(x, y, z));
			}
			else if (strcmp(buffer, "vt") == 0)
			{
				float u, v;
				fin >> u >> v;
				tcoords.push_back(XMFLOAT2(u, v));
			}
			else if (strcmp(buffer, "f") == 0)
			{
				unsigned int val;
				VertexOBJ vert;
				for (unsigned int i = 0; i < 3; i++)
				{
					fin >> val;
					vert.xyz = positions[val - 1];
					fin.ignore();

					fin >> val;
					vert.uv = tcoords[val - 1];
					fin.ignore();

					fin >> val;
					vert.nrm = norms[val - 1];
					fin.ignore();

					meshIndices.push_back(BuildIndices(vert));
				}
			}

		}

		fin.close();
	}

	numVertices = mesh.size();
	numIndices = meshIndices.size();
}

unsigned int Object::BuildIndices(VertexOBJ vert)
{
	for (unsigned int i = 0; i < mesh.size(); i++)
	{
		if (mesh[i] == vert)
			return i;
	}
	mesh.push_back(vert);
	return mesh.size() - 1;
}

void Object::WeaveModelThread(void* object, const wchar_t* filename)
{
	static_cast<Object*>(object)->LoadModelFromFile(filename);
}

void Object::WeaveTextureThread(void* object, const wchar_t* filename)
{
	static_cast<Object*>(object)->PaintObject(filename);
}