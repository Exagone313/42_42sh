#include "twenty_one_sh.h"

void	ft_putendl_fd(char const *s, int fd)
{
	if (fd == STDOUT_FILENO)
	{
		printer_str(&g_shell.out, (char *)s);
		printer_endl(&g_shell.out);
		printer_flush(&g_shell.out);
	}
	else if (fd == STDERR_FILENO)
	{
		printer_str(&g_shell.err, (char *)s);
		printer_endl(&g_shell.err);
		printer_flush(&g_shell.err);
	}
}

void	ft_putstr(char const *s)
{
	printer_str(&g_shell.out, (char *)s);
	printer_flush(&g_shell.out);
}

void	ft_putchar(char ch)
{
	printer_bin(&g_shell.out, (uint8_t *)&ch, 1);
	printer_flush(&g_shell.out);
}

void	ft_putnbr(int nb)
{
	printer_int(&g_shell.out, nb);
	printer_flush(&g_shell.out);
}

void	ft_putu8str(uint8_t *str)
{
	ft_putstr((char *)str);
}
