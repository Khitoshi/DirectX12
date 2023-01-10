#include "ModelLoader.h"

bool FBXModelLoader::Load(const string& filePath, VertexInfo* vertexInfo)
{
    //マネージャー生成
    FbxManager* manager = FbxManager::Create();

    //インポータ生成
    FbxImporter* importer = FbxImporter::Create(manager,"");
    if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings()))
    {
        //生成失敗
        return false;
    }

    //シーン生成
    FbxScene* scene = FbxScene::Create(manager, "");
    importer->Import(scene);
    //不要になったので破棄
    importer->Destroy();

    //三角ポリゴンへのコンバート
    FbxGeometryConverter geometryConverter(manager);
    if (!geometryConverter.Triangulate(scene, true))
    {
        //コンバート失敗
        return false;
    }

    //メッシュ取得
    FbxMesh* mesh = scene->GetSrcObject<FbxMesh>();
    if (!mesh)
    {
        //取得失敗
        return false;
    }

    //UVセット名取得
    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);
    const string uvSetName = uvSetNameList.GetStringAt(0);

    //頂点座標情報のリスト生成
    vector<vector<float>> vertexInfoList;
    for (int i = 0; i < mesh->GetControlPointsCount(); i++)
    {
        const FbxVector4 point = mesh->GetControlPointAt(i);

        //頂点座標取得
        //point[0];
        //point[1];
        //point[2];
        // 
        //頂点座標 登録
        vector<float> vertex;
        vertex.push_back(point[0]);
        vertex.push_back(point[1]);
        vertex.push_back(point[2]);

        vertexInfoList.push_back(vertex);

    }

    //頂点毎の情報を取得する
    vector<unsigned short> indices;
    vector<array<int, 2>> oldNewIndexPairList;
    
    for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
    {
        for (int polygonVertexIndex = 0; polygonVertexIndex < mesh->GetPolygonSize(polygonIndex); polygonVertexIndex++)
        {
            //インデックス座標
            auto vertexIndex = mesh->GetPolygonVertex(polygonIndex, polygonVertexIndex);

            //頂点座標
            //XMFLOAT3 vertexInfo = vertexInfoList[vertexIndex];

            //法線座標
            FbxVector4 normalPosition;
            mesh->GetPolygonVertexNormal(polygonIndex, polygonVertexIndex, normalPosition);

            //UV座標
            FbxVector2 uvPosition;
            bool isUnmapped;
            mesh->GetPolygonVertexUV(polygonIndex, polygonVertexIndex, uvSetName.c_str(), uvPosition, isUnmapped);

            //インデックス座標を設定
            indices.push_back(vertexIndex);
        }
    }

    //頂点情報生成
    vector<Vertex> vertices;
    for (int i = 0; i < vertexInfoList.size(); i++)
    {
        vector<float> vertexInfo = vertexInfoList[i];
        Vertex vertex = Vertex{
            {vertexInfo[0],vertexInfo[1],vertexInfo[2]},//position
            {vertexInfo[3],vertexInfo[4],vertexInfo[5]},//normal
            {vertexInfo[6],vertexInfo[7]}//uv
        };
        vertices.push_back(vertex);
    }

    //破棄
    scene->Destroy();
    manager->Destroy();
    *vertexInfo =
    {
        vertices,
        indices
    };
    return true;
}

bool FBXModelLoader::IsExistNormalUVInfo(XMFLOAT3 vertexIndex) const
{
    return true;
    //return vertexInfo.size() == 8; // 頂点3 + 法線3 + UV2
}

