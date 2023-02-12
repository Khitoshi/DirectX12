#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

#include <fbxsdk.h>

struct Scene
{
	struct node
	{
		uint64_t unique_id{ 0 };
		std::string name;
		FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
		int64_t parent_index{ -1 };
	};
	std::vector<node>nodes;

	int64_t indexof(uint64_t unique_id)const
	{
		int64_t index{ 0 };
		for (const node& node : nodes)
		{
			if (node.unique_id == unique_id)return index;

			index++;
		}
		return -1;
	}
};

class ModelFile
{
private:

	struct Vertex
	{
		Vector3	position;
		Vector3	normal;
		Vector2	texcoord;
	};

	struct constants
	{
		Matrix world;
		Vector4 material_color;
	};


public:
	ModelFile();

	~ModelFile();

	/// <summary>
	/// .fbx file ÇÉçÅ[Éh
	/// </summary>
	/// <param name="filePath">.fbx file path</param>
	void Load(const char* filePath);

private:

private:
	//VertexInfo vertex_Info_;
	Scene scene_View_;
};

