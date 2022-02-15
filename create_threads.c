/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 17:39:21 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/15 17:39:31 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_philo *philo, t_thinker *thinkers, pthread_t *threads,
pthread_t *death_thread)
{
	philo->mutex_create = -1;
	while (++philo->mutex_create < philo->number_of_philo)
	{
		philo->times_eat[philo->mutex_create] = 0;
		philo->forks[philo->mutex_create].state = 1;
		if (pthread_mutex_init(&philo->forks[philo->mutex_create].mutex, NULL))
			return (-1);
	}
	philo->philo_created = -1;
	while (++philo->philo_created < philo->number_of_philo)
	{
		thinkers[philo->philo_created].id = philo->philo_created;
		thinkers[philo->philo_created].philo = philo;
		eat_renewal(&thinkers[philo->philo_created]);
		if (pthread_create(&threads[philo->philo_created],
				NULL, start_routine, &thinkers[philo->philo_created]) != 0)
			return (-2);
	}
	if (pthread_create(death_thread, NULL, dead_checker, thinkers))
		return (-3);
	return (1);
}
