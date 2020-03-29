#ifndef __TRANSFORMER_FILE_H__
#define __TRANSFORMER_FILE_H__

#include <string>
#include <filesystem>

namespace transformer
{

class File
{
public:
	static std::string read(const std::filesystem::path& file_path);
	static void write(const std::filesystem::path& file_path, std::string_view data);
};

}

#endif // !__TRANSFORMER_FILE_H__
