/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 12:00:43 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/11 12:15:08 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_base_to_int(char value, char *base)
{
	int	i;

	i = 0;
	while (value != base[i])
	{
		i++;
	}
	return (i);
}

static int	str_to_int_base(const char *str, int size, char *base)
{
	int	result;
	int	i;
	int	base_s;
	int	pow_result;
	int	pow_i;

	result = 0;
	i = 0;
	while (i < size)
	{
		base_s = 0;
		while (base[base_s] != '\0')
			base_s++;
		pow_i = 0;
		pow_result = 1;
		while (pow_i < size - 1 - i)
		{
			pow_result = pow_result * base_s;
			pow_i++;
		}
		result += ft_base_to_int(str[i], base) * pow_result;
		i++;
	}
	return (result);
}

static int	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ');
}

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *nptr)
{
	unsigned int	i;
	unsigned int	size;
	int				negativ;

	negativ = 0;
	i = 0;
	size = 0;
	while (nptr[i] && ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-')
		negativ = 1;
	if (nptr[i] == '+' || nptr[i] == '-')
		i++;
	while (ft_isdigit(nptr[i + size]))
	{
		size++;
	}
	if (negativ)
		return (-str_to_int_base(&nptr[i], size, "0123456789"));
	return (str_to_int_base(&nptr[i], size, "0123456789"));
}
