#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS 
////////////////////////////////////////////////////////////////////////////////

bool		is_corrupted(t_object const *object, struct symtab_command const *symtab)
{
	if (object->ptr + symtab->symoff >= object->eof) 
	{
		error(CORRUPTED_FILE, object->name);
		return true;
	}
	if (object->ptr + symtab->stroff >= object->eof) 
	{
		error(CORRUPTED_FILE, object->name);
		return true;
	}
		
	return false;
}

struct symtab_command	*get_symtab(t_object const *object, struct load_command const *lc, uint32_t const ncmds)
{
	struct symtab_command	*symtab = NULL;

	for (uint32_t index = 0; index < ncmds; index++)
	{
		if ((void *)lc >= object->eof)
			return error(CORRUPTED_FILE, object->name);

		if (lc->cmd == LC_SYMTAB)
			symtab = (struct symtab_command *)lc;

		lc = (struct load_command *)((void *)lc + lc->cmdsize);
	}

	return symtab;
}
