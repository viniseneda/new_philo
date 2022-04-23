/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 13:10:36 by vvarussa          #+#    #+#             */
/*   Updated: 2022/04/23 04:26:41 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_interval_time(struct timeval end, struct timeval begin)
{
	return (((end.tv_sec * 1000000 + end.tv_usec) / 1000)
		-((begin.tv_sec * 1000000 + begin.tv_usec) / 1000));
}

void	print_message(char *message, t_thread_data data, int n)
{
	long	timestamp;

	sem_wait(data.print);
	gettimeofday(&data.end, NULL);
	timestamp = get_interval_time(data.end, data.begin);
	while (n-- > 0)
		printf("%-5ld %d %s\n", timestamp, data.name_of_thread, message);
	sem_post(data.print);
}

int	has_died(t_thread_data *data)
{
	long	timestamp;

	gettimeofday(&data->end, NULL);
	timestamp = get_interval_time(data->end, data->thinking_time);
	if (timestamp > data->time_to_die)
		return (1);
	return (0);
}

long	get_time_til_death(t_thread_data *data)
{
	long	timestamp;

	gettimeofday(&data->end, NULL);
	timestamp = get_interval_time(data->end, data->thinking_time);
	return (data->time_to_die - timestamp);
}

void	check_death(t_thread_data data)
{
	if (has_died(&data))
	{
		print_message("died", data, 1);
		sem_wait(data.print);
		sem_post(data.kill);
	}
}

void	check_wait_death(t_thread_data data)
{
	long	timestamp;
	long	death_time;
	
	if (has_died(&data))
	{
		sem_wait(data.print);
		gettimeofday(&data.end, NULL);
		timestamp = get_interval_time(data.end, data.begin);
		death_time = get_time_til_death(&data);
		timestamp += death_time;
		printf("%-5ld %d %s\n", timestamp, data.name_of_thread, "died");
		
		sem_post(data.kill);
	}
}


void	one_philosopher(t_thread_data data)
{
	print_message("is waiting", data, 1);
	while (1)
	{
		check_death(data);
		usleep(100);
	}
}


void	eat_or_sleep2(t_thread_data *data, long time_to_eat_or_sleep)
{
	long	time;
	
	check_death(*data);
	time = get_time_til_death(data) * 1000;
	// printf("(%ld, %ld)\n", time, time_to_eat_or_sleep);
	if (time < time_to_eat_or_sleep)
	{
		usleep(time + 1000);
		check_death(*data);
	}
	usleep(time_to_eat_or_sleep);
}

void	eat(t_thread_data *data)
{
	
	print_message("is waiting", *data, 1);
	if (data->number_philo % 2)
		usleep(100);
	sem_wait(data->forks);
	check_wait_death(*data);
	// check_death(*data);
	print_message("has taken a fork", *data, 2);
	print_message("is eating", *data, 1);
	gettimeofday(&data->thinking_time, NULL);
	eat_or_sleep2(data, data->time_to_eat);
	sem_post(data->forks);
	print_message("is sleeping", *data, 1);
	eat_or_sleep2(data, data->time_to_sleep);
}

void	child_process(t_thread_data data)
{
	
	data.print = sem_open("print", O_CREAT, 0600, 1);
	data.forks = sem_open("fork", O_CREAT, 0600, data.number_philo / 2);
	gettimeofday(&data.begin, NULL);
	gettimeofday(&data.thinking_time, NULL);
	if (data.number_philo == 1)
		one_philosopher(data);
	while (data.number_of_meals != 0)
	{
		eat(&data);
		data.number_of_meals--;
	}
	sem_close(data.print);
	sem_close(data.forks);
	sem_close(data.kill);
	exit(0);
}

void	unlink_semaphore()
{
	sem_unlink("print");
	sem_unlink("fork");
	sem_unlink("kill");
}

void	create_child_processes(t_thread_data data, int n)
{
	int	pid;
	int	pid2;
	
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
		sem_close(data.kill);
		exit(0);
	}
}

void	run_processes(t_thread_data data)
{
	int	n;

	n = data.number_philo;
	unlink_semaphore();
	data.kill = sem_open("kill", O_CREAT, 0600, 0);
	while (n > 0)
	{
		create_child_processes(data, n);
		n--;
	}
	while (n < data.number_philo)
	{
		wait(NULL);
		n++;
	}
	sem_post(data.kill);
	sem_close(data.kill);
	exit(0);
}

int	main(int argc, char **argv)
{
	t_thread_data	data;

	setbuf(stdout, NULL);
	data = init_thread_data(argc, argv);
	run_processes(data);
}
