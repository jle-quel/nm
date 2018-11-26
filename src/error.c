#include <ft_nm.h>

static const char *err[] =
{
	(char const *){"no such file or directory"},
	(char const *){"file not supported"},
	(char const *){"failed to open file"},
	(char const *){"corrupted or truncated file"},
	(char const *){"failed to fstat file"},
	(char const *){"failed to mmap file"}
};

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void		*error(t_code const code, char const *filename)
{
	printf("ft_nm: %s \"%s\"\n", err[code], filename);
	set_result(EXIT_FAILURE);

	return NULL;
}

void		set_result(uint8_t const result)
{
	*get_result() = result;
}

uint8_t		*get_result(void)
{
	static uint8_t	result;

	return &result;
}