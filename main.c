/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 11:28:26 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/13 18:52:08 by pleveque         ###   ########.fr       */
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
	if (id == -1)
	{
		printf("(ms:%lu) %s====> %s <====\n\033[0;37m", (time - first_time), color, content);
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
	pthread_mutex_unlock(&philo->modify_philo);
	return (res);
}

void	change_alive(t_philo *philo, int value)
{
	pthread_mutex_lock(&philo->modify_philo);
	philo->all_alive = value;
	pthread_mutex_unlock(&philo->modify_philo);
}

int	parsing(int argc, char **argv, t_philo *philo)
{
	if (argc < 5)
	{
		write(2, "error\n", 6);
		return (0);
	}
	else
	{
		philo->number_of_philo = ft_atoi(argv[1]);
		philo->time_to_die = ft_atoi(argv[2]);
		philo->time_to_eat = ft_atoi(argv[3]);
		philo->time_to_sleep = ft_atoi(argv[4]);
		if (argc >= 6)
			philo->time_must_eat = ft_atoi(argv[5]);
		else
			philo->time_must_eat = -1;
		philo->all_alive = 1;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo			philo;
	pthread_t		*threads;
	pthread_t		death_thread;
	t_thinker		*thinkers;
	int				i;

	if (!parsing(argc, argv, &philo))
		return (-1);
	pthread_mutex_init(&philo.talk, NULL);
	pthread_mutex_init(&philo.modify_philo, NULL);
	thinkers = malloc(sizeof(t_thinker) * philo.number_of_philo);
	threads = malloc(sizeof(pthread_t) * philo.number_of_philo);
	philo.times_eat = malloc(sizeof(int) * philo.number_of_philo);
	//*definie forks
	philo.forks = malloc(sizeof(t_fork) * philo.number_of_philo);
	i = 0;
	while (i < philo.number_of_philo)
	{
		philo.times_eat[i] = 0;
		philo.forks[i].state = 1;
		pthread_mutex_init(&philo.forks[i].mutex, NULL);
		++i;
	}
	i = 0;
	while (i < philo.number_of_philo)
	{
		thinkers[i].id = i;
		thinkers[i].philo = &philo;
		pthread_mutex_init(&thinkers[i].mutex, NULL);
		eat_renewal(&thinkers[i]);
		pthread_create(&threads[i], NULL, start_routine, &thinkers[i]);
		++i;
	}
	pthread_create(&death_thread, NULL, dead_checker, thinkers);
	i = 0;
	// return (0);
	while (i < philo.number_of_philo)
	{
		pthread_join(threads[i], NULL);
		++i;
	}
	pthread_join(death_thread, NULL);
	pthread_mutex_destroy(&philo.talk);
	//*
	free(thinkers);
	free(threads);
	free(philo.forks);
	free(philo.times_eat);
	return (0);
}
