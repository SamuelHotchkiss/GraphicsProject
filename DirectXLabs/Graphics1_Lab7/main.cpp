// D3D LAB 1a "Line Land".
// Author: L.Norri CD DRX, FullSail University

// Introduction:
// Welcome to the first lab of the Direct3D Graphics Programming class.
// This is the ONLY guided lab in this course! 
// Future labs will demand the habits & foundation you develop right now!  
// It is CRITICAL that you follow each and every step. ESPECIALLY THE READING!!!

// TO BEGIN: Open the word document that acompanies this lab and start from the top.

//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************
#include "Object.h"

//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	// TODO: PART 1 STEP 2
	//ID3D11Device*					theDevice;			// used for  creating stuff
	//ID3D11DeviceContext*			devContext;			// binds objects to the pipeline
	ID3D11RenderTargetView*			targetView;			// a pointer to a texture we are reading from
	IDXGISwapChain*					swapChain;			// responsible for presenting / blitting
	D3D11_VIEWPORT					viewPort;
	ID3D11Texture2D*				backBuffer;

	ID3D11Texture2D*				cubeTexture = nullptr;
	ID3D11ShaderResourceView*		shadeResourceView = nullptr;
	ID3D11SamplerState*				samplerState = nullptr;


	ID3D11Buffer*					vertexBuffer = nullptr;
	unsigned int					vertexCount = 0;
	ID3D11InputLayout*				inputLayout = nullptr;

	ID3D11VertexShader*				vShader = nullptr;
	ID3D11PixelShader*				pShader = nullptr;

	XTime							theTime;

	M_4x4 scene[2];
	M_4x4 worldMatrix;
	M_4x4 viewMatrix;
	M_4x4 projMatrix;
	M_4x4 cameraMatrix;

	//Object Star;
	Object Quad;
	Object Cube;
	//Object PointLight;

	ID3D11Buffer* constBuffer = nullptr;

	ID3D11Buffer* allBuffs[2];
	ID3D11Buffer* worldMatrixBuffer = nullptr;
	ID3D11Buffer* sceneMatrixBuffer = nullptr;

	ID3D11Buffer* indexBuffer = nullptr;

	ID3D11DepthStencilView* DepthStencilView;

	POINT prevPoint, currPoint;

	ID3D11RasterizerState* pRasterState;


	ID3D11Buffer* lightBuff = nullptr;
	ID3D11Buffer* ambientBuff = nullptr;
	ID3D11Buffer* ptltBuff = nullptr;

	float ambientLight[4];
	DIR_LIGHT theLight;
	//DIR_LIGHT myPointLight;
	DIR_LIGHT thePtLight;

public:

	struct SIMPLE_VERTEX
	{
		float position[4];

		SIMPLE_VERTEX(float x = 0, float y = 0, float z = 0, float w = 0)
		{
			position[0] = x;
			position[1] = y;
			position[2] = z;
			position[3] = w;
		}
	};

	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
	void MoveCamera(float moveSpd, float rotSpeed, float dt);
	void Resize();
};


