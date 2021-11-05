/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oneph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchatzip <mchatzip@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 12:12:29 by mchatzip          #+#    #+#             */
/*   Updated: 2021/11/05 08:28:27 by mchatzip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print(char *s, int i, t_philo *ph)
{
	pthread_mutex_lock(&ph->write);
	printf("%lld %d %s\n", gettime(), i, s);
	pthread_mutex_unlock(&ph->write);
}

void	onephilo(t_philo *philo)
//since there is only one fork, the philosopher shoulg grab it, and since he can't eat with just 1, he should die time_to_die ms later
{
	print("has taken a fork", 1, philo);
	usleep(philo->die * 1000);
	print("died", 1, philo);
	free(philo);
}

void	errorprint(int argc, t_philo *philo)
{
	if (!philo)
		write(2, "Error\nStruct allocation failed\n", 24); //error massage in case struct alloc failed
	if (argc != 5 && argc != 6)
		write(2, "Error\nWrong number of arguments\n", 32); //that one is self explanantory
	free(philo); //remember to always free any heap blocks be4 exiting a program.
}
