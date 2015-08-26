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

	Object Quad;

	Object Pyramid;
	Object SkyBox;
	Object QuadSeed;

	ID3D11Buffer* constBuffer = nullptr;

	ID3D11Buffer* allBuffs[2];

	ID3D11Buffer* worldMatrixBuffer = nullptr;
	ID3D11Buffer* sceneMatrixBuffer = nullptr;
	//ID3D11Buffer* instancedBuffer = nullptr;

	ID3D11Buffer* indexBuffer = nullptr;

	ID3D11DepthStencilView* DepthStencilView;

	POINT prevPoint, currPoint;

	ID3D11RasterizerState* pRasterState;
	ID3D11RasterizerState* pOtherState;


	ID3D11Buffer* lightBuff = nullptr;
	ID3D11Buffer* ambientBuff = nullptr;
	ID3D11Buffer* ptltBuff = nullptr;
	ID3D11Buffer* spotBuff = nullptr;

	float ambientLight[4];
	DIR_LIGHT theLight;
	//DIR_LIGHT myPointLight;
	DIR_LIGHT thePtLight;
	SPOT_LIGHT theSpotLight;

	int witchLight = 0;
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
	swapDescription.SampleDesc.Count = 4;
	swapDescription.SampleDesc.Quality = D3D11_CENTER_MULTISAMPLE_PATTERN;
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
	depthDescription.SampleDesc.Count = 4;
	depthDescription.SampleDesc.Quality = D3D11_CENTER_MULTISAMPLE_PATTERN;
	depthDescription.Usage = D3D11_USAGE_DEFAULT;
	depthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDescription.CPUAccessFlags = 0;
	depthDescription.MiscFlags = 0;

	HRESULT depthHR;
	depthHR = theDevice->CreateTexture2D(&depthDescription, NULL, &DepthStencil);

#pragma region STENCIL
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
#pragma endregion

	HRESULT DSVHR;
	DSVHR = theDevice->CreateDepthStencilView(DepthStencil, NULL, &DepthStencilView);

	//pDSState->Release();
	DepthStencil->Release();


	swapChain->GetBuffer(0, __uuidof(backBuffer), reinterpret_cast<void**>(&backBuffer));
	HRESULT hr = theDevice->CreateRenderTargetView(backBuffer, NULL, &targetView);

	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = BACKBUFFER_WIDTH;
	viewPort.Height = BACKBUFFER_HEIGHT;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;


#if 1

	theDevice->CreateVertexShader(SkyVShader, sizeof(SkyVShader), nullptr, &SkyBox.pVShader);
	theDevice->CreatePixelShader(SkyPShader, sizeof(SkyPShader), nullptr, &SkyBox.pPShader);

	D3D11_INPUT_ELEMENT_DESC skyLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	theDevice->CreateInputLayout(skyLayout, ARRAYSIZE(skyLayout), SkyVShader, sizeof(SkyVShader), &SkyBox.pInputLayout);

	SkyBox.Initialize(L"sky.obj", L"SkyboxOcean.dds");


#if 1
	theDevice->CreateVertexShader(InstanceVShader, sizeof(InstanceVShader), nullptr, &Pyramid.pVShader);
	theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &Pyramid.pPShader);

	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// TODO: PART 2 STEP 8b
	unsigned int numElements = ARRAYSIZE(vLayout);
	theDevice->CreateInputLayout(vLayout, numElements, InstanceVShader, sizeof(InstanceVShader), &Pyramid.pInputLayout);

	Pyramid.numInstances = 4;
	Pyramid.instanced = true;
	Pyramid.Initialize(L"test_pyramid_triangle.obj", L"metallock.dds");

	Translate(Pyramid.worldMatrix, 0.0f, 0.0f, 3.0f);

	float length = 5.0f;

	VertexOBJ quad[4] =
	{
		{ -length, 0.0f, length, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ length, 0.0f, length, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ -length, 0.0f, -length, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ length, 0.0f, -length, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f }
	};

	unsigned int quadIndices[6] =
	{
		0, 1, 3,
		0, 3, 2
	};

	theDevice->CreateVertexShader(VertexSlimShader, sizeof(VertexSlimShader), nullptr, &Quad.pVShader);
	theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &Quad.pPShader);

	theDevice->CreateInputLayout(skyLayout, 3, VertexSlimShader, sizeof(VertexSlimShader), &Quad.pInputLayout);

	Quad.pVertices = quad;
	Quad.numVertices = 4;
	Quad.pIndices = quadIndices;
	Quad.numIndices = 6;
	Quad.Initialize(nullptr, L"checkerboard.dds");

	Translate(Quad.worldMatrix, 0.0f, -1.0f, 3.0f);

	theDevice->CreateVertexShader(PassToGeoVShader, sizeof(PassToGeoVShader), nullptr, &QuadSeed.pVShader);
	theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &QuadSeed.pPShader);
	theDevice->CreateGeometryShader(QuadCreator, sizeof(QuadCreator), nullptr, &QuadSeed.pGShader);

	theDevice->CreateInputLayout(skyLayout, 3, PassToGeoVShader, sizeof(PassToGeoVShader), &QuadSeed.pInputLayout);

	VertexOBJ seed[1] = { { 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f } };

	QuadSeed.pVertices = seed;
	QuadSeed.numVertices = 6;
	QuadSeed.pIndices = quadIndices;
	QuadSeed.numIndices = 6;
	QuadSeed.Initialize(nullptr, L"checkerboard.dds");
	Translate(QuadSeed.worldMatrix, 0.0f, 0.0f, 3.0f);

