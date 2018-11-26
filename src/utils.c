#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS 
////////////////////////////////////////////////////////////////////////////////

struct load_command	*get_load_command(t_object const *object, size_t const size)
{
	if (object->ptr + size >= object->eof)
		return error(CORRUPTED_FILE, object->name);

	return (struct load_command *)((void *)object->ptr + size);
}

bool				is_segment_valid(char const *segname, uint32_t const filetype)
{
	if (strcmp(segname, "__TEXT") == 0)
		return true;
	if (strcmp(segname, "__DATA") == 0)
		return true;
	if (filetype == MH_OBJECT)
		return true;

	return false;
}

void				set_stat(t_stat *stat, char const *sectname)
{
	if (strcmp(sectname, SECT_TEXT) == 0)
		stat->text = stat->index + 1;
	else if (strcmp(sectname, SECT_DATA) == 0)
		stat->data = stat->index + 1;
	else if (strcmp(sectname, SECT_BSS) == 0)
		stat->bss = stat->index + 1;
}

bool						check_ptr(char const *str, t_object const *object)
{
	for (size_t index = 0; str[index]; index++)
	{
		if ((void *)str + index + 2 > object->eof)
			return true;
	}

	return false;
}