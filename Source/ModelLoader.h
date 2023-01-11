#pragma once

#include <vector>
#include <fbxsdk.h>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;

struct Scene
{
    struct Node
    {
        uint64_t unique_id{ 0 };
        string name{"not found"};
        FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
        int64_t parent_index{ -1 };
    };
    vector<Node>nodes;

    int64_t indexof(uint64_t unique_id)const
    {
        int64_t index(0);
        for (const Node& node: nodes)
        {
            if (node.unique_id == unique_id)
            {
                return index;
            }
            index++;
        }
        return -1;
    }

};

//FBX model Çì«Ç›çûÇ›
class FBXModelLoader
{
private:
public:
    FBXModelLoader(const char* fbx_filename, bool triangulate = false);
    ~FBXModelLoader() {};

private:
    

public:

private:
    Scene scene_view_;
};