#pragma region THE_LIGHTS
	theLight = DIR_LIGHT(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f);
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

	thePtLight = DIR_LIGHT(0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f, 0.5f);

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

	theSpotLight = SPOT_LIGHT(-0.5f, 5.0f, 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.99f);

	D3D11_BUFFER_DESC spotDesc;
	ZeroMemory(&spotDesc, sizeof(spotDesc));
	spotDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	spotDesc.ByteWidth = sizeof(theSpotLight);
	spotDesc.Usage = D3D11_USAGE_DYNAMIC;
	spotDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA spotSub;
	ZeroMemory(&spotSub, sizeof(spotSub));
	spotSub.pSysMem = &theSpotLight;

	theDevice->CreateBuffer(&spotDesc, &spotSub, &spotBuff);
#pragma endregion

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
	SkyBox.worldMatrix.M[3][0] = viewMatrix.M[3][0];
	SkyBox.worldMatrix.M[3][1] = viewMatrix.M[3][1];
	SkyBox.worldMatrix.M[3][2] = viewMatrix.M[3][2];

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

	/*D3D11_BUFFER_DESC instBuffDesc;
	ZeroMemory(&instBuffDesc, sizeof(instBuffDesc));
	instBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	instBuffDesc.ByteWidth = sizeof(M_4x4) * Pyramid.numInstances;
	instBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	instBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA instSub;
	instSub.pSysMem = Pyramid.worldMatrices;
	instSub.SysMemPitch = 0;
	instSub.SysMemSlicePitch = 0;

	theDevice->CreateBuffer(&instBuffDesc, &instSub, &instancedBuffer);*/

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
	rasterDesc.MultisampleEnable = true;

	theDevice->CreateRasterizerState(&rasterDesc, &pRasterState);

	CD3D11_RASTERIZER_DESC otherDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	otherDesc.FrontCounterClockwise = FALSE;
	otherDesc.CullMode = D3D11_CULL_FRONT;
	otherDesc.AntialiasedLineEnable = true;
	//rasterDesc.MultisampleEnable = true;

	theDevice->CreateRasterizerState(&otherDesc, &pOtherState);
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

	D3D11_MAPPED_SUBRESOURCE spotMap;
	ZeroMemory(&spotMap, sizeof(spotMap));
	devContext->Map(spotBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &spotMap);
	memcpy(spotMap.pData, &theSpotLight, sizeof(theSpotLight));
	devContext->Unmap(spotBuff, 0);

	scene[0] = Inverse4x4(viewMatrix);

	D3D11_MAPPED_SUBRESOURCE sceneMap;
	ZeroMemory(&sceneMap, sizeof(sceneMap));
	devContext->Map(sceneMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sceneMap);
	memcpy(sceneMap.pData, scene, sizeof(scene));
	devContext->Unmap(sceneMatrixBuffer, 0);

	devContext->VSSetConstantBuffers(1, 1, &sceneMatrixBuffer);
	devContext->PSSetConstantBuffers(0, 1, &lightBuff);
	devContext->PSSetConstantBuffers(1, 1, &ambientBuff);
	devContext->PSSetConstantBuffers(2, 1, &ptltBuff);
	devContext->PSSetConstantBuffers(3, 1, &spotBuff);
	//devContext->GSSetConstantBuffers(0, 1, &pWorldBuffer);
	devContext->GSSetConstantBuffers(1, 1, &sceneMatrixBuffer);

	Pyramid.worldMatrices[0] = RotateY(50.0f * dt) * Pyramid.worldMatrices[0];
	Pyramid.worldMatrices[1] = RotateZ(50.0f * dt) * Pyramid.worldMatrices[1];
	Pyramid.worldMatrices[2] = RotateX(50.0f * dt) * Pyramid.worldMatrices[2];
	Pyramid.worldMatrices[3] = RotateY(200.0f * dt) * RotateZ(200.0f * dt) * RotateX(200.0f * dt) * Pyramid.worldMatrices[3];

	//SKYBOX RENDERING/////
	devContext->RSSetState(pOtherState);
	//SkyBox.worldMatrix = viewMatrix;

	SkyBox.worldMatrix.M[3][0] = viewMatrix.M[3][0];
	SkyBox.worldMatrix.M[3][1] = viewMatrix.M[3][1];
	SkyBox.worldMatrix.M[3][2] = viewMatrix.M[3][2];

	SkyBox.Render();
	devContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	///////////////////////

	devContext->RSSetState(pRasterState);

	Pyramid.Render();
	Quad.Render();
	QuadSeed.Render();

	swapChain->Present(0, 0);

	return true;
}

