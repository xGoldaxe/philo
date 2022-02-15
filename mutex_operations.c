/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 12:47:27 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/15 16:44:43 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_mutex(char *content, t_philo *philo, char *color, int id)
{
	struct timeval				tv;
	int							res;
	long unsigned int			time;
	static long unsigned int	first_time = 0;

	res = 1;
	pthread_mutex_lock(&philo->talk);
	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * MS * MS + tv.tv_usec) / MS;
	if (first_time == 0)
		first_time = time;
	if (id == -2)
	{
		pthread_mutex_unlock(&philo->talk);
		return (res);
	}
	if (id == -1)
	{
		printf("(ms:%lu) %s====> %s <====\n\033[0;37m",
			(time - first_time), color, content);
		pthread_mutex_unlock(&philo->talk);
		return (res);
	}
	if (!verify_alive(philo) || verify_all_eats_enough(philo) == 0)
		res = -1;
	else
		printf("(ms:%lu) (%d) %s | %s\n\033[0;37m", (time - first_time),
			id + 1, color, content);
	pthread_mutex_unlock(&philo->talk);
	return (res);
}

int	verify_all_eats_enough(t_philo *philo)
{
	int	i;

	pthread_mutex_lock(&philo->modify_philo);
	if (philo->time_must_eat == -1)
	{
		pthread_mutex_unlock(&philo->modify_philo);
		return (1);
	}
	i = 0;
	while (i < philo->number_of_philo)
	{
		if (philo->times_eat[i] < philo->time_must_eat)
		{
			pthread_mutex_unlock(&philo->modify_philo);
			return (1);
		}
		++i;
	}
	pthread_mutex_unlock(&philo->modify_philo);
	return (0);
}

int	verify_alive(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->modify_philo);
	res = philo->all_alive;
	if (res && philo->error_occured)
		res = 0;
	pthread_mutex_unlock(&philo->modify_philo);
	return (res);
}

void	change_alive(t_philo *philo, int value)
{
	pthread_mutex_lock(&philo->modify_philo);
	philo->all_alive = value;
	pthread_mutex_unlock(&philo->modify_philo);
}
