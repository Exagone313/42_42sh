/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 16:17:36 by aguillot          #+#    #+#             */
/*   Updated: 2019/03/12 19:40:23 by schakor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

t_char	*find_first_non_prompt(t_char *head)
{
	while (head && head->is_prompt == 1)
		head = head->next;
	return (head);
}

static size_t		get_buff_size(t_char *head)
{
	size_t	buff_size;

	buff_size = 0;
	while (head)
	{
		buff_size += head->len;
		head = head->next;
	}
	return (buff_size);
}

void	return_fn(void)
{
	uint8_t		*buff;
	t_char		*head;
	size_t		buff_size;
	int			i;

	if ((head = find_first_non_prompt(g_shell.edit.char_list.head)) == NULL)
		return ;
	buff_size = get_buff_size(head);
	if (!(buff = (uint8_t*)malloc(sizeof(uint8_t) * (buff_size + 2))))
		readline_errors_controler(MALLOC_ERROR);
	buff[buff_size + 1] = '\0';
	buff[buff_size] = '\n';
	i = 0;
	while (head)
	{
		ft_memmove(buff + i, head->charac, head->len);
		i += head->len;
		head = head->next;
	}
	free_controler(FREE_ALL);
	write(1, "\n", 1);
	g_shell.edit.reading = FALSE;
	g_shell.line = buff;
	g_shell.line_size = buff_size + 1;
}

void	check_all_pos(void)
{
	t_char			*curr;
	struct winsize	max;

	curr = g_shell.edit.char_list.tail;
	max = g_shell.edit.term_info.max;
	if (curr->y_pos + 1 == max.ws_row
			&& (curr->x_pos + 1 == max.ws_col || curr->charac[0] == '\n'))
	{
		if (curr->charac[0] != '\n')
			write(1, "\n", 1);
		shift_pos_up();
	}
}

void	eot_fn(void)
{
	if (g_shell.edit.char_list.tail->is_prompt == 1)
	{
		cooked_terminal();
		free_controler(FREE_ALL_AND_EXIT);
	}
	supr_charac();
}
