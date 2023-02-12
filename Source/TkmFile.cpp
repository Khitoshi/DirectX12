#include "TkmFile.h"
#include <Windows.h>

//�@���X���[�W���O�B
class NormalSmoothing {
private:
	struct SSmoothVertex {
		Vector3 newNormal = Vector3::GetVec3Zero();
		TkmFile::SVertex* vertex = nullptr;
	};
	struct SFace {
		Vector3 normal;
		std::vector<int> vertexNos;
	};
public:
	template <class IndexBuffer>
	void Execute(TkmFile::SMesh& mesh, const IndexBuffer& indexBuffer)
	{

		//�ʖ@�����v�Z���Ă����B
		auto numPolygon = indexBuffer.indices.size() / 3;
		std::vector< SFace> faces;
		faces.reserve(numPolygon);

		for (auto polyNo = 0; polyNo < numPolygon; polyNo++) {
			auto no = polyNo * 3;
			auto vertNo_0 = indexBuffer.indices[no];
			auto vertNo_1 = indexBuffer.indices[no + 1];
			auto vertNo_2 = indexBuffer.indices[no + 2];

			auto& vert_0 = mesh.vertexBuffer[vertNo_0];
			auto& vert_1 = mesh.vertexBuffer[vertNo_1];
			auto& vert_2 = mesh.vertexBuffer[vertNo_2];

			//�@�����v�Z����B
			Vector3 v0tov1 = vert_1.pos - vert_0.pos;
			Vector3 v0tov2 = vert_2.pos - vert_0.pos;
			Vector3 normal = Cross(v0tov1, v0tov2);
			normal.Normalize();
			SFace face;
			face.normal = normal;
			face.vertexNos.push_back(vertNo_0);
			face.vertexNos.push_back(vertNo_1);
			face.vertexNos.push_back(vertNo_2);
			faces.push_back(face);
		}

		//�X�e�b�v�Q�@�@���̕��ω�
		for (auto& face : faces) {
			for (auto vertNo : face.vertexNos) {
				auto& vert = mesh.vertexBuffer[vertNo];
				vert.normal += face.normal;
			}
		}
		for (auto& vert : mesh.vertexBuffer) {
			vert.normal.Normalize();
		}
		//�X�e�b�v�Q�@���W�ƌ������������_�̖@���𕽋ω����Ă����B
		if (mesh.isFlatShading == 0)
		{
			//�d�����Ă��钸�_�̖@���𕽋ω�
			std::vector<SSmoothVertex> smoothVertex;
			smoothVertex.reserve(mesh.vertexBuffer.size());
			for (auto& v : mesh.vertexBuffer) {
				smoothVertex.push_back({ v.normal, &v });
			}
			for (auto& va : smoothVertex) {
				for (auto& vb : smoothVertex) {

					if (va.vertex != vb.vertex
						&& va.vertex->pos.x == vb.vertex->pos.x
						&& va.vertex->pos.y == vb.vertex->pos.y
						&& va.vertex->pos.z == vb.vertex->pos.z
						) {
						//�������W�B
						if (va.vertex->normal.Dot(vb.vertex->normal) > 0.0f) {
							//���������B
							va.newNormal += vb.vertex->normal;
						}
					}
				}
				va.newNormal.Normalize();
			}
			for (auto& va : smoothVertex) {
				va.vertex->normal = va.newNormal;
			}
		}
	}
};


/// <summary>
/// TKM�t�@�C���t�H�[�}�b�g�B
/// </summary>
/// <remarks>
/// �����n�ɂ���Ă�1�o�C�g��8bit�łȂ����Ƃ�����A
/// int�^��short�^���K�������A4�o�C�g�A2�o�C�g�ł���Ƃ͌���Ȃ��B
/// ���̂��߁Astd::uint16_t��std::uint32_t�𗘗p���Ă���B
/// �����͒�`����Ă��鏈���n�ł���΁A�T�C�Y�͕K�������ł���B
/// </remarks>
namespace tkmFileFormat {
	//���݂�TKM�t�@�C���̃o�[�W�����B
	std::uint16_t VERSION = 100;