//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{
	// ****************** BEGIN WARNING ***********************// 
	// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY! 
	application = hinst;
	appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectXApplication", L"Graphics II Project", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);
	//********************* END WARNING ************************//

	// TODO: PART 1 STEP 3a
	DXGI_SWAP_CHAIN_DESC swapDescription;
	ZeroMemory(&swapDescription, sizeof(swapDescription));
	swapDescription.BufferCount = 1;
	swapDescription.BufferDesc.Width = BACKBUFFER_WIDTH;
	swapDescription.BufferDesc.Height = BACKBUFFER_HEIGHT;
	swapDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDescription.BufferDesc.RefreshRate.Numerator = 60;
	swapDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDescription.OutputWindow = window;
	swapDescription.SampleDesc.Count = 1;
	swapDescription.SampleDesc.Quality = 0;
	swapDescription.Windowed = true;

	// TODO: PART 1 STEP 3b
	D3D_FEATURE_LEVEL list[6] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT test =
		D3D11CreateDeviceAndSwapChain(nullptr,		// which screen to draw on
		D3D_DRIVER_TYPE_HARDWARE,				// use the hardware
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,				// how to treat the device
		list, 6,								// array of feature levels and how many there are (null sets a default value)
		D3D11_SDK_VERSION,						// whatever sdk we are currently using
		&swapDescription,
		&swapChain, &theDevice,					// output parameters
		nullptr, &devContext);

	ID3D11Texture2D* DepthStencil = NULL;
	D3D11_TEXTURE2D_DESC depthDescription;
	depthDescription.Width = BACKBUFFER_WIDTH;
	depthDescription.Height = BACKBUFFER_HEIGHT;
	depthDescription.MipLevels = 1;
	depthDescription.ArraySize = 1;
	depthDescription.Format = DXGI_FORMAT_D32_FLOAT;
	depthDescription.SampleDesc.Count = 1;
	depthDescription.SampleDesc.Quality = 0;
	depthDescription.Usage = D3D11_USAGE_DEFAULT;
	depthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDescription.CPUAccessFlags = 0;
	depthDescription.MiscFlags = 0;

	HRESULT depthHR;
	depthHR = theDevice->CreateTexture2D(&depthDescription, NULL, &DepthStencil);

	//D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
	//ZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
	//// Depth test parameters
	//depthStencilDescription.DepthEnable = true;
	//depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;

	//// Stencil test parameters
	//depthStencilDescription.StencilEnable = true;
	//depthStencilDescription.StencilReadMask = 0xFF;
	//depthStencilDescription.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing
	//depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing
	//depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	//ID3D11DepthStencilState * pDSState;
	//HRESULT DSS;
	//DSS = theDevice->CreateDepthStencilState(&depthStencilDescription, &pDSState);

	//D3D11_DEPTH_STENCIL_VIEW_DESC DSVdesc;
	//ZeroMemory(&DSVdesc, sizeof(DSVdesc));
	//DSVdesc.Format = DXGI_FORMAT_D32_FLOAT;
	//DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//DSVdesc.Texture2D.MipSlice = 0;

	HRESULT DSVHR;
	DSVHR = theDevice->CreateDepthStencilView(DepthStencil, NULL, &DepthStencilView);

	//pDSState->Release();
	DepthStencil->Release();


	// TODO: PART 1 STEP 4
	swapChain->GetBuffer(0, __uuidof(backBuffer), reinterpret_cast<void**>(&backBuffer));
	HRESULT hr = theDevice->CreateRenderTargetView(backBuffer, NULL, &targetView);

	// TODO: PART 1 STEP 5
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = BACKBUFFER_WIDTH;
	viewPort.Height = BACKBUFFER_HEIGHT;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;


	//box.PaintObject(1, Tron_height, Tron_width, Tron_numlevels, Tron_pixels, Tron_leveloffsets);

#if 1

	float angle = 0.0f;
	VERTEX star[12];

	star[0] = { 0.0f, 0.0f, -0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	star[0].CreateNormal();

	for (unsigned int numVerts = 1; numVerts < 11; numVerts++)
	{
		if (numVerts % 2 != 0)
		{
			star[numVerts] = { sinf((angle * 3.14f) / 180.0f), cosf((angle * 3.14f) / 180.0f), 0.0f, 1.0f };
			star[numVerts].CreateNormal();
		}
		else
		{
			star[numVerts] = { sinf((angle * 3.14f) / 180.0f) * 0.3f, cosf((angle * 3.14f) / 180.0f) * 0.3f, 0.0f, 1.0f };
			star[numVerts].CreateNormal();
		}
		angle += 36.0f;
	}
	star[11] = { 0.0f, 0.0f, 0.25f, 1.0f, 0.0f, 1.0f, 1.0f };
	star[11].CreateNormal();


#if 0
	// TODO: PART 2 STEP 3b
	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(bufferDescription));
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = sizeof(OBJ_VERT) * 776;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;

	// TODO: PART 2 STEP 3c
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = Cube_data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;
#endif

	unsigned int Indices[60];
