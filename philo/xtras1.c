/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xtras1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchatzip <mchatzip@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 12:12:29 by mchatzip          #+#    #+#             */
/*   Updated: 2021/11/05 08:59:22 by mchatzip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	num;
	int	sign;

	num = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '-' || *str == '+')
	{
		sign = 1 - 2 * (*str++ == '-');
	}
	while (*str >= '0' && *str <= '9')
	{
		if (num > 2147483647 / 10 || (num == 2147483647 / 10 && *str - '0' > 7))
		{
			if (sign == 1)
				return (2147483647);
			else
				return (-2147483648);
		}
		num = 10 * num + (*str++ - '0');
	}
	return (num * sign);
}

int	checkeat(int *s, int i, int len)
{
	int	k;

	k = 0;
	while (--len >= 0)
	{
		if (s[k++] == i)
			continue ;
		else
			return (1);
	}
	return (0);
}

int	allate(t_philo *ph)
{
	if (!checkeat(ph->timeseat, ph->maxeat, ph->philnum)) //simple loop here: check if all the numbers in the appropriate array are equal to max_eat, if so return 0
	{
		ph->dead = 1; //set the exit condition to true. Don't be lazy guys, use a boolean. EFFICIENCY, PEOPLE.
		return (1);
	}	
	return (0);
}

void	eatchecker(t_philo *philo)
{
	int	i;

	i = 0;
	while (!allate(philo) && !philo->dead) //if all of them ate or some1 died this loop will break and we will proceed to free all memory
		continue ;
	philo->allphsate = 1;
	while (i < philo->philnum)
	{
		pthread_mutex_destroy(&philo->forks[i]); //free all our forks with this pthread function
		pthread_join(philo->philos[i++], NULL); //the pthread-join function will "join" the thread passed as an argument to the thread calling the function, in our case the main
	}											//this means that the calling thread will w8 for the passed thread to return, then it will free its resources and continue execution
												//simply put, the main will w8 for all other threads to finish executing right here, before freeing all their memory and continuing execution itself
	pthread_mutex_destroy(&philo->write); //I forgot to destroy both the deathcheck mutex and the eatcheck mutex
	pthread_join(philo->calc, NULL);		//thankfully, when freeing the struct, they were freed too, so no leaks. THIS DOES NOT HAPPEN WITH THREAD POINTERS. Even if u free the pointer, the thread's resources will remain allocated, causing a leak. So use join on all threads
}

void	*routine(void *id)
{
	t_philo	*ph;

	ph = (t_philo *)id;
	while (!chkdeath(ph, 0)) //the function loops eternally, unless the death conditions are fulfilled, in which case it returns 0
		continue ;
	return (0);
}