	/// <summary>
	/// �w�b�_�[�t�@�C��
	/// </summary>
	struct SHeader {
		std::uint8_t	version;		//�o�[�W�����B
		std::uint8_t	isFlatShading;	//�t���b�g�V�F�[�f�B���O�H
		std::uint16_t	numMeshParts;	//���b�V���p�[�c�̐��B
	};

	/// <summary>
	/// ���b�V���p�[�c�w�b�_�[�B
	/// </summary>
	struct SMeshePartsHeader {
		std::uint32_t numMaterial;		//�}�e���A���̐��B
		std::uint32_t numVertex;		//���_���B
		std::uint8_t indexSize;			//�C���f�b�N�X�̃T�C�Y�B2��4�B
		std::uint8_t pad[3];			//�p�f�B���O�B
	};

	/// <summary>
	/// ���_
	/// </summary>
	struct SVertex {
		//���_���W
		float pos[3];					
		//�@��
		float normal[3];				
		//UV���W
		float uv[2];					
		//�X�L���E�F�C�g
		float weights[4];				
		//�X�L���C���f�b�N�X
		std::int16_t indices[4];		
	};
};

template< class IndexBuffer>
void BuildTangentAndBiNormalImp(TkmFile::SMesh& mesh, const IndexBuffer& indexBuffer)
{
	//���_�X���[�X�͋C�ɂ��Ȃ��B
	auto numPolygon = indexBuffer.indices.size() / 3;
	for (auto polyNo = 0; polyNo < numPolygon; polyNo++) {
		auto no = polyNo * 3;
		auto vertNo_0 = indexBuffer.indices[no];
		auto vertNo_1 = indexBuffer.indices[no + 1];
		auto vertNo_2 = indexBuffer.indices[no + 2];

		auto& vert_0 = mesh.vertexBuffer[vertNo_0];
		auto& vert_1 = mesh.vertexBuffer[vertNo_1];
		auto& vert_2 = mesh.vertexBuffer[vertNo_2];

		Vector3 cp0[3];
		cp0[0] = { vert_0.pos.x, vert_0.uv.x, vert_0.uv.y };
		cp0[1] = { vert_0.pos.y, vert_0.uv.x, vert_0.uv.y };
		cp0[2] = { vert_0.pos.z, vert_0.uv.x, vert_0.uv.y };
		/*{
			{ vert_0.pos.x, vert_0.uv.x, vert_0.uv.y},
			{ vert_0.pos.y, vert_0.uv.x, vert_0.uv.y},
			{ vert_0.pos.z, vert_0.uv.x, vert_0.uv.y}
		};
		*/

		Vector3 cp1[3];
		cp1[0] = { vert_1.pos.x, vert_1.uv.x, vert_1.uv.y };
		cp1[1] = { vert_1.pos.y, vert_1.uv.x, vert_1.uv.y };
		cp1[2] = { vert_1.pos.z, vert_1.uv.x, vert_1.uv.y };
		/*
		{
			{ vert_1.pos.x, vert_1.uv.x, vert_1.uv.y},
			{ vert_1.pos.y, vert_1.uv.x, vert_1.uv.y},
			{ vert_1.pos.z, vert_1.uv.x, vert_1.uv.y}
		};
		*/

		Vector3 cp2[3];
		cp2[0] = { vert_2.pos.x, vert_2.uv.x, vert_2.uv.y };
		cp2[1] = { vert_2.pos.y, vert_2.uv.x, vert_2.uv.y };
		cp2[2] = { vert_2.pos.z, vert_2.uv.x, vert_2.uv.y };
		/*
		{
			{ vert_2.pos.x, vert_2.uv.x, vert_2.uv.y},
			{ vert_2.pos.y, vert_2.uv.x, vert_2.uv.y},
			{ vert_2.pos.z, vert_2.uv.x, vert_2.uv.y}
		};
		*/

		// ���ʃp�����[�^����UV�����W�Z�o����B
		Vector3 tangent, binormal;
		for (int i = 0; i < 3; ++i) {
			auto V1 = cp1[i] - cp0[i];
			auto V2 = cp2[i] - cp1[i];
			auto ABC = Cross(V1, V2);

			if (ABC.x == 0.0f) {
				tangent.v[i] = 0.0f;
				binormal.v[i] = 0.0f;
			}
			else {
				tangent.v[i] = -ABC.y / ABC.x;
				binormal.v[i] = -ABC.z / ABC.x;
			}
		}

		tangent.Normalize();
		binormal.Normalize();

		vert_0.tangent += tangent;
		vert_1.tangent += tangent;
		vert_2.tangent += tangent;

		vert_0.binormal += binormal;
		vert_1.binormal += binormal;
		vert_2.binormal += binormal;
	}
	//�@���A�ڃx�N�g���A�]�x�N�g���𕽋ω�����B
	for (auto& vert : mesh.vertexBuffer) {
		vert.tangent.Normalize();
		vert.binormal.Normalize();
	}

}


