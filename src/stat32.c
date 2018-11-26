#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS 
////////////////////////////////////////////////////////////////////////////////

static inline struct section	*get_section(t_object const *object, struct segment_command const *segment, uint32_t const index)
{
	if ((void *)segment + sizeof(struct segment_command) + (sizeof(struct section) * index) >= object->eof)
		return error(CORRUPTED_FILE, object->name);

	return (void *)segment + sizeof(struct segment_command) + (sizeof(struct section) * index);
}

static void						get_stat(t_object const *object, t_stat *stat, struct load_command const *lc, uint32_t const filetype)
{
	struct segment_command const	*segment = (struct segment_command const *)lc;
	struct section					*section = NULL;
	
	if (is_segment_valid(segment->segname, filetype))
	{
		for (uint32_t index = 0; index < segment->nsects; index++)
		{
			if ((section = get_section(object, segment, index)) == NULL)
				return ;

			set_stat(stat, section->sectname);
			stat->index++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS 
////////////////////////////////////////////////////////////////////////////////

t_stat							*get_stat32(t_object const *object, struct load_command const *lc, struct mach_header const *header)
{
	t_stat	*stat = constructor(sizeof(t_stat));

	for (uint32_t index = 0; index < header->ncmds; index++)
	{
		if ((void *)lc >= object->eof)
		{
			free(stat);
			return error(CORRUPTED_FILE, object->name);
		}
	
		if (lc->cmd == LC_SEGMENT)
			get_stat(object, stat, lc, header->filetype);

		lc = (struct load_command *)((void *)lc + lc->cmdsize);
	}

	return stat;
}
