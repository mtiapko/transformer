#include "transformer/Exception.h"
#include "transformer/Generator.h"
#include "transformer/Log.h"

int main(int argc, char* argv[])
{
	try {
		transformer::Config    config { argc, argv };
		transformer::Parser    parser { config };
		transformer::Generator gen    { config, parser };
	} catch (const transformer::Exception& e) {
		TF_PRINT_ERR(e);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