#pragma region IndexArray
#if 1
	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;
	Indices[3] = 0;
	Indices[4] = 2;
	Indices[5] = 3;
	Indices[6] = 0;
	Indices[7] = 3;
	Indices[8] = 4;
	Indices[9] = 0;
	Indices[10] = 4;
	Indices[11] = 5;
	Indices[12] = 0;
	Indices[13] = 5;
	Indices[14] = 6;
	Indices[15] = 0;
	Indices[16] = 6;
	Indices[17] = 7;
	Indices[18] = 0;
	Indices[19] = 7;
	Indices[20] = 8;
	Indices[21] = 0;
	Indices[22] = 8;
	Indices[23] = 9;
	Indices[24] = 0;
	Indices[25] = 9;
	Indices[26] = 10;
	Indices[27] = 0;
	Indices[28] = 10;
	Indices[29] = 1;
	Indices[30] = 11;
	Indices[31] = 1;
	Indices[32] = 10;
	Indices[33] = 11;
	Indices[34] = 10;
	Indices[35] = 9;
	Indices[36] = 11;
	Indices[37] = 9;
	Indices[38] = 8;
	Indices[39] = 11;
	Indices[40] = 8;
	Indices[41] = 7;
	Indices[42] = 11;
	Indices[43] = 7;
	Indices[44] = 6;
	Indices[45] = 11;
	Indices[46] = 6;
	Indices[47] = 5;
	Indices[48] = 11;
	Indices[49] = 5;
	Indices[50] = 4;
	Indices[51] = 11;
	Indices[52] = 4;
	Indices[53] = 3;
	Indices[54] = 11;
	Indices[55] = 3;
	Indices[56] = 2;
	Indices[57] = 11;
	Indices[58] = 2;
	Indices[59] = 1;
#endif
#pragma endregion

#if 0
	D3D11_BUFFER_DESC indexDescription;
	ZeroMemory(&indexDescription, sizeof(indexDescription));
	indexDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexDescription.ByteWidth = 4 * 1692;
	indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDescription.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = Cube_indicies;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
#endif


#if 1
	// TODO: PART 2 STEP 7
	theDevice->CreateVertexShader(VertexSlimShader, sizeof(VertexSlimShader), nullptr, &Cube.pVShader);
	theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &Cube.pPShader);
	
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UVW", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// TODO: PART 2 STEP 8b
	unsigned int numElements = ARRAYSIZE(vLayout);
	theDevice->CreateInputLayout(vLayout, numElements, VertexSlimShader, sizeof(VertexSlimShader), &Cube.pInputLayout);
#if 0
	D3D11_INPUT_ELEMENT_DESC starLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int num = 3;
	theDevice->CreateInputLayout(starLayout, num, StarShader, sizeof(StarShader), &Star.pInputLayout);

	Star.pVertices = star;
	Star.pIndices = Indices;
	Star.numVertices = 12;
	Star.numIndices = 60;
	Star.Initialize();
