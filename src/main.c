#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline bool	default_file_exist(void)
{
	int fd = 0;

	if ((fd = open("./a.out", O_RDONLY, 0644)) == -1)
		return false;

	close(fd);

	return true;
}

static void			handle_default_file(void)
{
	char *vector[] =
	{
		(char *){"a.out"},
		(char *){NULL}
	};

	default_file_exist()  == false ? error(NO_FILE, "a.out") : nm(vector);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int					main(int ac, char **av)
{
	set_result(EXIT_SUCCESS);

	(ac < VALID_ARGV_SIZE) ? handle_default_file() : nm(av + 1);

	return *get_result();
}