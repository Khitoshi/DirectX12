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
    

        //マネージャー　破棄
        manager->Destroy();

    }
    catch (const std::string err_str)
    {
        cout << err_str << endl;
        exit(-1);
    }


}
