#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer DIR_LIGHT
// {
//
//   float3 lightDir;                   // Offset:    0 Size:    12
//   float dirNada;                     // Offset:   12 Size:     4 [unused]
//   float4 lightColor;                 // Offset:   16 Size:    16
//
// }
//
// cbuffer AMBIENT
// {
//
//   float4 ambientColor;               // Offset:    0 Size:    16
//
// }
//
// cbuffer POINT_LIGHT
// {
//
//   float3 ptLightPos;                 // Offset:    0 Size:    12
//   float ptNada;                      // Offset:   12 Size:     4 [unused]
//   float4 ptLightColor;               // Offset:   16 Size:    16
//
// }
//
// cbuffer SPOT_LIGHT
// {
//
//   float3 spotLightPos;               // Offset:    0 Size:    12
//   float coneRatio;                   // Offset:   12 Size:     4
//   float3 coneDir;                    // Offset:   16 Size:    12
//   float spotNada;                    // Offset:   28 Size:     4 [unused]
//   float4 spotLightColor;             // Offset:   32 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// filters                           sampler      NA          NA    0        1
// baseTexture                       texture  float4          2d    0        1
// DIR_LIGHT                         cbuffer      NA          NA    0        1
// AMBIENT                           cbuffer      NA          NA    1        1
// POINT_LIGHT                       cbuffer      NA          NA    2        1
// SPOT_LIGHT                        cbuffer      NA          NA    3        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// UV                       0   xy          1     NONE   float   xy  
// NORMAL                   0   xyz         2     NONE   float   xyz 
// LT_POSITION              0   xyzw        3     NONE   float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
ps_4_0
dcl_constantbuffer cb0[2], immediateIndexed
dcl_constantbuffer cb1[1], immediateIndexed
dcl_constantbuffer cb2[2], immediateIndexed
dcl_constantbuffer cb3[3], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_input_ps linear v1.xy
dcl_input_ps linear v2.xyz
dcl_input_ps linear v3.xyz
dcl_output o0.xyzw
dcl_temps 5
sample r0.xyzw, v1.xyxx, t0.xyzw, s0
lt r1.x, r0.w, l(0.100000)
discard_nz r1.x
dp3 r1.x, v2.xyzx, v2.xyzx
rsq r1.x, r1.x
mul r1.xyz, r1.xxxx, v2.xyzx
dp3 r1.w, cb0[0].xyzx, cb0[0].xyzx
rsq r1.w, r1.w
mul r2.xyz, r1.wwww, cb0[0].xyzx
dp3_sat r1.w, -r2.xyzx, r1.xyzx
add r2.xyz, -v3.xyzx, cb2[0].xyzx
dp3 r2.w, r2.xyzx, r2.xyzx
rsq r3.x, r2.w
mul r2.xyz, r2.xyzx, r3.xxxx
dp3_sat r2.x, r2.xyzx, r1.xyzx
sqrt r2.y, r2.w
mul r2.y, r2.y, l(0.333333)
min r2.y, r2.y, l(1.000000)
add r2.y, -r2.y, l(1.000000)
mul r2.y, r2.y, r2.y
add r3.xyz, -v3.xyzx, cb3[0].xyzx
dp3 r2.z, r3.xyzx, r3.xyzx
rsq r2.z, r2.z
mul r3.xyz, r2.zzzz, r3.xyzx
dp3_sat r1.x, r3.xyzx, r1.xyzx
dp3_sat r1.y, -r3.xyzx, cb3[1].xyzx
lt r1.z, cb3[0].w, r1.y
and r1.z, r1.z, l(0x3f800000)
add_sat r1.y, -r1.y, cb3[0].w
add r1.y, -r1.y, l(1.000000)
mul r1.y, r1.y, r1.y
mul r3.xyzw, r0.xyzw, cb0[1].xyzw
mul r4.xyzw, r0.xyzw, cb2[1].xyzw
mul r4.xyzw, r2.xxxx, r4.xyzw
mul r2.xyzw, r2.yyyy, r4.xyzw
mul r4.xyzw, r0.xyzw, cb3[2].xyzw
mul r4.xyzw, r1.xxxx, r4.xyzw
mul r4.xyzw, r1.zzzz, r4.xyzw
mad r2.xyzw, r3.xyzw, r1.wwww, r2.xyzw
mad r1.xyzw, r4.xyzw, r1.yyyy, r2.xyzw
mad_sat o0.xyzw, r0.xyzw, cb1[0].xyzw, r1.xyzw
ret 
// Approximately 42 instruction slots used
#endif

