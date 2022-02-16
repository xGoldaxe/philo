/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 11:50:48 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/16 12:52:44 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_renewal(t_thinker *thinker)
{
	pthread_mutex_lock(&thinker->philo->modify_philo);
	gettimeofday(&thinker->philo->last_time_eat[thinker->id], NULL);
	pthread_mutex_unlock(&thinker->philo->modify_philo);
}

void	add_times_eat(t_thinker *thinker)
{
	pthread_mutex_lock(&thinker->philo->modify_philo);
	thinker->philo->times_eat[thinker->id] += 1;
	pthread_mutex_unlock(&thinker->philo->modify_philo);
}

int	verify_eat_time(t_thinker *thinker, int delay)
{
	struct timeval	tv;
	int				res;	
	t_philo			*philo;

	philo = thinker->philo;
	pthread_mutex_lock(&philo->modify_philo);
	gettimeofday(&tv, NULL);
	res = (tv.tv_sec * 1000 + tv.tv_usec / MS)
		- (philo->last_time_eat[thinker->id].tv_sec * 1000
			+ philo->last_time_eat[thinker->id].tv_usec / MS)
		> philo->time_to_die + delay;
	pthread_mutex_unlock(&philo->modify_philo);
	return (res);
}

void	*start_routine(void *entry)
{
	t_thinker		*thinker;

	thinker = (t_thinker *)entry;
	pthread_mutex_lock(&thinker->philo->start);
	pthread_mutex_unlock(&thinker->philo->start);
	if (thinker->philo->number_of_philo == 1)
		return (NULL);
	if (thinker->id % 2)
		new_usleep(thinker->philo->time_to_eat / 2);
	if (thinker->philo->number_of_philo % 2
		&& (thinker->id + 1) == thinker->philo->number_of_philo)
		new_usleep(thinker->philo->time_to_eat
			+ thinker->philo->time_to_eat / 2);
	while (verify_alive(thinker->philo))
	{
		if (!lock_forks(thinker->philo, thinker))
			return (NULL);
		new_usleep(thinker->philo->time_to_sleep);
		if (!print_mutex("is thinking", thinker->philo, YEL, thinker->id))
			return (NULL);
		if (thinker->philo->number_of_philo % 2)
			usleep((thinker->philo->time_to_eat * 2
					- thinker->philo->time_to_sleep) * 1000);
	}
	return (NULL);
}

void	*dead_checker(void *entry)
{
	int				i;
	t_thinker		*thinkers;
	int				number_of_philo;

	thinkers = (t_thinker *)entry;
	pthread_mutex_lock(&thinkers->philo->start);
	pthread_mutex_unlock(&thinkers->philo->start);
	number_of_philo = thinkers->philo->number_of_philo;
	while (1)
	{
		i = -1;
		while (++i < number_of_philo)
		{
			if (verify_all_eats_enough(thinkers->philo) == 0)
			{
				print_mutex("everyone eats enough", thinkers[0].philo, CYN, -1);
				return (change_alive(thinkers[i].philo, 0), NULL);
			}
			else if (verify_eat_time(&thinkers[i], 0))
			{
				print_mutex("died", thinkers[i].philo, RED, thinkers[i].id);
				return (change_alive(thinkers[i].philo, 0), NULL);
			}
		}
	}
}
