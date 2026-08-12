#include "../../includes/core/Main.hpp"

std::string intToString(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

void freeEnvp(char** envp)
{
	if (!envp)
		return;
	for (size_t i = 0; envp[i] != NULL; i++)
		delete[] envp[i];
	delete[] envp;
}