const BYTE Trivial_PS[] =
{
     68,  88,  66,  67, 249, 155, 
    110, 213,  17, 156, 237, 155, 
    122,  17,  25,  25, 252, 138, 
    201, 185,   1,   0,   0,   0, 
     96,  10,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    196,   3,   0,   0,  88,   4, 
      0,   0, 140,   4,   0,   0, 
    228,   9,   0,   0,  82,  68, 
     69,  70, 136,   3,   0,   0, 
      4,   0,   0,   0,  28,   1, 
      0,   0,   6,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    255, 255,   0,   1,   0,   0, 
     83,   3,   0,   0, 220,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    228,   0,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0, 240,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 250,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      2,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  14,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 102, 105, 
    108, 116, 101, 114, 115,   0, 
     98,  97, 115, 101,  84, 101, 
    120, 116, 117, 114, 101,   0, 
     68,  73,  82,  95,  76,  73, 
     71,  72,  84,   0,  65,  77, 
     66,  73,  69,  78,  84,   0, 
     80,  79,  73,  78,  84,  95, 
     76,  73,  71,  72,  84,   0, 
     83,  80,  79,  84,  95,  76, 
     73,  71,  72,  84,   0, 171, 
    171, 171, 240,   0,   0,   0, 
      3,   0,   0,   0, 124,   1, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 250,   0,   0,   0, 
      1,   0,   0,   0,  20,   2, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   1,   0,   0, 
      3,   0,   0,   0,  60,   2, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  14,   1,   0,   0, 
      5,   0,   0,   0, 164,   2, 
      0,   0,  48,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 196,   1,   0,   0, 
      0,   0,   0,   0,  12,   0, 
      0,   0,   2,   0,   0,   0, 
    208,   1,   0,   0,   0,   0, 
      0,   0, 224,   1,   0,   0, 
     12,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    232,   1,   0,   0,   0,   0, 
      0,   0, 248,   1,   0,   0, 
     16,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
      4,   2,   0,   0,   0,   0, 
      0,   0, 108, 105, 103, 104, 
    116,  68, 105, 114,   0, 171, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    100, 105, 114,  78,  97, 100, 
     97,   0,   0,   0,   3,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    108, 105, 103, 104, 116,  67, 
    111, 108, 111, 114,   0, 171, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  44,   2, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0,   4,   2,   0,   0, 
      0,   0,   0,   0,  97, 109, 
     98, 105, 101, 110, 116,  67, 
    111, 108, 111, 114,   0, 171, 
    171, 171, 132,   2,   0,   0, 
      0,   0,   0,   0,  12,   0, 
      0,   0,   2,   0,   0,   0, 
    208,   1,   0,   0,   0,   0, 
      0,   0, 143,   2,   0,   0, 
     12,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    232,   1,   0,   0,   0,   0, 
      0,   0, 150,   2,   0,   0, 
     16,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
      4,   2,   0,   0,   0,   0, 
      0,   0, 112, 116,  76, 105, 
    103, 104, 116,  80, 111, 115, 
      0, 112, 116,  78,  97, 100, 
     97,   0, 112, 116,  76, 105, 
    103, 104, 116,  67, 111, 108, 
    111, 114,   0, 171,  28,   3, 
      0,   0,   0,   0,   0,   0, 
     12,   0,   0,   0,   2,   0, 
      0,   0, 208,   1,   0,   0, 
      0,   0,   0,   0,  41,   3, 
      0,   0,  12,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 232,   1,   0,   0, 
      0,   0,   0,   0,  51,   3, 
      0,   0,  16,   0,   0,   0, 
     12,   0,   0,   0,   2,   0, 
      0,   0, 208,   1,   0,   0, 
      0,   0,   0,   0,  59,   3, 
      0,   0,  28,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 232,   1,   0,   0, 
      0,   0,   0,   0,  68,   3, 
      0,   0,  32,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0,   4,   2,   0,   0, 
      0,   0,   0,   0, 115, 112, 
    111, 116,  76, 105, 103, 104, 
    116,  80, 111, 115,   0,  99, 
    111, 110, 101,  82,  97, 116, 
    105, 111,   0,  99, 111, 110, 
    101,  68, 105, 114,   0, 115, 
    112, 111, 116,  78,  97, 100, 
     97,   0, 115, 112, 111, 116, 
     76, 105, 103, 104, 116,  67, 
    111, 108, 111, 114,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  54,  46, 
     51,  46,  57,  54,  48,  48, 
     46,  49,  54,  51,  56,  52, 
      0, 171, 171, 171,  73,  83, 
     71,  78, 140,   0,   0,   0, 
      4,   0,   0,   0,   8,   0, 
      0,   0, 104,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0, 116,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   3,   3, 
      0,   0, 119,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   7,   7, 
      0,   0, 126,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,  15,   7, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  85,  86,   0,  78, 
     79,  82,  77,  65,  76,   0, 
     76,  84,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
    171, 171,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  65,  82, 
     71,  69,  84,   0, 171, 171, 
     83,  72,  68,  82,  80,   5, 
      0,   0,  64,   0,   0,   0, 
     84,   1,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  89,   0,   0,   4, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   2,   0,   0,   0, 
      2,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   1,   0,   0,   0, 
     98,  16,   0,   3, 114,  16, 
     16,   0,   2,   0,   0,   0, 
     98,  16,   0,   3, 114,  16, 
     16,   0,   3,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   5,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  49,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 205, 204, 204,  61, 
     13,   0,   4,   3,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     16,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   2,   0, 
      0,   0,  70,  18,  16,   0, 
      2,   0,   0,   0,  68,   0, 
      0,   5,  18,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70,  18,  16,   0, 
      2,   0,   0,   0,  16,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  68,   0, 
      0,   5, 130,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8, 114,   0, 
     16,   0,   2,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  16,  32,   0,   8, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16, 128, 
     65,   0,   0,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   9, 114,   0,  16,   0, 
      2,   0,   0,   0,  70,  18, 
     16, 128,  65,   0,   0,   0, 
      3,   0,   0,   0,  70, 130, 
     32,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  68,   0,   0,   5, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
      6,   0,  16,   0,   3,   0, 
      0,   0,  16,  32,   0,   7, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     75,   0,   0,   5,  34,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0, 171, 170, 170,  62, 
     51,   0,   0,   7,  34,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   8,  34,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     56,   0,   0,   7,  34,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   9, 114,   0,  16,   0, 
      3,   0,   0,   0,  70,  18, 
     16, 128,  65,   0,   0,   0, 
      3,   0,   0,   0,  70, 130, 
     32,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   7,  66,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,  68,   0,   0,   5, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      3,   0,   0,   0, 166,  10, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,  16,  32,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     16,  32,   0,   9,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16, 128,  65,   0, 
      0,   0,   3,   0,   0,   0, 
     70, 130,  32,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     49,   0,   0,   8,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     58, 128,  32,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   1,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
      0,  32,   0,   9,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
     58, 128,  32,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   8,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  56,   0,   0,   7, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   2,   0,   0,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7, 242,   0,  16,   0, 
      4,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  56,   0,   0,   7, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,  56,   0,   0,   7, 
    242,   0,  16,   0,   4,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     56,   0,   0,   7, 242,   0, 
     16,   0,   4,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0,  50,   0, 
      0,   9, 242,   0,  16,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,   9, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,  32, 
      0,  10, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,  42,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     38,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
