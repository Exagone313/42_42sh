/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_controler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 17:16:17 by aguillot          #+#    #+#             */
/*   Updated: 2019/03/11 14:43:22 by schakor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void	free_all_and_exit(void)
{
	t_char *curr;
	t_char *tmp;

	curr = g_shell.edit.char_list.head;
	tmp = NULL;
	if (!curr)
		return ;
	while (curr)
	{
		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
	exit(0);
}

void	free_all(void)
{
	t_char *curr;
	t_char *tmp;

	curr = g_shell.edit.char_list.head;
	tmp = NULL;
	if (!curr)
		return ;
	while (curr)
	{
		tmp = curr->next;
		free(curr);
		curr = NULL;
		curr = tmp;
	}
	/*
	if (g_shell.edit.cpy_buff) {
		free(g_shell.edit.cpy_buff);
	}*/
}

void	free_controler(int code)
{
	if (code == FREE_ALL_AND_EXIT)
		free_all_and_exit();
	if (code == FREE_ALL)
		free_all();
}