#endif

	Cube.pVertices = Cube_data;
	Cube.numVertices = 776;
	Cube.pIndices = Cube_indicies;
	Cube.numIndices = 1692;

	Cube.Initialize(nullptr, L"Tron.dds");

	float length = 5.0f;

	OBJ_VERT quad[4] =
	{
		{ -length, 0.0f, length, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ length, 0.0f, length, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ -length, 0.0f, -length, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ length, 0.0f, -length, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }
	};
	
	unsigned int quadIndices[6] =
	{
		0, 1, 3,
		0, 3, 2
	};

	//theDevice->CreateShaderResourceView(cubeTexture, NULL, &Quad.pShaderResource);

	theDevice->CreateVertexShader(VertexSlimShader, sizeof(VertexSlimShader), nullptr, &Quad.pVShader);
	theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &Quad.pPShader);
	
	theDevice->CreateInputLayout(vLayout, 3, VertexSlimShader, sizeof(VertexSlimShader), &Quad.pInputLayout);
	
	Quad.pVertices = quad;
	Quad.numVertices = 4;
	Quad.pIndices = quadIndices;
	Quad.numIndices = 6;
	Quad.Initialize(nullptr, L"Tron.dds");
	
	Translate(Quad.worldMatrix, 0.0f, -1.0f, 0.0f);


	theLight = DIR_LIGHT(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f);
	ambientLight[0] = 0.5f;
	ambientLight[1] = 0.5f;
	ambientLight[2] = 0.5f;
	ambientLight[3] = 1.0f;

	D3D11_BUFFER_DESC lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.ByteWidth = sizeof(theLight);
	lightDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA lightSub;
	ZeroMemory(&lightSub, sizeof(lightSub));
	lightSub.pSysMem = &theLight;
	lightSub.SysMemPitch = 0;
	lightSub.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&lightDesc, &lightSub, &lightBuff);

	D3D11_BUFFER_DESC ambDesc;
	ZeroMemory(&ambDesc, sizeof(ambDesc));
	ambDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ambDesc.ByteWidth = 32;//sizeof(theLight);
	ambDesc.Usage = D3D11_USAGE_DYNAMIC;
	ambDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA ambSub;
	ZeroMemory(&ambSub, sizeof(ambSub));
	ambSub.pSysMem = &ambientLight;
	ambSub.SysMemPitch = 0;
	ambSub.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&ambDesc, &ambSub, &ambientBuff);

	thePtLight = DIR_LIGHT(1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.5f);

	D3D11_BUFFER_DESC ptDesc;
	ZeroMemory(&ptDesc, sizeof(ptDesc));
	ptDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ptDesc.ByteWidth = sizeof(thePtLight);
	ptDesc.Usage = D3D11_USAGE_DYNAMIC;
	ptDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA ptSub;
	ZeroMemory(&ptSub, sizeof(ptSub));
	ptSub.pSysMem = &thePtLight;
	ptSub.SysMemPitch = 0;
	ptSub.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&ptDesc, &ptSub, &ptltBuff);

#endif



#endif

#if 1
	// Matrix setup
	float XScale, YScale, Aspect, zFar, zNear;
	Aspect = 1.0f;
	YScale = 1.0f / tanf((45.0f * 3.14f) / 180.0f);
	XScale = YScale * Aspect;
	zNear = 0.1f;
	zFar = 100.0f;

	projMatrix =
	{
		XScale, 0.0f, 0.0f, 0.0f,
		0.0f, YScale, 0.0f, 0.0f,
		0.0f, 0.0f, zFar / (zFar - zNear), 1.0f,
		0.0f, 0.0f, -(zFar * zNear) / (zFar - zNear), 0.0f
	};

	cameraMatrix = Inverse4x4(viewMatrix);

	scene[0] = /*cameraMatrix*/ viewMatrix;
	scene[1] = projMatrix;

	// Build World Matrix Buffer
	D3D11_BUFFER_DESC worldBuffDesc;
	ZeroMemory(&worldBuffDesc, sizeof(worldBuffDesc));
	worldBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	worldBuffDesc.ByteWidth = sizeof(M_4x4);
	worldBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	worldBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA worldSub;
	worldSub.pSysMem = &worldMatrix;
	worldSub.SysMemPitch = 0;
	worldSub.SysMemSlicePitch = 0;
	
	theDevice->CreateBuffer(&worldBuffDesc, &worldSub, &pWorldBuffer);

	D3D11_BUFFER_DESC sceneBuffDesc;
	ZeroMemory(&sceneBuffDesc, sizeof(sceneBuffDesc));
	sceneBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	sceneBuffDesc.ByteWidth = sizeof(M_4x4) * 2;
	sceneBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sceneBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sceneSub;
	sceneSub.pSysMem = scene;
	sceneSub.SysMemPitch = 0;
	sceneSub.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&sceneBuffDesc, &sceneSub, &sceneMatrixBuffer);
