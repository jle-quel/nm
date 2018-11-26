#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void	set_title(t_object *object, char const *str)
{
	size_t const	length = strlen(object->name);

	if ((object->title = (char *)malloc(strlen(str) + length + 1)) == NULL)
	{
		printf("fatal: out of memory, malloc failed\n");
		exit(1);
	}

	bzero(object->title, strlen(str) + length + 1);
	strcpy(object->title, object->name);
	strcpy(object->title + strlen(object->title), str);
} 

static inline struct fat_arch	*get_arch(t_object const *object, size_t const size, bool swap)
{
	struct fat_arch *arch = (struct fat_arch *)((void *)object->ptr + sizeof(struct fat_header) + size);
	
	if (swap)
	{
		arch->offset = OSSwapConstInt32(arch->offset);
		arch->cputype = OSSwapConstInt32(arch->cputype);
	}

	return arch;
}

static bool						is_corrupt(t_object const *object, struct fat_header const *header, bool const render)
{
	if (header)
	{
		for (unsigned int index = 0; index < header->nfat_arch; index++)
		{
			if (object->ptr + sizeof(struct fat_header) + (sizeof(struct fat_arch) * index) >= object->eof)
			{
				if (render)
					error(CORRUPTED_FILE, object->name);
				return true;
			}
			if (object->ptr + get_arch(object, sizeof(struct fat_arch) * index, true)->offset >= object->eof)
			{
				if (render)
					error(CORRUPTED_FILE, object->name);
				return true;
			}
		}
		return false;
	}

	if (render)
		error(CORRUPTED_FILE, object->name);
	return true;
}

static inline uint32_t			contains_x86(t_object const *object, struct fat_header const *header)
{
	uint32_t	index = 0;

	while (index < header->nfat_arch)
	{
		if (get_arch(object, sizeof(struct fat_arch) * index, false)->cputype == X86_64)
			break ;
		index++;
	}

	return index;
}

static void						call(t_object *object, unsigned int const index)
{
	struct fat_arch			*arch = get_arch(object, sizeof(struct fat_arch) * index, false);

	object->ptr += arch->offset;
	handler(object);
	object->ptr -= arch->offset;
}

static void						output_my_arch(t_object *object, struct fat_header const *header)
{
	for (unsigned int index = 0; index < header->nfat_arch; index++)
	{
		if (is_corrupt(object, header, false))
			return ;
		set_title(object, " (for architecture i386)");
		call(object, index);
		free(object->title);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void							fat_big(t_object *object)
{
	struct fat_header	*header = (struct fat_header *)object->ptr;
	
	if (header)
	{
		header->nfat_arch = OSSwapConstInt32(header->nfat_arch);
		fat_little(object);
	}
}

void							fat_little(t_object *object)
{
	struct fat_header const	*header = (struct fat_header *)object->ptr;
	uint32_t				index = 0;

	if (is_corrupt(object, header, true) == false)
	{
		if (header->nfat_arch == 1 || (index = contains_x86(object, header)) < header->nfat_arch)
		{
			if (header->nfat_arch == 1)
			{
				object->title = (char *)object->name;
				object->stupid_nm = true;
			}
			else
				object->title = NULL;
			call(object, index);
		}
		else
			output_my_arch(object, header);
	}
}