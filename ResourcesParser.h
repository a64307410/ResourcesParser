#ifndef RESOURCES_PARSER_H
#define RESOURCES_PARSER_H

#include "ResourceTypes.h"

#include <string>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <memory>

class ResourcesParser {
public:
	typedef unsigned char byte;

	struct EntryPool {
		std::shared_ptr<uint32_t> pOffsets;
		std::shared_ptr<byte> pData;
		uint32_t dataCount;
		uint32_t offsetCount;
	};

	struct ResStringPool {
		ResStringPool_header header;
		std::shared_ptr<uint32_t> pOffsets;
		std::shared_ptr<byte> pStrings;
	};
	typedef std::shared_ptr<ResStringPool> ResStringPoolPtr;

	struct ResTableType {
		ResTable_type header;
		EntryPool entryPool;
		std::vector<const ResTable_entry*> entries;
		std::vector<const Res_value*> values;
	};
	typedef std::shared_ptr<ResTableType> ResTableTypePtr;

	struct PackageResource {
		ResTable_package header;
		ResStringPoolPtr pTypes;
		ResStringPoolPtr pKeys;
		std::map<int, std::list<ResTableTypePtr> > resTablePtrs;
	};
	typedef std::shared_ptr<PackageResource> PackageResourcePtr;

public:
	ResourcesParser(const std::string& filePath);

	std::string getStringFromGlobalStringPool(uint32_t index);

	const std::map<std::string, PackageResourcePtr>& getResourceForPackageName() {
		return mResourceForPackageName;
	}

	static std::string getStringFromResStringPool(
			ResStringPoolPtr pPool,
			uint32_t index);

	std::string stringOfValue(const Res_value* value);
private:
	ResTable_header mResourcesInfo;
	ResStringPoolPtr mGlobalStringPool;

	std::map<std::string, PackageResourcePtr> mResourceForPackageName;
	std::map<uint32_t, PackageResourcePtr> mResourceForId;
	std::vector<ResTable_package> mPackageTables;

	ResStringPoolPtr parserResStringPool(std::ifstream& resources);

	PackageResourcePtr parserPackageResource(std::ifstream& resources);

	EntryPool parserEntryPool(
			std::ifstream& resources,
			uint32_t dataCount,
			uint32_t dataStart);

	uint32_t getEntryPoolDataBuffSize(const uint32_t* pData, uint32_t entryCount);

	const ResTable_entry* getEntryFromEntryPool(EntryPool pool, uint32_t index);

	const Res_value* getValueFromEntry(const ResTable_entry* pEntry);
};

#endif  /*RESOURCES_PARSER_H*/
