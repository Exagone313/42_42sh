/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_fn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 15:51:35 by aguillot          #+#    #+#             */
/*   Updated: 2019/04/24 13:14:07 by aguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int		is_line_empty(t_char *tail)
{
	while (tail && !tail->is_prompt)
	{
		if (tail->charac[0] != ' ' && tail->charac[0] != '\n'
			&& tail->charac[0] != '\t')
			return (1);
		if (tail->len == 2)
			if (ft_memcmp(tail->charac, NBSP, 2) != 0)
				return (1);
		tail = tail->prev;
	}
	return (0);
}

static void		return_end(uint8_t *buff, size_t buff_size, int add_to_hist)
{
	rl_free_controler(FREE_ALL_EDIT);
	write(1, "\n", 1);
	ft_putstr(tgetstr("cr", NULL));
	g_shell.edit.reading = FALSE;
	if (g_shell.edit.prompt_id == BASIC_PROMPT)
		buff = (uint8_t *)replace_exclaim((char *)buff,\
		g_shell.hist.history, NULL, NULL);
	rl_line_to_hist(add_to_hist, buff, buff_size);
	buff = (uint8_t *)ft_strfjoin((char *)buff, "\n", 0);
	buff_size = ft_u8_strlen(buff);
	g_shell.line = buff;
	g_shell.line_size = buff_size;
}

void			return_fn(void)
{
	uint8_t		*buff;
	t_char		*head;
	size_t		buff_size;
	int			i;

	if ((head = find_first_non_prompt(g_shell.edit.char_list.head)) == NULL)
		buff_size = 0;
	else
		buff_size = get_buf_size(head, NULL);
	if (!(buff = (uint8_t*)malloc(sizeof(uint8_t) * (buff_size + 1))))
		readline_errors_controler(MALLOC_ERROR);
	buff[buff_size] = '\0';
	i = 0;
	while (head)
	{
		ft_memmove(buff + i, head->charac, head->len);
		i += head->len;
		head = head->next;
	}
	if (is_line_empty(g_shell.edit.char_list.tail))
		i = 1;
	else
		i = 0;
	return_end(buff, buff_size, i);
}