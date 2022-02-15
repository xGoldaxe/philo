/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 12:24:38 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/15 14:52:19 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_error(char *error_msg)
{
	printf("error: %s\n", error_msg);
	return (0);
}

int	parsing(int argc, char **argv, t_philo *philo)
{
	if (argc < 5)
		return (parse_error("not enough args"));
	philo->number_of_philo = ft_atoi(argv[1]);
	if (philo->number_of_philo <= 0)
		return (parse_error("invalid number_of_philo number"));
	philo->time_to_die = ft_atoi(argv[2]);
	if (philo->time_to_die <= 0)
		return (parse_error("invalid time_to_die number"));
	philo->time_to_eat = ft_atoi(argv[3]);
	if (philo->time_to_eat <= 0)
		return (parse_error("invalid time_to_eat number"));
	philo->time_to_sleep = ft_atoi(argv[4]);
	if (philo->time_to_sleep <= 0)
		return (parse_error("invalid time_to_sleep number"));
	if (argc >= 6)
	{
		philo->time_must_eat = ft_atoi(argv[5]);
		if (philo->time_must_eat < 0)
			return (parse_error("invalid time_must_eat number"));
	}
	else
		philo->time_must_eat = -1;
	philo->all_alive = 1;
	return (1);
}
