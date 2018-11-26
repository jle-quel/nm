#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS 
////////////////////////////////////////////////////////////////////////////////

static t_symbol	*get_symbol(struct nlist_64 const symbol_table, char const *string_table, t_stat const *stat)
{
	t_symbol *symbol = constructor(sizeof(t_symbol));

	symbol->value = symbol_table.n_value;
	symbol->type = get_type(stat, symbol_table.n_type, symbol_table.n_sect, symbol_table.n_value);
	symbol->n_type = symbol_table.n_type,
	symbol->name = string_table + symbol_table.n_un.n_strx;

	return symbol;
}

static t_tree	*save_data(t_object const *object, struct symtab_command const *symtab, t_stat const *stat)
{
	struct nlist_64 const	*symbol_table = (struct nlist_64 const *)((void *)object->ptr + symtab->symoff);
	char const 				*string_table = (char const *)(object->ptr + symtab->stroff);
	t_tree					*tree = NULL;

	for (uint32_t index = 0; index < symtab->nsyms; index++)
	{
		if ((void *)string_table + symbol_table[index].n_un.n_strx >= object->eof)
		{
			release((t_tree *)tree);
			return error(CORRUPTED_FILE, object->name);
		}

		if (symbol_table[index].n_type & N_STAB)
			continue;
		else
		{
			if (check_ptr(string_table + symbol_table[index].n_un.n_strx, object))
			{
				release((t_tree *)tree);
				return error(CORRUPTED_FILE, object->name);
			}
			tree = add_to_tree(tree, get_symbol(symbol_table[index], string_table, stat));
		}
	}

	return tree;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS 
////////////////////////////////////////////////////////////////////////////////

void			macho64(t_object *object)
{
	struct mach_header_64 const	*header = (struct mach_header_64 const *)object->ptr;
	struct load_command			*lc = NULL;
	struct symtab_command		*symtab = NULL;
	t_stat						*stat = NULL;
	t_tree						*tree = NULL;

	if ((lc = get_load_command(object, sizeof(struct mach_header_64))) == NULL)
	{
		title(object);
		return ;
	}
	if ((symtab = get_symtab(object, lc, header->ncmds)) == NULL)
	{
		title(object);
		return ;
	}
	if ((stat = get_stat64(object, lc, header)) == NULL)
	{
		title(object);
		return ;
	}
	
	if (is_corrupted(object, symtab) == false)
	{
		if ((tree = save_data(object, symtab, stat)))
		{
			title(object);
			render(tree, &render64);
			release((t_tree *)tree);
		}
	}

	free((void *)stat);
}