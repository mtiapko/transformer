#include "transformer/Exception.h"
#include "transformer/Generator.h"
#include "transformer/Log.h"

int main(int argc, char* argv[])
{
	try {
		trans::Config    config { argc, argv };
		trans::Parser    parser { config };
		trans::Generator gen    { config, parser };
	} catch (const trans::Exception& e) {
		TRANS_PRINT_ERR(e);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
