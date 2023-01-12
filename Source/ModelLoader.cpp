#include "ModelLoader.h"
#include <iostream>
#include <functional>
//#include <debugapi.h>
//#include <sstream>

FBXModelLoader::FBXModelLoader(const char* fbx_filename, bool triangulate)
{
    //マネージャー　作成
    FbxManager* manager(FbxManager::Create());

    //シーン　作成
    FbxScene* scene(FbxScene::Create(manager, ""));

    //インポータ　作成
    FbxImporter* importer(FbxImporter::Create(manager, ""));
    bool importer_status(false);
    try
    {
        //importer　初期化
        importer_status = importer->Initialize(fbx_filename);
        if (!importer_status) throw importer->GetStatus().GetErrorString();

        //シーンをインポート
        importer_status = importer->Import(scene);
        if (!importer_status)throw importer->GetStatus().GetErrorString();

        //三角ポリゴン化
        FbxGeometryConverter converter(manager);
        if (triangulate)
        {
            converter.Triangulate(scene, true, false);
            converter.RemoveBadPolygonsFromMeshes(scene);
        }

        //情報 登録
        function<void(FbxNode*)> traverse{ [&](FbxNode* fbx_node) {

            Scene::Node& node(scene_view_.nodes.emplace_back());
            //識別番号 登録
            node.unique_id = fbx_node->GetUniqueID();
            //名前 登録
            node.name = fbx_node->GetName();
            //属性 登録
            node.attribute = fbx_node->GetNodeAttribute() ?
                fbx_node->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
            //親の番号 登録
            node.parent_index = scene_view_.indexof(fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0);
            //子供の情報も登録する
            for (int i = 0; i < fbx_node->GetChildCount(); i++)
            {
                traverse(fbx_node->GetChild(i));
            }
            }
        };
        //シーンに登録されている情報を登録
        traverse(scene->GetRootNode());
    
#if 1
        for (const Scene::Node& node : scene_view_.nodes)
        {
            FbxNode* fbx_node{ scene->FindNodeByName(node.name.c_str()) };
            // Display node data in the output window as debug
            string node_name = fbx_node->GetName();
            uint64_t uid = fbx_node->GetUniqueID();
            uint64_t parent_uid = fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0;
            int32_t type = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() :
                0;
            //stringstream debug_string;
            //debug_string << node_name << ":" << uid << ":" << parent_uid << ":" << type << "¥n";
            //OutputDebugStringA(debug_string.str().c_str());
            int potate = 555;
        };
#endif
    
        FetchMeshes(scene, meshes_);

        //マネージャー　破棄
        manager->Destroy();

    }
    catch (const std::string err_str)
    {
        cout << err_str << endl;
        exit(-1);
    }


}

void FBXModelLoader::FetchMeshes(FbxScene* scene, vector<Mesh>& meshes)
{
    
    for (const Scene::Node& node : scene_view_.nodes)
    {
        
        if (node.attribute != FbxNodeAttribute::EType::eMesh)continue;

        //FbxNode* fbx_node{ scene->FindNodeByName(node.name.c_str()) };
        //FbxMesh* fbx_mesh{ fbx_node->GetMesh() };

        FbxNode* fbx_node(scene->FindNodeByName(node.name.c_str()));
        FbxMesh* fbx_mesh(fbx_node->GetMesh());

        //各種情報の登録
        Mesh& mesh(meshes.emplace_back());
        mesh.unique_id = fbx_mesh->GetNode()->GetUniqueID();
        mesh.name = fbx_mesh->GetNode()->GetName();
        mesh.node_index = scene_view_.indexof(mesh.unique_id);

        //polygon数 取得
        const int polygon_count(fbx_mesh->GetPolygonCount());

        //polygonに合わせたサイズに変更　*3LL とは　long long の 3　という意味*
        mesh.vertexces.resize(polygon_count * 3LL);
        mesh.indices.resize(polygon_count * 3LL);

        //uv名　取得
        FbxStringList uv_names;
        fbx_mesh->GetUVSetNames(uv_names);

        //頂点バッファ　取得
        const FbxVector4* control_points(fbx_mesh->GetControlPoints());

        for (int polygon_index = 0; polygon_index < polygon_count; polygon_index++)
        {
            for (int position_in_polygon = 0; position_in_polygon < 3; position_in_polygon++)
            {
                //頂点番号 
                const int vertex_index(polygon_index * 3 + position_in_polygon);

                //polygonの頂点情報取得
                const int polygon_vertex(fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon));
                
                //頂点情報
                Vertex vertex;
                
                //位置 登録
                vertex.position.x = static_cast<float>(control_points[polygon_vertex][0]);
                vertex.position.y = static_cast<float>(control_points[polygon_vertex][1]);
                vertex.position.z = static_cast<float>(control_points[polygon_vertex][2]);
                
                //ジオメトリ要素のある法線　番号 がある場合
                if (fbx_mesh->GetElementNormalCount() > 0)
                {
                    //法線 取得
                    FbxVector4 normal;
                    fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon,normal);
                    
                    //法線 登録
                    vertex.normal.x = static_cast<float>(normal[0]);
                    vertex.normal.y = static_cast<float>(normal[1]);
                    vertex.normal.z = static_cast<float>(normal[2]);
                }

                //uv
                if (fbx_mesh->GetElementUVCount() > 0)
                {
                    FbxVector2 uv;
                    bool unmapped_uv;
                    fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon, uv_names[0], uv, unmapped_uv);
                    vertex.texcoord.x = static_cast<float>(uv[0]);
                    vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
                }

                //上記で取得した頂点情報をmeshに登録
                mesh.vertexces.at(vertex_index) = move(vertex);
                mesh.indices.at(vertex_index) = vertex_index;
            }
            
        }
    }
    
}