//.tkm file ���[�h
void TkmFile::Load(const char* filePath)
{
    //�t�@�C�����J��
    FILE* fp;
    fopen_s(&fp, filePath, "rb");

    //�t�@�C���`�F�b�N
    if (fp == nullptr)
    {
        MessageBoxA(nullptr, ".tkm�t�@�C�����J���܂���B", "�G���[", MB_OK);
		std::abort();
    }

    //tkm�t�@�C���̃w�b�_�[��ǂݍ��݁B
    tkmFileFormat::SHeader header;
    fread(&header, sizeof(header), 1, fp);
    if (header.version != tkmFileFormat::VERSION) {
        //tkm�t�@�C���̃o�[�W�������Ⴄ�B
        MessageBoxA(nullptr, ".tkm�t�@�C���̃o�[�W�������قȂ��Ă��܂��B", "�G���[", MB_OK);
		std::abort();
    }

	//���b�V���������[�h���Ă���
	this->mesh_Parts_.resize(header.numMeshParts);
	for (int meshPartsNo = 0; meshPartsNo < header.numMeshParts; meshPartsNo++) {

		auto& mesh_parts = this->mesh_Parts_[meshPartsNo];
		mesh_parts.isFlatShading = header.isFlatShading;
		tkmFileFormat::SMeshePartsHeader meshPartsHeader;
		fread(&meshPartsHeader, sizeof(meshPartsHeader), 1, fp);
		//�}�e���A�������L�^�ł���̈���m�ہB
		mesh_parts.materials.resize(meshPartsHeader.numMaterial);
		//�}�e���A�������\�z���Ă����B
		for (unsigned int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
			auto& material = mesh_parts.materials[materialNo];
			BuildMaterial(material, fp, filePath);
		}

		//�����Ē��_�o�b�t�@
		mesh_parts.vertexBuffer.resize(meshPartsHeader.numVertex);
		for (unsigned int vertNo = 0; vertNo < meshPartsHeader.numVertex; vertNo++) {
			tkmFileFormat::SVertex vertexTmp;
			int err = fread(&vertexTmp, sizeof(vertexTmp), 1, fp);
			auto& vertex = mesh_parts.vertexBuffer[vertNo];
			vertex.pos.Set(vertexTmp.pos[0], vertexTmp.pos[1], vertexTmp.pos[2]);
			//	vertex.normal.Set(vertexTmp.normal[0], vertexTmp.normal[1], vertexTmp.normal[2]);
			vertex.normal = Vector3::GetVec3Zero();
			vertex.tangent = Vector3::GetVec3Zero();
			vertex.binormal = Vector3::GetVec3Zero();
			vertex.uv.Set(vertexTmp.uv[0], vertexTmp.uv[1]);
			vertex.skinWeights.Set(vertexTmp.weights[0], vertexTmp.weights[1], vertexTmp.weights[2], vertexTmp.weights[3]);
			vertex.indices[0] = vertexTmp.indices[0] != -1 ? vertexTmp.indices[0] : 0;
			vertex.indices[1] = vertexTmp.indices[1] != -1 ? vertexTmp.indices[1] : 0;
			vertex.indices[2] = vertexTmp.indices[2] != -1 ? vertexTmp.indices[2] : 0;
			vertex.indices[3] = vertexTmp.indices[3] != -1 ? vertexTmp.indices[3] : 0;
		}

		//�����ăC���f�b�N�X�o�b�t�@�B
		//�C���f�b�N�X�o�b�t�@�̓}�e���A���̐����������݂���
		if (meshPartsHeader.indexSize == 2) {
			//16bit�̃C���f�b�N�X�o�b�t�@�B
			mesh_parts.indexBuffer16Array.resize(meshPartsHeader.numMaterial);
		}
		else {
			//32bit�̃C���f�b�N�X�o�b�t�@�B
			mesh_parts.indexBuffer32Array.resize(meshPartsHeader.numMaterial);
		}

		for (unsigned int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
			//�|���S���������[�h�B
			int numPolygon;
			fread(&numPolygon, sizeof(numPolygon), 1, fp);
			//�g�|���W�[�̓g���C�A���O�����X�g�I�����[�Ȃ̂ŁA3����Z����ƃC���f�b�N�X�̐��ɂȂ�B
			int numIndex = numPolygon * 3;
			if (meshPartsHeader.indexSize == 2) {
				LoadIndexBuffer(
					mesh_parts.indexBuffer16Array[materialNo].indices,
					numIndex,
					fp
				);
			}
			else {
				LoadIndexBuffer(
					mesh_parts.indexBuffer32Array[materialNo].indices,
					numIndex,
					fp
				);
			}
		}
	}
	//�ڃx�N�g���Ə]�x�N�g�����\�z����B
	BuildTangentAndBiNormal();

	//�t�@�C�������
	fclose(fp);
}

