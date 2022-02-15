/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 17:34:26 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/15 17:38:44 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock_the_forks(t_philo *philo, t_thinker *thinker, int fork_id)
{
	int	res;

	if (thinker->id % 2)
	{
		pthread_mutex_lock(&philo->forks[fork_id].mutex);
		res = print_mutex("has taken a fork", philo, BLU, thinker->id);
		pthread_mutex_lock(&philo->forks[thinker->id].mutex);
		res = print_mutex("has taken a fork", philo, BLU, thinker->id);
	}
	else
	{
		pthread_mutex_lock(&philo->forks[thinker->id].mutex);
		res = print_mutex("has taken a fork", philo, BLU, thinker->id);
		pthread_mutex_lock(&philo->forks[fork_id].mutex);
		res = print_mutex("has taken a fork", philo, BLU, thinker->id);
	}
	return (res);
}

int	lock_forks(t_philo *philo, t_thinker *thinker)
{
	int	fork_id;
	int	res;

	res = 0;
	if (thinker->id == 0)
		fork_id = philo->number_of_philo - 1;
	else
		fork_id = thinker->id - 1;
	pthread_mutex_lock(&thinker->philo->queue_mutex);
	res = lock_the_forks(philo, thinker, fork_id);
	pthread_mutex_unlock(&thinker->philo->queue_mutex);
	eat_renewal(thinker);
	res = print_mutex("is eating", thinker->philo, GRN, thinker->id);
	add_times_eat(thinker);
	new_usleep(philo->time_to_eat);
	res = print_mutex("is sleeping", thinker->philo, CYN, thinker->id);
	pthread_mutex_unlock(&philo->forks[thinker->id].mutex);
	pthread_mutex_unlock(&philo->forks[fork_id].mutex);
	return (res);
}
