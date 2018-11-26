#include <ft_nm.h>

static t_state_machine const sm[] = {
	(t_state_machine){FAT_CIGAM, &fat_big},
	(t_state_machine){FAT_MAGIC, &fat_little}, 
	(t_state_machine){MH_MAGIC_64, &macho64},
	(t_state_machine){MH_MAGIC, &macho32},
};

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__hot_function void handler(t_object *object)
{
	uint32_t const magic_number = *(uint32_t *)object->ptr;

	for (uint16_t index = 0; index < STATE_MACHINE_SIZE; index++)
	{
		if (magic_number == sm[index].magic_number)
		{
			sm[index].f(object);
			return ;
		}
	}

	if (*(uint64_t *)object->ptr == AR_MAGIC)
		archive(object);
	else
		error(FILE_NOT_VALID, object->name);
}

void	nm(char **av)
{
	size_t const	size = get_size(av);
	t_object		*object = NULL;

	for (size_t index = 0; index < size; index++)
	{
		if ((object = get_object(av[index], size)))
		{
			handler(object);
			release(object);
		}
	}
}