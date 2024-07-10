/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procs_philo_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 21:40:37 by daeha             #+#    #+#             */
/*   Updated: 2024/07/09 21:16:52 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	eat_think_sleep(t_stat *stat);
static void	p_think(t_philo *philo);
static void	p_sleep(t_philo *philo);
static void	p_eat(t_philo *philo, int num_philos);

void	proc_philos(t_stat *stat)
{	
	pthread_t	monitor;

	stat->philo.birth_time = ft_gettime();
	stat->philo.last_meal = stat->philo.birth_time;
	pthread_create(&monitor, NULL, monitoring, stat);
	eat_think_sleep(stat);
	pthread_join(monitor, NULL);
	sem_close(stat->forks);
	sem_close(stat->write);
	sem_close(stat->eat);
	free(stat->pids);
	exit(EXIT_SUCCESS);
}

static void	eat_think_sleep(t_stat *stat)
{
	t_philo	*philo;

	philo = &stat->philo;
	if (philo->name % 2 == 0)
		ft_usleep(1);
	while (TRUE)
	{
		if (philo->count_meal != -2 && philo->current_meal >= philo->count_meal)
			break;
		p_eat(philo, stat->num_philos);
		p_sleep(philo);
		p_think(philo);
	}
}

static void	p_think(t_philo *philo)
{	
	ft_print_semaphore("is thinking", philo);
}

static void	p_sleep(t_philo *philo)
{
	ft_print_semaphore("is sleeping", philo);
	ft_usleep(philo->time_to_sleep);
}

static void	p_eat(t_philo *philo, int num_philos)
{
	sem_wait(philo->forks);
	ft_print_semaphore("has taken a fork", philo);
	if (num_philos == 1)
	{	
		ft_usleep(philo->time_to_die);
		sem_post(philo->forks);
		return ;
	}
	sem_wait(philo->forks);
	ft_print_semaphore("has taken a fork", philo);
	ft_print_semaphore("is eating", philo);
	sem_wait(philo->eat);
	philo->last_meal = ft_gettime();
	sem_post(philo->eat);
	ft_usleep(philo->time_to_eat);
	sem_wait(philo->eat);
	philo->current_meal++;
	sem_post(philo->eat);
	sem_post(philo->forks);
	sem_post(philo->forks);
}