#endif


#if 1 // Rasterstate

	CD3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	rasterDesc.FrontCounterClockwise = TRUE;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.AntialiasedLineEnable = true;
	//rasterDesc.MultisampleEnable = false;

	theDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
#endif

	

}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	devContext->OMSetRenderTargets(1, &targetView, DepthStencilView);
	
	devContext->RSSetViewports(1, &viewPort);
	
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devContext->ClearRenderTargetView(targetView, ClearColor);
	devContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	theTime.Signal();

	float dt = (float)theTime.Delta();

	MoveCamera(5.0f, 200.0f, dt);

	D3D11_MAPPED_SUBRESOURCE ambMap;
	ZeroMemory(&ambMap, sizeof(ambMap));
	devContext->Map(ambientBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &ambMap);
	memcpy(ambMap.pData, &ambientLight, sizeof(ambientLight));
	devContext->Unmap(ambientBuff, 0);

	D3D11_MAPPED_SUBRESOURCE lightMap;
	ZeroMemory(&lightMap, sizeof(lightMap));
	devContext->Map(lightBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &lightMap);
	memcpy(lightMap.pData, &theLight, sizeof(theLight));
	devContext->Unmap(lightBuff, 0);

	D3D11_MAPPED_SUBRESOURCE ptLtMap;
	ZeroMemory(&ptLtMap, sizeof(ptLtMap));
	devContext->Map(ptltBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &ptLtMap);
	memcpy(ptLtMap.pData, &thePtLight, sizeof(thePtLight));
	devContext->Unmap(ptltBuff, 0);

	scene[0] = Inverse4x4(viewMatrix);

	D3D11_MAPPED_SUBRESOURCE sceneMap;
	ZeroMemory(&sceneMap, sizeof(sceneMap));
	devContext->Map(sceneMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sceneMap);
	memcpy(sceneMap.pData, scene, sizeof(scene));
	devContext->Unmap(sceneMatrixBuffer, 0);

	devContext->VSSetConstantBuffers(0, 1, &pWorldBuffer);
	devContext->VSSetConstantBuffers(1, 1, &sceneMatrixBuffer);
	devContext->PSSetConstantBuffers(0, 1, &lightBuff);
	devContext->PSSetConstantBuffers(1, 1, &ambientBuff);
	devContext->PSSetConstantBuffers(2, 1, &ptltBuff);
	//devContext->PSSetConstantBuffers(3, 1, &sceneMatrixBuffer);

	devContext->RSSetState(pRasterState);

#if 0
	worldMatrix = RotateY(50.0f * dt) * worldMatrix;

	devContext->VSSetConstantBuffers(0, 1, &worldMatrixBuffer);

	D3D11_MAPPED_SUBRESOURCE worldMap;
	ZeroMemory(&worldMap, sizeof(worldMap));

	devContext->Map(worldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldMap);
	memcpy(worldMap.pData, &worldMatrix, sizeof(worldMatrix));
	devContext->Unmap(worldMatrixBuffer, 0);

	unsigned int stride = sizeof(VERTEX);
	unsigned int offset = 0;
	devContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offset);

	devContext->PSSetShaderResources(0, 1, &shadeResourceView);

	devContext->VSSetShader(vShader, nullptr, 0);
	devContext->PSSetShader(pShader, nullptr, 0);

	devContext->IASetInputLayout(inputLayout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devContext->DrawIndexed(60, 0, 0);
#endif
	Cube.worldMatrix = RotateY(50.0f * dt) * Cube.worldMatrix;

	//Star.Render();
	Cube.Render();
	Quad.Render();

	swapChain->Present(0, 0);

	return true;
}

