
#include <iostream>
#include <ctime>
#include <windows.h>
#include <windef.h>
#include <algorithm>
#include <vector>
#include <fstream>

#include "XTime.h"
#include "Cube.h"
#include "Tron.h"

#define SAFE_RELEASE(x) if(x != nullptr) {x->Release(); x = nullptr;}

#define BACKBUFFER_WIDTH	750
#define BACKBUFFER_HEIGHT	750

using namespace std;

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include "SamsMath.h"

using namespace DirectX;

#include "Trivial_PS.csh"
#include "VertexSlimShader.csh"
#include "StarShader.csh"
#include "StarPixel.csh"

ID3D11Device*					theDevice;			// used for  creating stuff
ID3D11DeviceContext*			devContext;			// binds objects to the pipeline