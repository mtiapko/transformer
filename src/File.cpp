#include <fstream>

#include "transformer/File.h"
#include "transformer/Log.h"

namespace transformer
{

namespace /* anonymous */
{
	template<typename... Args>
	[[noreturn]] void print_error_and_exit(std::string_view file_path,
		std::string_view op, const Args&... args) noexcept
	{
		TF_LOG_ERROR_IMPL(args..., "Failed to ", op, " file '", file_path, "': ",
			std::generic_category().message(errno));

		std::exit(EXIT_FAILURE);
	}
}

/* static */ std::string File::read(const std::filesystem::path& file_path)
{
	std::ifstream file;
	file.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try {
		file.open(file_path, std::ios::in | std::ios::ate | std::ios::binary);
	} catch (const std::system_error&) {
		print_error_and_exit(file_path.native(), "open for reading", TF_LOG_FMT);
	}

	std::string data;
	data.resize(file.tellg());

	file.seekg(std::ios::beg);

	try {
		file.read(data.data(), data.size());
	} catch (const std::ios_base::failure&) {
		print_error_and_exit(file_path.native(), "read from", TF_LOG_FMT);
	}

	return data;
}

/* static */ void File::write(const std::filesystem::path& file_path, std::string_view data,
	bool extra_new_line /* = false */, bool append /* = false */)
{
	std::ofstream file;
	file.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try {
		file.open(file_path, std::ios::binary
			| (append
				? std::ios::out | std::ios::app
				: std::ios::out));

	} catch (const std::ios_base::failure&) {
		print_error_and_exit(file_path.native(), "open for writing", TF_LOG_FMT);
	}

	try {
		file.write(data.data(), data.size());

		if (extra_new_line)
			file.write("\n", 1);
	} catch (const std::ios_base::failure&) {
		print_error_and_exit(file_path.native(), "write to", TF_LOG_FMT);
	}
}

}
