/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 21:40:37 by daeha             #+#    #+#             */
/*   Updated: 2024/07/09 00:54:05 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	p_think(t_philo *philo);
static void	p_sleep(t_philo *philo);
static void	p_eat(t_philo *philo);

void	*eat_think_sleep(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->name % 2 == 0)
		ft_usleep(1);
	while (!is_philo_terminated(philo))
	{	
		if (philo->count_meal != OPTION_OFF && philo->current_meal >= philo->count_meal)
			break ;
		p_eat(philo);
		p_sleep(philo);
		p_think(philo);
	}
	return (arg);
}

int	is_philo_terminated(t_philo *philo)
{	
	pthread_mutex_lock(philo->dead);
	if (*philo->terminate)
	{
		pthread_mutex_unlock(philo->dead);
		return (TRUE);
	}
	pthread_mutex_unlock(philo->dead);
	return (FALSE);
}

static void	p_think(t_philo *philo)
{	
	ft_print_mutex("is thinking", philo);
}

static void	p_sleep(t_philo *philo)
{
	ft_print_mutex("is sleeping", philo);
	ft_usleep(philo->time_to_sleep);
}

static void p_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	ft_print_mutex("has taken a fork", philo);
	if (philo->l_fork == philo->r_fork)
	{	
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->r_fork);
		return ;
	}
	pthread_mutex_lock(philo->l_fork);
	ft_print_mutex("has taken a fork", philo);
	ft_print_mutex("is eating", philo);
	pthread_mutex_lock(philo->eat);
	philo->is_eating = TRUE;
	philo->last_meal = ft_gettime();
	pthread_mutex_unlock(philo->eat);
	ft_usleep(philo->time_to_eat);
	pthread_mutex_lock(philo->eat);
	philo->is_eating = FALSE;
	philo->current_meal++;
	pthread_mutex_unlock(philo->eat);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}