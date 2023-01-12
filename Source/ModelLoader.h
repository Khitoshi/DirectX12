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
    public:
        uint64_t unique_id{ 0 };
        string name{"Node Name not found"};
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

struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
};

struct Mesh
{
public:
    uint64_t unique_id{ 0 };
    string name{ "Mesh Name not found" };
    int64_t node_index{ 0 };

    vector<Vertex> vertexces;
    vector<uint32_t> indices;
private:

};


//FBX model Çì«Ç›çûÇ›
class FBXModelLoader
{
public:
    FBXModelLoader(const char* fbx_filename, bool triangulate = false);
    ~FBXModelLoader() {};

private:
    //Mesh éÊìæ
    void FetchMeshes(FbxScene* scene, vector<Mesh>& meshes);
    
public:
    //Get SceneèÓïÒ
    //Scene GetSceneView()const { return scene_view_; }
    Scene GetSceneView() { return scene_view_; }
    
    //Get Mesh èÓïÒ
    //vector<Mesh> GetMeshes()const { return meshes_; }
    vector<Mesh> GetMeshes()const { return meshes_; }
   
    int GetSize() {
        int index = 0;
        for (const Mesh& mesh : meshes_)
        {
            index += mesh.indices.size();
        }
        return index;
    }

private:
    Scene scene_view_;
    vector<Mesh> meshes_;
};

