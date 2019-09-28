#include <fstream>
#include "transformer/Exception.h"
#include "transformer/File.h"

namespace trans
{

/* static */ std::string File::read(const std::filesystem::path& file_path)
{
	std::ifstream file(file_path, std::ios::in | std::ios::ate | std::ios::binary);
	if (!file.is_open())
		throw TRANS_EXCEPTION("Failed to open file '", file_path.string(), '\'');

	std::string data;
	data.resize(file.tellg());

	file.seekg(std::ios::beg);
	file.read(data.data(), data.size());
	if (file.fail())
		throw TRANS_EXCEPTION("Failed to read from file '", file_path.string(), '\'');

	return data;
}

/* static */ void File::write(const std::filesystem::path& file_path, std::string_view data)
{
	std::ofstream file(file_path, std::ios::out | std::ios::binary);
	if (!file.is_open())
		throw TRANS_EXCEPTION("Failed to open file '", file_path.string(), '\'');

	file.write(data.data(), data.size());
	if (file.fail())
		throw TRANS_EXCEPTION("Failed to write to file '", file_path.string(), '\'');
}

}
