/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khsadira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 15:28:32 by khsadira          #+#    #+#             */
/*   Updated: 2019/04/05 14:33:03 by khsadira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static void	write_operands(char arg)
{
	if (arg == 'a')
		write(1, "\a", 1);
	else if (arg == 'b')
		write(1, "\b", 1);
	else if (arg == 't')
		write(1, "\t", 1);
	else if (arg == 'n')
		write(1, "\n", 1);
	else if (arg == 'v')
		write(1, "\v", 1);
	else if (arg == 'f')
		write(1, "\f", 1);
	else if (arg == 'r')
		write(1, "\r", 1);
	else if (arg == '\\')
		write(1, "\\", 1);
}

static int	find_operands(char *arg)
{
	int		i;
	char	op[2];

	op[0] = '\\';
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '\\')
		{
			if (arg[i + 1] == 'c')
				return (1);
			else
				write_operands(arg[i + 1]);
			i += 2;
		}
		write(1, arg + i, 1);
		i++;
	}
	return (0);
}

int			built_echo(char **arg, t_envl *envl)
{
	int		i;

	(void)envl;
	i = 1;
	while (arg[i])
	{
		if (ft_strchr(arg[i], '\\'))
		{
			if ((find_operands(arg[i])) == 1)
				return (0);
		}
		else
			write(1, arg[i], ft_strlen(arg[i]));
		if (arg[i + 1])
			write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}
