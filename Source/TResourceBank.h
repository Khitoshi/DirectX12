#pragma once

#include <map>
#include <string>
#include <memory>

/// <summary>
/// ���\�[�X�𒙂߂�N���X
/// </summary>
/// <typeparam name="TResource"></typeparam>
template<class TResource>
class TResourceBank
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	TResourceBank() :
		resource_Map()
	{

	}

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~TResourceBank()
	{

	}

	TResource* Get(const char* filePath)
	{
		auto it = this->resource_Map.find(filePath);
		if (it != this->resource_Map.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	void Regist(const char* filePath, TResource* resource)
	{
		auto it = this->resource_Map.find(filePath);
		if (it == this->resource_Map.end()) {
			this->resource_Map.insert(
				std::pair< std::string, std::unique_ptr<TResource>>(filePath, resource)
			);
		}
	}

private:
	//���\�[�X
	std::map<std::string, std::unique_ptr<TResource>> resource_Map;
};

