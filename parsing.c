/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 12:24:38 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/22 15:11:39 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_error(char *error_msg)
{
	printf("error: %s\n", error_msg);
	return (0);
}

int	ft_onlynumber(char *str)
{
	int	i;

	i = 0;
	if (!str[0])
		return (0);
	if (str[i] == '-')
		++i;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		++i;
	}
	return (1);
}

int	is_integer_atoi(char *str)
{
	int	size;

	size = ft_strlen(str);
	if (size > 11)
		return (-1);
	if (size == 11)
	{
		if (str[0] != '-')
			return (0);
		if (ft_strcmp(&str[1], "2147483648") > 0)
			return (-1);
	}
	if (size == 10)
		if (ft_strcmp(str, "2147483647") > 0)
			return (-1);
	if (!ft_onlynumber(str))
		return (-1);
	return (ft_atoi(str));
}

int	parsing(int argc, char **argv, t_philo *philo)
{
	if (argc < 5 || argc > 6)
		return (parse_error("bad args number"));
	philo->number_of_philo = is_integer_atoi(argv[1]);
	if (philo->number_of_philo <= 0)
		return (parse_error("invalid number_of_philo number"));
	philo->time_to_die = is_integer_atoi(argv[2]);
	if (philo->time_to_die <= 0)
		return (parse_error("invalid time_to_die number"));
	philo->time_to_eat = is_integer_atoi(argv[3]);
	if (philo->time_to_eat <= 0)
		return (parse_error("invalid time_to_eat number"));
	philo->time_to_sleep = is_integer_atoi(argv[4]);
	if (philo->time_to_sleep <= 0)
		return (parse_error("invalid time_to_sleep number"));
	if (argc >= 6)
	{
		philo->time_must_eat = is_integer_atoi(argv[5]);
		if (philo->time_must_eat < 0)
			return (parse_error("invalid time_must_eat number"));
	}
	else
		philo->time_must_eat = -1;
	philo->all_alive = 1;
	return (1);
}