//�e�N�X�`���������[�h�B
std::string TkmFile::LoadTextureFileName(FILE* fp)
{
	//�e�N�X�`����
	std::string texture_name;
	//�t�@�C���� �̒���
	std::uint32_t file_name_length;
	//�t�@�C�����̒����̎擾
	fread(&file_name_length, sizeof(file_name_length), 1, fp);

	//�t�@�C���̊m�F
	//0 �̏ꍇ �t�@�C��������
	if (file_name_length <= 0)
	{
		return texture_name;
		/*//�e�N�X�`���̃��[�h�Ɏ��s*/
		//MessageBoxA(nullptr, "�e�N�X�`���̃��[�h�Ɏ��s", "�G���[", MB_OK);
		//std::abort();
	}

	//�t�@�C���l�[������
	char* local_file_name = reinterpret_cast<char*>(malloc(file_name_length + 1));
	//null�����ǂݍ��ނ̂� + 1
	fread(local_file_name, file_name_length + 1, 1, fp);
	texture_name = local_file_name;
	//���
	free(local_file_name);
	
	return texture_name;
}

//�C���f�b�N�X�o�b�t�@�����[�h
template<class T>
void TkmFile::LoadIndexBuffer(std::vector<T>& indices, int numIndex, FILE* fp)
{
	indices.resize(numIndex);
	for (int indexNo = 0; indexNo < numIndex; indexNo++)
	{
		T index;
		fread(&index, sizeof(index), 1, fp);
		indices[indexNo] = index - 1;
	}
}

