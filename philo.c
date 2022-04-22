/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 13:10:36 by vvarussa          #+#    #+#             */
/*   Updated: 2022/04/22 15:15:57 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*thread(void *data)
{
	t_thread_data	*d;
	long			timestamp;

	d = (t_thread_data *)data;
	define_fork_indexes(data);
	gettimeofday(&d->begin, NULL);
	gettimeofday(&d->thinking_time, NULL);
	while (d->number_of_meals != 0 && !has_died(data))
	{
		print_message("is thinking", *d);
		while (!try_to_eat(d) && !has_died(data))
		{
			gettimeofday(&d->end, NULL);
			timestamp = get_interval_time(d->end, d->thinking_time);
			if (timestamp > d->time_to_die)
				die(data);
		}
		d->number_of_meals = d->number_of_meals - 1;
	}
	free(data);
	return (NULL);
}

void	init_mutexes(t_thread_data *data, pthread_mutex_t *m)
{
	int	n;

	n = data->number_philo + 2;
	while (n > 0)
	{
		n--;
		pthread_mutex_init(&m[n], NULL);
	}
	data->forks_mutexes = m;
}

void	run(pthread_t *philosophers, t_thread_data *data)
{
	int	n;

	n = data->number_philo;
	while (n > 0)
	{
		n--;
		if (pthread_create(&philosophers[n], NULL,
				&thread, alloc_thread_data(*data, n + 1)) != 0)
			perror("failed to create thread");
	}
}

void	run_threads(t_thread_data data)
{
	pthread_t		*philosophers;
	pthread_mutex_t	*mutexes;
	// pthread_t		philosophers[data.number_philo];
	// pthread_mutex_t	mutexes[data.number_philo + 2];
	int				n;

	philosophers = (pthread_t *) malloc(sizeof(pthread_t) * data.number_philo);
	mutexes = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * (data.number_philo + 2));
	init_mutexes(&data, mutexes);
	run(philosophers, &data);
	n = data.number_philo;
	while (n > 0)
	{
		n--;
		if (pthread_join(philosophers[n], NULL) != 0)
			perror("failed to create thread");
	}
	free(philosophers);
}

int	main(int argc, char **argv)
{
	t_thread_data	data;

	data = init_thread_data(argc, argv);
	run_threads(data);
	free(data.forks);
	free(data.forks_mutexes);
}
