/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_to_eat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 11:44:56 by vvarussa          #+#    #+#             */
/*   Updated: 2022/04/15 20:19:41 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_fork(t_thread_data *data, int fork_index, int value)
{
	pthread_mutex_lock(&data->forks_mutexes[fork_index]);
	data->forks[fork_index] = value;
	pthread_mutex_unlock(&data->forks_mutexes[fork_index]);
}

int		has_died(t_thread_data *data)
{
	int	value;

	pthread_mutex_lock(&data->forks_mutexes[data->number_philo + 1]);
	value = data->forks[data->number_philo];
	pthread_mutex_unlock(&data->forks_mutexes[data->number_philo + 1]);
	return (value);
}

void	die(t_thread_data *data)
{
	pthread_mutex_lock(&data->forks_mutexes[data->number_philo + 1]);
	print_message("died", *data);
	data->forks[data->number_philo] = 1;
	pthread_mutex_unlock(&data->forks_mutexes[data->number_philo + 1]);
}

int		eat(t_thread_data *data)
{
	print_message("has taken a fork", *data);
	print_message("has taken a fork", *data);
	data->forks[data->right_index] = 0;
	pthread_mutex_unlock(&data->forks_mutexes[data->right_index]);
	print_message("is eating", *data);
	gettimeofday(&data->thinking_time, NULL);
	usleep(data->time_to_eat);
	set_fork(data, data->left_index, 1);
	set_fork(data, data->right_index, 1);
	if (has_died(data))
		return (1);
	print_message("is sleeping", *data);
	usleep(data->time_to_sleep);
	return (1);
}

int	try_to_eat(t_thread_data *data)
{
	pthread_mutex_lock(&data->forks_mutexes[data->left_index]);
	if (data->forks[data->left_index])
	{
		data->forks[data->left_index] = 0;
		pthread_mutex_unlock(&data->forks_mutexes[data->left_index]);
		pthread_mutex_lock(&data->forks_mutexes[data->right_index]);
		if (data->forks[data->right_index])
		{
			return (eat(data));
		}
		pthread_mutex_unlock(&data->forks_mutexes[data->right_index]);
		set_fork(data, data->left_index, 1);
		return (0);
	}
	pthread_mutex_unlock(&data->forks_mutexes[data->left_index]);
	if (data->name_of_thread % 2 == 0)
		usleep(1000);
	usleep(1000);
	return (0);
}
