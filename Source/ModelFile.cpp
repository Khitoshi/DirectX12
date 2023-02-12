#include "ModelFile.h"
#include <functional>
#include <Windows.h>
#include <sstream>
ModelFile::ModelFile()
{

}

ModelFile::~ModelFile()
{

}

//load fbx file
void ModelFile::Load(const char* filePath)
{
    //�}�l�[�W���[ �쐬
    FbxManager* manager = FbxManager::Create();


    //�C���|�[�^ �쐬
    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filePath, -1, manager->GetIOSettings()))
    {
        //���b�Z�[�W�{�b�N�X��\�����āC�ُ�I������
        MessageBox(nullptr, TEXT("FbxImporter�̍쐬�Ɏ��s���܂����D"), TEXT("�G���["), MB_OK);
        std::abort();
    }


    //�V�[������
    FbxScene* scene = FbxScene::Create(manager, "");
    importer->Import(scene);
    //�s�v�ɂȂ����̂Ŕj��
    importer->Destroy();


    //�O�p�|���S���ւ̃R���o�[�g
    /*
    FbxGeometryConverter geometryConverter(manager);
    if (!geometryConverter.Triangulate(scene, true))
    {
        //���b�Z�[�W�{�b�N�X��\�����āC�ُ�I������
        MessageBox(nullptr, TEXT("�O�ppolygon�ւ̃R���o�[�g�Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
        std::abort();
    }
    geometryConverter.RemoveBadPolygonsFromMeshes(scene);
    */
    
    std::function<void(FbxNode*)>traverse{ [&](FbxNode* fbx_node) {
        Scene::node& node{scene_View_.nodes.emplace_back()};
        node.attribute = fbx_node->GetNodeAttribute() ?
            fbx_node->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
        node.name = fbx_node->GetName();
        node.unique_id = fbx_node->GetUniqueID();
        node.parent_index = scene_View_.indexof(fbx_node->GetParent() ?
            fbx_node->GetParent()->GetUniqueID() : 0);

        for (int childIndex = 0; childIndex < fbx_node->GetChildCount(); childIndex++)
        {
            traverse(fbx_node->GetChild(childIndex));
        }
    } };
    traverse(scene->GetRootNode());

#if _DEBUG
    
    for (const Scene::node& node : scene_View_.nodes)
    {
        FbxNode* fbx_node{ scene->FindNodeByName(node.name.c_str()) };

        //�m�[�h�f�[�^���f�B�X�v���C�ɕ\��
        std::string node_name = fbx_node->GetName();
        uint64_t uid = fbx_node->GetUniqueID();
        uint64_t parent_uid = fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0;
        int32_t type = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() : -1;

        std::stringstream debug_string;
        debug_string << node_name << ":" <<
            uid << ":" <<
            parent_uid << ":" <<
            type << "\n";

        //�\��
        OutputDebugStringA(debug_string.str().c_str());
    }
#endif // _DEBUG

    //�j��
    manager->Destroy();
}
