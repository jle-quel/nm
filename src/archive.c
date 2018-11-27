#include <ft_nm.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void	set_title(t_object *object, char const *str)
{
	size_t const	length = strlen(object->name);

	object->title = constructor(strlen(str) + length + 3);

	strcpy(object->title, object->name);
	strcpy(object->title + strlen(object->title), "(");
	strcpy(object->title + strlen(object->title), str);
	strcpy(object->title + strlen(object->title), ")");
} 

static struct ar_hdr	*get_ar(t_object *object, struct ar_hdr const *header)
{
	struct ar_hdr		*ar = (struct ar_hdr *)((void *)header + sizeof(struct ar_hdr) + atoi(header->ar_size));
	
	if ((void *)ar > object->eof)
		return NULL;

	object->ptr += (void *)ar - object->ptr;
	object->ptr += sizeof(struct ar_hdr) + atoi(ar->ar_name + 3);

	return (object->ptr >= object->eof) ? NULL : ar;
}

static inline void	call(t_object *object, struct ar_hdr const *ar)
{
	set_title(object, ar->ar_fmag + 2);
	handler(object);
	free(object->title);
}

static inline bool	is_eof(t_object const *object, struct ar_hdr const *ar)
{
	if (object->ptr >= object->eof)
		return true;	
	if (*(uint64_t *)ar == AR_MAGIC)
		return true;
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void				archive(t_object *object)
{
	struct ar_hdr const	*header = (struct ar_hdr *)((void *)object->ptr + 8);
	struct ar_hdr		*ar = NULL;
	char				*name = "";

	ar = get_ar(object, header);
	
	while (ar && is_eof(object, ar) == false)
	{
		if (strcmp(name, ar->ar_fmag + 2) != 0)
			call(object, ar);

		name = ar->ar_fmag + 2;
		ar = get_ar(object, ar);
	}

	object->ptr = (void *)object->eof - object->size;
}