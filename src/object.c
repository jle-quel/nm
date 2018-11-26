#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void		*err(t_code const code, char const *filename, t_object *object, int fd)
{
	close(fd);
	free(object);
	return error(code, filename);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__hot_function t_object				*get_object(char *filename, size_t const size)
{
	t_object	*object = constructor(sizeof(t_object));
	int			fd = 0;
	struct stat	st;
	
	if ((fd = open(filename, O_RDONLY, 0644)) == -1)
		return err(OPEN_FAILED, filename, object, fd);
	if (fstat(fd, &st) == -1)
		return err(STAT_FAILED, filename, object, fd);
	if ((object->ptr = mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return err(MMAP_FAILED, filename, object, fd);
	
	if (size > 1)
		object->title = filename;
	object->name = filename;
	object->size = st.st_size;
	object->stupid_nm = false;
	object->eof = object->ptr + object->size;
	close(fd);

	return object;
}