//�}�e���A�����\�z
void TkmFile::BuildMaterial(SMaterial& tkmMaterial, FILE* fp, const char* filePath)
{
	//�e�t�@�C���������[�h
	//�A���x�h�}�b�v
	tkmMaterial.albedoMapFileName = LoadTextureFileName(fp);
	//�@���}�b�v
	tkmMaterial.normalMapFileName = LoadTextureFileName(fp);
	//�X�y�L�����}�b�v
	tkmMaterial.specularMapFileName = LoadTextureFileName(fp);
	//���t���N�V�����}�b�v�̃t�@�C���������[�h�B
	tkmMaterial.reflectionMapFileName = LoadTextureFileName(fp);
	//���܃}�b�v
	tkmMaterial.refractionMapFileName = LoadTextureFileName(fp);

	//�e�N�X�`�����[�h
	std::string tex_file_path = filePath;
	auto loadTexture = [&](
		std::string& texFileName,
		std::unique_ptr<char[]>& ddsFileMemory,
		unsigned int& fileSize,
		std::string& texFilePathDst
		)
	{
		//�t�@�C�����̒���
		int file_path_length = static_cast<int>(tex_file_path.length());

		//�t�@�C���̑��݊m�F
		if (texFileName.length() <= 0) { return; }

		//���f���̃t�@�C���p�X���烉�X�g�̃t�H���_��؂��T��
		auto replaseStartPos = tex_file_path.find_last_of('/');
		if (replaseStartPos == std::string::npos) {
			replaseStartPos = tex_file_path.find_last_of('\\');
		}
		replaseStartPos += 1;
		auto replaceLen = file_path_length - replaseStartPos;
		tex_file_path.replace(replaseStartPos, replaceLen, texFileName);
		//�g���q��dds�ɕύX����B
		replaseStartPos = tex_file_path.find_last_of('.') + 1;
		replaceLen = tex_file_path.length() - replaseStartPos;
		tex_file_path.replace(replaseStartPos, replaceLen, "dds");
		//�e�N�X�`���t�@�C���p�X���L�����Ă����B
		texFilePathDst = tex_file_path;

		//�e�N�X�`�������[�h
		FILE* tex_file_fp;
		fopen_s(&tex_file_fp, tex_file_path.c_str(), "rb");
		if (tex_file_fp == nullptr)
		{
			//�e�N�X�`���̃��[�h�Ɏ��s
			MessageBoxA(nullptr, "�e�N�X�`�����[�h�Ɏ��s", "�G���[", MB_OK);
			std::abort();
		}

		//�t�@�C���T�C�Y���擾�B
		fseek(tex_file_fp, 0L, SEEK_END);
		fileSize = ftell(tex_file_fp);
		fseek(tex_file_fp, 0L, SEEK_SET);

		ddsFileMemory = std::make_unique<char[]>(fileSize);
		fread(ddsFileMemory.get(), fileSize, 1, tex_file_fp);
		fclose(tex_file_fp);
	};

	//�e�e�N�X�`�������[�h
	//�A���x�h�}�b�v�@���[�h
	loadTexture(
		tkmMaterial.albedoMapFileName,
		tkmMaterial.albedoMap,
		tkmMaterial.albedoMapSize,
		tkmMaterial.albedoMapFilePath
	);

	//�@���}�b�v�@���[�h
	loadTexture(
		tkmMaterial.normalMapFileName,
		tkmMaterial.normalMap,
		tkmMaterial.normalMapSize,
		tkmMaterial.normalMapFilePath
	);

	//�X�y�L�����}�b�v ���[�h
	loadTexture(
		tkmMaterial.specularMapFileName,
		tkmMaterial.specularMap,
		tkmMaterial.specularMapSize,
		tkmMaterial.specularMapFilePath
	);

	//���t���N�V�����}�b�v�@���[�h
	loadTexture(
		tkmMaterial.reflectionMapFileName,
		tkmMaterial.reflectionMap,
		tkmMaterial.reflectionMapSize,
		tkmMaterial.reflectionMapFilePath
	);

	//���܃}�b�v�@���[�h
	loadTexture(
		tkmMaterial.refractionMapFileName,
		tkmMaterial.refractionMap,
		tkmMaterial.refractionMapSize,
		tkmMaterial.refractionMapFilePath
	);
}

//�ڃx�N�g���Ə]�x�N�g�����v�Z
void TkmFile::BuildTangentAndBiNormal()
{
	NormalSmoothing normalSmoothing;
	for (auto& mesh : this->mesh_Parts_) {
		for (auto& indexBuffer : mesh.indexBuffer16Array) {
			normalSmoothing.Execute(mesh, indexBuffer);
			BuildTangentAndBiNormalImp(mesh, indexBuffer);
		}
		for (auto& indexBuffer : mesh.indexBuffer32Array) {
			normalSmoothing.Execute(mesh, indexBuffer);
			BuildTangentAndBiNormalImp(mesh, indexBuffer);
		}
	}
}
