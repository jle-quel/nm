#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline char	get_section_type(t_stat const *stat, uint8_t const n_sect)
{
	if (n_sect == stat->text)
		return ('T');
	if (n_sect == stat->data)
		return ('D');
	if (n_sect == stat->bss)
		return ('B');

	return ('S');
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

char				get_type(t_stat const *stat, uint8_t const n_type, uint8_t const n_sect, uint8_t const n_value)
{
	char	type = '?';

	if ((n_type & N_TYPE) == N_UNDF)
		type = n_value ? 'C' : 'U';
	else if ((n_type & N_TYPE) == N_ABS)
		type = 'A';
	else if ((n_type & N_TYPE) == N_PBUD)
		type = 'U';
	else if ((n_type & N_TYPE) == N_INDR)
		type = 'I';
	else if ((n_type & N_TYPE) == N_SECT)
		type = get_section_type(stat, n_sect);

	if ((n_type & N_STAB) != 0)
		type = 'Z';
	if ((n_type & N_EXT) == 0 && type != '?')
		type += 32;

	return type;
}