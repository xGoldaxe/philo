/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 11:27:37 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/22 15:05:05 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include   <limits.h>
# include	<stdio.h>
# include	<stdlib.h>
# include	<unistd.h>
# include	<fcntl.h>
# include	<stdlib.h>
# include	<pthread.h>
# include	<sys/time.h>
# include	<pthread.h>
# include	<string.h>
# define MS	1000
# define S	1000000
# define RED   "\x1B[31m"
# define GRN   "\x1B[32m"
# define YEL   "\x1B[33m"
# define BLU   "\x1B[34m"
# define MAG   "\x1B[35m"
# define CYN   "\x1B[36m"
# define WHT   "\x1B[37m"
# define RESET "\x1B[0m"

typedef struct s_fork {
	int					state;
	pthread_mutex_t		mutex;
}	t_fork;

typedef struct s_philo {
	int					number_of_philo;
	int					philo_created;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_must_eat;
	int					all_alive;
	int					error_occured;
	int					mutex_create;
	int					*times_eat;
	struct timeval		*last_time_eat;
	t_fork				*forks;
	pthread_mutex_t		talk;
	pthread_mutex_t		modify_philo;
	pthread_mutex_t		queue_mutex;
	pthread_mutex_t		start;
}	t_philo;

typedef struct s_thinker {
	t_philo				*philo;
	int					id;
	struct timeval		last_time_eat;
}	t_thinker;

/* UTILS */
int					ft_atoi(const char *nptr);
int					print_mutex(char *content, t_philo *philo,
						char *color, int id);
int					verify_alive(t_philo *entry);
void				change_alive(t_philo *entry, int value);
void				eat_renewal(t_thinker *thinker);
int					verify_all_eats_enough(t_philo *philo);
long unsigned int	get_time(void);
void				new_usleep(int ms);
int					parsing(int argc, char **argv, t_philo *philo);
void				add_times_eat(t_thinker *thinker);
int					erify_eat_time(t_thinker *thinker, int delay);
int					create_threads(t_philo *philo, t_thinker *thinkers,
						pthread_t *threads, pthread_t *death_thread);
int					ft_strlen(char *str);
int					ft_strcmp(char *s1, char *s2);

/* ROUTINE */
void				*start_routine(void *entry);
void				*dead_checker(void *entry);
int					lock_the_forks(t_philo *philo,
						t_thinker *thinker, int fork_id);
int					lock_forks(t_philo *philo, t_thinker *thinker);

/* MUTEX OPERATIONS */
void				change_alive(t_philo *philo, int value);
int					verify_alive(t_philo *philo);
int					verify_all_eats_enough(t_philo *philo);
int					print_mutex(char *content, t_philo *philo,
						char *color, int id);

#endif