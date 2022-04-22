/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 14:15:30 by vvarussa          #+#    #+#             */
/*   Updated: 2022/04/22 20:48:09 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_interval_time(struct timeval end, struct timeval begin)
{
	return (((end.tv_sec * 1000000 + end.tv_usec) / 1000)
		- ((begin.tv_sec * 1000000 + begin.tv_usec) / 1000));
}

void	print_message(char *message, t_thread_data data)
{
	long	timestamp;

	gettimeofday(&data.end, NULL);
	timestamp = get_interval_time(data.end, data.begin);
	pthread_mutex_lock(&data.forks_mutexes[data.number_philo]);
	printf("%-5ld %d %s\n", timestamp, data.name_of_thread, message);
	pthread_mutex_unlock(&data.forks_mutexes[data.number_philo]);
}

void	print_data_mute(t_thread_data data)
{
	gettimeofday(&data.end, NULL);
	pthread_mutex_lock(&data.forks_mutexes[data.number_philo]);
	print_thread_data(data);
	pthread_mutex_unlock(&data.forks_mutexes[data.number_philo]);
}

void	define_fork_indexes(t_thread_data *data)
{
	if (data->name_of_thread - 2 < 0)
		data->left_index = data->number_philo - 1;
	else
		data->left_index = data->name_of_thread - 2;
	data->right_index = data->name_of_thread - 1;
}
