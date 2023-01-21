#pragma once

#include <map>
#include <string>
#include <memory>

/// <summary>
/// リソースを貯めるクラス
/// </summary>
/// <typeparam name="TResource"></typeparam>
template<class TResource>
class TResourceBank
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	TResourceBank() :
		resource_Map()
	{

	}

	/// <summary>
	/// デフォルト デストラクタ
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
	//リソース
	std::map<std::string, std::unique_ptr<TResource>> resource_Map;
};

