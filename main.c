/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 11:28:26 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/15 16:26:53 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_philo *philo, t_thinker *thinkers, pthread_t *threads,
pthread_t *death_thread)
{
	int	i;

	i = -1;
	while (++i < philo->number_of_philo)
	{
		philo->times_eat[i] = 0;
		philo->forks[i].state = 1;
		if (pthread_mutex_init(&philo->forks[i].mutex, NULL))
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

int	free_all(t_thinker *thinkers, pthread_t *threads, t_philo *philo)
{
	free(thinkers);
	free(threads);
	free(philo->forks);
	free(philo->times_eat);
	free(philo->last_time_eat);
	return (0);
}

int	free_mutex(t_philo *philo)
{
	int	i;

	pthread_mutex_destroy(&philo->talk);
	pthread_mutex_destroy(&philo->modify_philo);
	pthread_mutex_destroy(&philo->queue_mutex);
	i = 0;
	while (i < philo->philo_created)
	{
		pthread_mutex_destroy(&philo->forks[i].mutex);
		++i;
	}
	return (0);
}

int	free_threads(t_philo *philo, t_thinker *thinkers, pthread_t *threads,
pthread_t *death_thread)
{
	int	i;

	i = 0;
	while (i < philo->philo_created)
	{
		pthread_join(threads[i], NULL);
		++i;
	}
	if (philo->error_occured == 0)
		pthread_join(*death_thread, NULL);
	free_mutex(philo);
	free_all(thinkers, threads, philo);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo			philo;
	pthread_t		*threads;
	pthread_t		death_thread;
	t_thinker		*thinkers;

	if (!parsing(argc, argv, &philo))
		return (-1);
	//* need protection
	pthread_mutex_init(&philo.talk, NULL);
	pthread_mutex_init(&philo.modify_philo, NULL);
	pthread_mutex_init(&philo.queue_mutex, NULL);
	pthread_mutex_init(&philo.start, NULL);
	//** need protection
	thinkers = NULL;
	threads = NULL;
	philo.times_eat = NULL;
	philo.forks = NULL;
	philo.last_time_eat = NULL;
	thinkers = malloc(sizeof(t_thinker) * philo.number_of_philo);
	threads = malloc(sizeof(pthread_t) * philo.number_of_philo);
	philo.times_eat = malloc(sizeof(int) * philo.number_of_philo);
	philo.last_time_eat = malloc(sizeof(struct timeval)
			* philo.number_of_philo);
	philo.forks = malloc(sizeof(t_fork) * philo.number_of_philo);
	if (!thinkers || !threads || !philo.times_eat || !philo.forks
		|| !philo.last_time_eat)
		return (free_all(thinkers, threads, &philo));
	/***/
	print_mutex("hey", &philo, "e", -2);
	philo.error_occured = 0;
	pthread_mutex_lock(&philo.start);
	if (create_threads(&philo, thinkers, threads, &death_thread) != 1)
	{
		philo.error_occured = 1;
		write(1, "error occured\n", 14);
	}
	pthread_mutex_unlock(&philo.start);
	free_threads(&philo, thinkers, threads, &death_thread);
	return (0);
}
