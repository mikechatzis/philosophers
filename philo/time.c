/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchatzip <mchatzip@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 12:12:29 by mchatzip          #+#    #+#             */
/*   Updated: 2021/11/05 09:08:33 by mchatzip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	gettime(void) //the gettime function gives us the timestamp in micro seconds, according to the unix standard. Man gettimeofday for more details, it's just some math from there
{
	struct timeval		time;
	unsigned long long	ms;

	gettimeofday(&time, NULL);
	ms = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (ms);
}

void	eatsequence(t_philo *ph, int i)
{
	pthread_mutex_lock(&ph->eatstart); //with this mutex, we make sure that no philo will die when he starts eating, which is not allowed
	ph->timedie[i - 1] = gettime(); //store the timestamp of the eat in the appropriate position, overwriting the prev meal.
	print("is eating", i, ph);
	pthread_mutex_unlock(&ph->eatstart); //now that eating is done, the philo is able to die again. Yes, a philo can die WHILE eating, but not when it STARTS eating
	usleep(ph->eat * 1000);
	if (i != 1)
		pthread_mutex_unlock(&ph->forks[i - 2]); //after eating is done, unlock the appropriate mutexes to realease the forks
	else
		pthread_mutex_unlock(&ph->forks[ph->philnum - 1]);
	pthread_mutex_unlock(&ph->forks[i - 1]);
	if (ph->maxeat >= 0) //check if a max_eat condition exists. If so store the times each philo has eaten to an int array, up to the max_eat point
	{					// (any philo can and should continue eating while w8ing for the others to eat max_eat times and after all have done so, the threads should return)
		if (ph->timeseat[i - 1] < ph->maxeat)
			ph->timeseat[i - 1]++;
	}
}

void	lockforks(t_philo *ph, int i)
{
	if (ph->dead) //more death checks
		return ;
	pthread_mutex_lock(&ph->forkt); //this mutex assures that a philosopher will grab exactly 2 forks and not 1, which would result as a block between 2 threads
	if (ph->dead)
	{
		pthread_mutex_unlock(&ph->forkt); //don't forget to unlock any locked mutexes be4 returning, else you will prevent other threads from reaching their exit condition
		return ;
	}
	if (i != 1)
		pthread_mutex_lock(&ph->forks[i - 2]); //remember, philos sit in a circle, so philo no 1's left fork is the last philo's right fork. in any other case it's much simpler
	else
		pthread_mutex_lock(&ph->forks[ph->philnum - 1]);
	print("has picked up a fork", i, ph);
	pthread_mutex_lock(&ph->forks[i - 1]); //here we lock the right fork, which is the same for all philos, including the 1st
	print("has picked up a fork", i, ph);
	pthread_mutex_unlock(&ph->forkt); //unlock the "grab" condition, allowing other philos to attempt to grab forks
}
