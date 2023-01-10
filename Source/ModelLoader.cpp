#include "ModelLoader.h"

bool FBXModelLoader::Load(const string& filePath, VertexInfo* vertexInfo)
{
    //�}�l�[�W���[����
    FbxManager* manager = FbxManager::Create();

    //�C���|�[�^����
    FbxImporter* importer = FbxImporter::Create(manager,"");
    if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings()))
    {
        //�������s
        return false;
    }

    //�V�[������
    FbxScene* scene = FbxScene::Create(manager, "");
    importer->Import(scene);
    //�s�v�ɂȂ����̂Ŕj��
    importer->Destroy();

    //�O�p�|���S���ւ̃R���o�[�g
    FbxGeometryConverter geometryConverter(manager);
    if (!geometryConverter.Triangulate(scene, true))
    {
        //�R���o�[�g���s
        return false;
    }

    //���b�V���擾
    FbxMesh* mesh = scene->GetSrcObject<FbxMesh>();
    if (!mesh)
    {
        //�擾���s
        return false;
    }

    //UV�Z�b�g���擾
    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);
    const string uvSetName = uvSetNameList.GetStringAt(0);

    //���_���W���̃��X�g����
    vector<vector<float>> vertexInfoList;
    for (int i = 0; i < mesh->GetControlPointsCount(); i++)
    {
        const FbxVector4 point = mesh->GetControlPointAt(i);

        //���_���W�擾
        //point[0];
        //point[1];
        //point[2];
        // 
        //���_���W �o�^
        vector<float> vertex;
        vertex.push_back(point[0]);
        vertex.push_back(point[1]);
        vertex.push_back(point[2]);

        vertexInfoList.push_back(vertex);

    }

    //���_���̏����擾����
    vector<unsigned short> indices;
    vector<array<int, 2>> oldNewIndexPairList;
    
    for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
    {
        for (int polygonVertexIndex = 0; polygonVertexIndex < mesh->GetPolygonSize(polygonIndex); polygonVertexIndex++)
        {
            //�C���f�b�N�X���W
            auto vertexIndex = mesh->GetPolygonVertex(polygonIndex, polygonVertexIndex);

            //���_���W
            //XMFLOAT3 vertexInfo = vertexInfoList[vertexIndex];

            //�@�����W
            FbxVector4 normalPosition;
            mesh->GetPolygonVertexNormal(polygonIndex, polygonVertexIndex, normalPosition);

            //UV���W
            FbxVector2 uvPosition;
            bool isUnmapped;
            mesh->GetPolygonVertexUV(polygonIndex, polygonVertexIndex, uvSetName.c_str(), uvPosition, isUnmapped);

            //�C���f�b�N�X���W��ݒ�
            indices.push_back(vertexIndex);
        }
    }

    //���_��񐶐�
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

    //�j��
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
    //return vertexInfo.size() == 8; // ���_3 + �@��3 + UV2
}

