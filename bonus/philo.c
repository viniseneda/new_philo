/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 13:10:36 by vvarussa          #+#    #+#             */
/*   Updated: 2022/04/20 23:52:34 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long    get_interval_time(struct timeval end, struct timeval begin)
{
	return ((end.tv_sec * 1000000 + end.tv_usec) / 1000) -
		((begin.tv_sec * 1000000 + begin.tv_usec) / 1000);
}

void	print_message(char *message, t_thread_data data)
{
	long timestamp;

	sem_wait(data.print);
	gettimeofday(&data.end, NULL);
	timestamp = get_interval_time(data.end, data.begin);
	printf("%-5ld %d %s\n", timestamp, data.name_of_thread, message);
	sem_post(data.print);
}

int     has_died(t_thread_data *data)
{
	long timestamp;

	gettimeofday(&data->end, NULL);
	timestamp = get_interval_time(data->end, data->thinking_time);
	if (timestamp > data->time_to_die)
		return (1);
	return (0);
}

void	check_death(t_thread_data data)
{
	if (has_died(&data))
	{
		print_message("died", data);
		sem_wait(data.print);
		// printf("%ld\n", get_interval_time(data.end, data.thinking_time));
		sem_post(data.kill);
	}
}

void	one_philosopher(t_thread_data data)
{
	print_message("is waiting", data);
	while(1)
	{
		check_death(data);
		usleep(100);
	}
}

void	child_process(t_thread_data data)
{
	data.print = sem_open("print", O_CREAT, 0600, 1);
	data.forks = sem_open("fork", O_CREAT, 0600, data.number_philo / 2);
	gettimeofday(&data.begin, NULL);
	gettimeofday(&data.thinking_time, NULL);
	// printf("%d\n", data.number_philo / 2);
	if (data.number_philo == 1)
		one_philosopher(data);
	while(data.number_of_meals != 0)
	{
		print_message("is waiting", data);
		if (data.number_philo % 2)
			usleep(100);
		sem_wait(data.forks);
		check_death(data);
		print_message("is eating", data);
		gettimeofday(&data.thinking_time, NULL);
		usleep(data.time_to_eat);
		check_death(data);
		sem_post(data.forks);
		print_message("is sleeping", data);
		usleep(data.time_to_sleep);
		check_death(data);
		data.number_of_meals--;
	}
	exit(0);
}

void    run_processes(t_thread_data data)
{
	int n;
	int pid;
	int pid2;

	n = data.number_philo;
	sem_unlink("print");
	sem_unlink("fork");
	sem_unlink("kill");
	data.kill = sem_open("kill", O_CREAT, 0600, 0);
	while (n > 0)
	{
		pid = fork();
		if (pid == 0)
		{
			data.name_of_thread = n;
			child_process(data);
		}
		pid2 = fork();
		if (pid2 == 0)
		{
			sem_wait(data.kill);
			kill(pid, SIGKILL);
			sem_post(data.kill);
			exit(0);
		}
		n--;
	}
	while(n < data.number_philo)
	{
		wait(NULL);
		n++;
	}
	sem_post(data.kill);
	sem_unlink("print");
	sem_unlink("fork");
	sem_unlink("kill");
	exit(0);
}

int main(int argc, char **argv)
{
	t_thread_data data;

	setbuf(stdout, NULL);
	data = init_thread_data(argc, argv);
	run_processes(data);
}