void DEMO_APP::MoveCamera(float moveSpd, float rotSpeed, float dt)
{
	GetCursorPos(&currPoint);

	// Translation
	if (GetAsyncKeyState('W'))
	{
		Translate(viewMatrix, 0.0f, 0.0f, moveSpd * dt);
	}
	else if (GetAsyncKeyState('S'))
	{
		Translate(viewMatrix, 0.0f, 0.0f, -moveSpd * dt);
	}
	if (GetAsyncKeyState('A'))
	{
		Translate(viewMatrix, -moveSpd * dt, 0.0f, 0.0f);
	}
	else if (GetAsyncKeyState('D'))
	{
		Translate(viewMatrix, moveSpd * dt, 0.0f, 0.0f);
	}

	if (GetAsyncKeyState('F') & 0x1)
	{
		theLight.color[0] = 0.0f;
		theLight.color[1] = 0.0f;
		theLight.color[2] = 0.0f;
		theLight.color[3] = 0.0f;
	}
	else if (GetAsyncKeyState('G') & 0x1)
	{
		theLight.color[0] = 1.0f;
		theLight.color[1] = 1.0f;
		theLight.color[2] = 0.0f;
		theLight.color[3] = 0.0f;
	}

	if (GetAsyncKeyState('Q') & 0x1)
	{
		ambientLight[0] = 0.0f;
		ambientLight[1] = 0.0f;
		ambientLight[2] = 0.0f;
		ambientLight[3] = 0.0f;
	}
	else if (GetAsyncKeyState('E') & 0x1)
	{
		ambientLight[0] = 0.5f;
		ambientLight[1] = 0.5f;
		ambientLight[2] = 0.5f;
		ambientLight[3] = 1.0f;
	}

	// Rotation
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		float dx = (float)(prevPoint.x - currPoint.x);
		float dy = (float)(prevPoint.y - currPoint.y);

		float oldPosition[3] = { viewMatrix.M[3][0], viewMatrix.M[3][1], viewMatrix.M[3][2] };

		viewMatrix.M[3][0] = 0;
		viewMatrix.M[3][1] = 0;
		viewMatrix.M[3][2] = 0;

		viewMatrix = viewMatrix * RotateY(dx * rotSpeed * dt);
		viewMatrix = RotateX(dy * rotSpeed * dt) * viewMatrix;

		viewMatrix.M[3][0] = oldPosition[0];
		viewMatrix.M[3][1] = oldPosition[1];
		viewMatrix.M[3][2] = oldPosition[2];
	}

	if (GetAsyncKeyState(VK_UP))
	{
		Translate(viewMatrix, 0.0f, moveSpd * dt, 0.0f);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		Translate(viewMatrix, 0.0f, -moveSpd * dt, 0.0f);
	}

	prevPoint = currPoint;
}

void DEMO_APP::Resize()
{

}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{
	devContext->ClearState();

	Cube.Shutdown();
	Quad.Shutdown();

	SAFE_RELEASE(ptltBuff);
	SAFE_RELEASE(ambientBuff);
	SAFE_RELEASE(lightBuff);
	SAFE_RELEASE(cubeTexture);
	SAFE_RELEASE(shadeResourceView);
	SAFE_RELEASE(samplerState);
	SAFE_RELEASE(pRasterState);
	SAFE_RELEASE(constBuffer);
	SAFE_RELEASE(worldMatrixBuffer);
	SAFE_RELEASE(sceneMatrixBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(DepthStencilView);
	SAFE_RELEASE(vShader);
	SAFE_RELEASE(pShader);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(targetView);
	SAFE_RELEASE(backBuffer);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(pWorldBuffer);
	SAFE_RELEASE(devContext);
	SAFE_RELEASE(theDevice);

	UnregisterClass(L"DirectXApplication", application);
	return true;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY!

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && myApp.Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	myApp.ShutDown();
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message)
	{
	case (WM_DESTROY) : { PostQuitMessage(0); }
						break;
	case (WM_SIZE) : { /*something, something, something.... dark side*/}
					 break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


//********************* END WARNING ************************//