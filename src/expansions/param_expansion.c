/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 15:53:41 by emartine          #+#    #+#             */
/*   Updated: 2019/04/18 15:53:42 by emartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "expansions_internal.h"

static size_t	until_dollar_copy(t_lexer_token *tok, size_t j, size_t k)
{
	size_t	x;

	x = 0;
	while (k + x < j && tok->buffer[k + x] != '$')
		x++;
	buffer_append(tok, tok->buffer + k, x);
	return (x);
}

static char	*expand_tmp_token(char *buffer)
{
	size_t		len;
	t_lexer_token	*t;
	char		*new_buffer;

	t = NULL;
	new_buffer = NULL;
	if (NULL == (t = malloc(sizeof(*t))))
		return (NULL);
	ft_memset(t, 0, sizeof(*t));
	len = ft_strlen(buffer);
	if (NULL == (t->buffer = malloc(len)))
	{
		free(t);
		return (NULL);
	}
	ft_memcpy(t->buffer, buffer, len);
	t->size = len;
	printf("size before = %zu\n", t->size);
	for (size_t i = 0; i < t->size; i++)
		ft_putchar(t->buffer[i]);
	if (buffer_expand(t) < 0)
		return (NULL);
	printf("size then = %zu\n", t->size);
	for (size_t i = 0; i < t->size; i++)
		ft_putchar(t->buffer[i]);
	if (NULL == (new_buffer = malloc(t->size + 1)))
	{
		free(t);
		return (NULL);
	}
	ft_memcpy(new_buffer, t->buffer, t->size);
	new_buffer[t->size] = '\0';
	return (new_buffer);
}

static int		subtype(char *key, size_t start)
{
	if (key[start] == ':')
	{
		if (key[start + 1] == '+')
			return (1);
		if (key[start + 1] == '-')
			return (2);
		if (key[start + 1] == '=')
			return (3);
		if (key[start + 1] == '?')
			return (4);
	}
	return (-1);
}

static int		substitute_param(t_lexer_token *tok, char *key, size_t start, int type)
{
	char	*param;
	char	*word;
	char	*tmp;
	char	*value;

	word = NULL;
	tmp = NULL;
	if (NULL == (param = malloc(start + 1)))
		return (-1);
	ft_memmove(param, key, start);
	param[start] = '\0';
	if (key[start + 2])
	{
		if ((word = malloc(ft_strlen(key) - start + 1)))
		{
			ft_memmove(word, key + start + 2, ft_strlen(key) - start - 1);
			word[ft_strlen(key) - start - 1] = '\0';
		}
		else
			return (-1);
	}
	if (type == 1)
	{
		ft_putendl("type = 1");
		if (!(value = get_env_val(g_shell.envl, param)) || value[0] == '\0')
			buffer_append(tok, NULL, 0);
		else
			buffer_append(tok, (uint8_t *)word, ft_strlen(word)); //TODO EXPAND WORD
	}
	else if (type == 2)
	{
		ft_putendl("type = 2");
		if (!(value = get_env_val(g_shell.envl, param)) || value[0] == '\0')
		{
			ft_putendl("value unset or null");
			buffer_append(tok, (uint8_t *)word, ft_strlen(word)); //TODO EXPAND WORD
		}
		else
		{
			ft_putendl("value exists");
			buffer_append(tok, (uint8_t *)value, ft_strlen(value));
		}
	}
	else if (type == 3)
	{
		ft_putendl("type = 3");
		if (!(value = get_env_val(g_shell.envl, param)) || value[0] == '\0')
		{
			tmp = expand_tmp_token(word);
			push_env(&g_shell.envl, param, tmp, 0); //TODO expand WORD
			buffer_append(tok, (uint8_t *)tmp, ft_strlen(word)); //TODO EXPAND WORD
		}
		else
			buffer_append(tok, (uint8_t *)value, ft_strlen(value));
	}
	else if (type == 4)
	{
		ft_putendl("type = 4");
		if (!(value = get_env_val(g_shell.envl, param)) || value[0] == '\0')
		{
			if (word)
				ft_putendl_fd(word, 2); //TODO EXPAND WORD
			else
			{
				ft_putstr_fd("42sh: ", 2);
				ft_putstr_fd(param, 2);
				ft_putendl_fd(": parameter null or not set", 2);
			}
			return (-1);
		}
		else
			buffer_append(tok, (uint8_t *)value, ft_strlen(value));
	}
	return (0);
}

static int		split_key(t_lexer_token *tok, char *key, size_t start)
{
	if (start == 0)
		return (-1);
		//return (format_error(key)); //TODO comment gerer l'erreur des expansions
	if (key[start] == ':' && isvalsep(key[start + 1]))
		return (substitute_param(tok, key, start, subtype(key, start)));
	if (key[start] == ':' && !isvalsep(key[start + 1]))
		return (-1); //TODO format error
	return (0);
}

static int		sub_string_len(t_lexer_token *tok, char *key)
{
	char	*val;
	char	*res;

	val = NULL;
	val = get_env_val(g_shell.envl, key + 1);
	if (!val)
		res = ft_itoa(0);
	else
		res = ft_itoa(ft_strlen(val));
	return (buffer_append(tok, (uint8_t *)res, ft_strlen(res)));
}

static int		key_format(t_lexer_token *tok, char *key, int brace)
{
	char	*val;
	size_t	i;
	size_t	len;

	val = NULL;
	i = 0;
	if (!brace || isvarstr(key))
		val = get_env_val(g_shell.envl, key);
	else
	{
		if (!ft_strchr(key, ':'))
		{
			if (key[0] == '#' && isvarstr(key + 1))
				return (sub_string_len(tok, key));
			else
			{
				while (isvarchar(key[i]))
					i++;
				len = ft_strlen(key);
				if (len - i > 2)
					return (-1); //print format error
				return (0); //ajouter pattern sub
			}
		}
		else
		{
			while (isvarchar(key[i]))
				i++;
			return (split_key(tok, key, i));
		}
	}
	if (val != NULL && val[0] != '\0')
		buffer_append(tok, (uint8_t *)val, ft_strlen(val));
	return (0);
}

static int	dollar_expand(t_lexer_token *tok, size_t j, size_t k)
{
	size_t	x;
	char	*key;
	int		brace;

	brace = 0;
	if (k + 1 == j)
	{
		buffer_append(tok, (uint8_t *)"$", 1);
		return (1);
	}
	x = 1;
	if (k + 1 < j && tok->buffer[k + 1] == '{')
		brace = 1;
	if (k + 1 < j && (tok->buffer[k + 1] == '$' || tok->buffer[k + 1] == '?'))
		x++;
	else
		while ((isvarchar(tok->buffer[k + x]) && !brace && k + x < j) || (brace && tok->buffer[k + x] != '}'))
			x++;
	if (NULL == (key = malloc(x - brace)))
		fatal_exit(SH_ENOMEM);
	ft_memmove(key, tok->buffer + k + brace + 1, x - brace - 1);
	key[x - brace - 1] = '\0';
	if (key_format(tok, key, brace) < 0)
		tok->exp_error = 1;
	free(key);
	return (brace ? x + 1: x);
}

int			var_expand(t_lexer_token *tok, size_t j, size_t k)
{
	while (k < j)
	{
		if (tok->buffer[k] != '$')
			k += until_dollar_copy(tok, j, k);
		else
		{
			k += dollar_expand(tok, j, k);
		}
		if (tok->exp_error)
		{
			tok->exp_error = 0;
			return (-1);
		}
	}
	return (0);
}
