/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 11:28:26 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/22 15:10:50 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_destroy(&philo->start);
	i = 0;
	while (i < philo->mutex_create)
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

int	define_philo(t_philo *philo, t_thinker **thinkers, pthread_t **threads)
{
	pthread_mutex_init(&philo->talk, NULL);
	pthread_mutex_init(&philo->modify_philo, NULL);
	pthread_mutex_init(&philo->queue_mutex, NULL);
	pthread_mutex_init(&philo->start, NULL);
	philo->philo_created = 0;
	philo->mutex_create = 0;
	philo->error_occured = 0;
	philo->times_eat = NULL;
	philo->forks = NULL;
	philo->last_time_eat = NULL;
	*thinkers = malloc(sizeof(t_thinker) * philo->number_of_philo);
	*threads = malloc(sizeof(pthread_t) * philo->number_of_philo);
	philo->times_eat = malloc(sizeof(int) * philo->number_of_philo);
	philo->last_time_eat = malloc(sizeof(struct timeval)
			* philo->number_of_philo);
	philo->forks = malloc(sizeof(t_fork) * philo->number_of_philo);
	if (!*thinkers || !*threads || !philo->times_eat || !philo->forks
		|| !philo->last_time_eat)
		return (free_all(*thinkers, *threads, philo));
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo			philo;
	pthread_t		*threads;
	pthread_t		death_thread;
	t_thinker		*thinkers;

	if (!parsing(argc, argv, &philo))
		return (-1);
	thinkers = NULL;
	threads = NULL;
	if (define_philo(&philo, &thinkers, &threads) == 0)
		return (0);
	print_mutex(NULL, &philo, NULL, -2);
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
