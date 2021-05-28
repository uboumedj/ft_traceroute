#include "../inc/ft_traceroute.h"

/*
** Utilitary functions useful for the project, mostly extracted from my libft
*/

int		ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while ((unsigned char)s1[i] == (unsigned char)s2[i] && s1[i] != '\0')
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

void	*ft_memset(void *ptr, int value, size_t len)
{
	unsigned char	*str;
	size_t			i;

	str = (unsigned char *)ptr;
	i = 0;
	while (i < len)
	{
		str[i] = (unsigned char)value;
		i++;
	}
	return (ptr);
}

void	*ft_memcpy(void *dest, const void *src, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*res;
	size_t	len;
	int		i;

	len = ft_strlen(s);
	if (!(res = (char *)malloc(sizeof(char) * len + 1)))
	{
		return (NULL);
	}
	i = 0;
	while (s[i] != '\0')
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

size_t		ft_strlen(const char *str)
{
	int len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}
