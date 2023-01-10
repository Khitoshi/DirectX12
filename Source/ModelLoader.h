#pragma once

#include <vector>
#include <fbxsdk.h>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;
struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 uv;
};

struct VertexInfo
{
    vector<Vertex> vertices;
    vector<unsigned short> indices;
};


//FBX model Çì«Ç›çûÇ›
class FBXModelLoader
{
private:


public:
    FBXModelLoader() {};
    ~FBXModelLoader() {};

    bool Load(const string& filePath, VertexInfo* vertexInfo);

private:
    bool IsExistNormalUVInfo(XMFLOAT3)const;

};