void DEMO_APP::MoveCamera(float moveSpd, float rotSpeed, float dt)
{
	GetCursorPos(&currPoint);

	if (GetAsyncKeyState('1') & 0x1) { witchLight = 1; }
	else if (GetAsyncKeyState('2') & 0x1) { witchLight = 2; }
	else if (GetAsyncKeyState('3') & 0x1) { witchLight = 3; }
	else if (GetAsyncKeyState('0') & 0x1) { witchLight = 0; }

	if (witchLight == 1)
	{
		if (GetAsyncKeyState('Q') & 0x1)
		{
			theLight.color[0] = 0.0f;
			theLight.color[1] = 0.0f;
			theLight.color[2] = 0.0f;
			theLight.color[3] = 0.0f;
		}
		else if (GetAsyncKeyState('E') & 0x1)
		{
			theLight.color[0] = 1.0f;
			theLight.color[1] = 0.0f;
			theLight.color[2] = 0.0f;
			theLight.color[3] = 1.0f;
		}

		// Translation
		if (GetAsyncKeyState('W'))
		{
			theLight.direction[3] += moveSpd * dt;
		}
		else if (GetAsyncKeyState('S'))
		{
			theLight.direction[3] -= moveSpd * dt;
		}
		if (GetAsyncKeyState('A'))
		{
			theLight.direction[0] -= moveSpd * dt;
		}
		else if (GetAsyncKeyState('D'))
		{
			theLight.direction[0] += moveSpd * dt;
		}
	}
	else if (witchLight == 2)
	{
		if (GetAsyncKeyState('Q') & 0x1)
		{
			thePtLight.color[0] = 0.0f;
			thePtLight.color[1] = 0.0f;
			thePtLight.color[2] = 0.0f;
			thePtLight.color[3] = 0.0f;
		}
		else if (GetAsyncKeyState('E') & 0x1)
		{
			thePtLight.color[0] = 0.0f;
			thePtLight.color[1] = 0.0f;
			thePtLight.color[2] = 1.0f;
			thePtLight.color[3] = 1.0f;
		}

		// Translation
		if (GetAsyncKeyState('W'))
		{
			thePtLight.direction[2] += moveSpd * dt;
		}
		else if (GetAsyncKeyState('S'))
		{
			thePtLight.direction[2] -= moveSpd * dt;
		}
		if (GetAsyncKeyState('A'))
		{
			thePtLight.direction[0] -= moveSpd * dt;
		}
		else if (GetAsyncKeyState('D'))
		{
			thePtLight.direction[0] += moveSpd * dt;
		}
	}

	else if (witchLight == 0)
	{
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

	//Cube.Shutdown();
	Quad.Shutdown();
	Pyramid.Shutdown();
	SkyBox.Shutdown();
	QuadSeed.Shutdown();

	SAFE_RELEASE(pOtherState);
	SAFE_RELEASE(spotBuff);
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
	SAFE_RELEASE(instancedBuffer);
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