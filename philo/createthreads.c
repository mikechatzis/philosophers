/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   createthreads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchatzip <mchatzip@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 12:12:29 by mchatzip          #+#    #+#             */
/*   Updated: 2021/11/05 09:07:20 by mchatzip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	loop(t_philo *ph, int i)
{
	while (!ph->dead)
	{
		lockforks(ph, i); //the sequence where each philosopher attempts to grab forks
		if (ph->dead) //multiple death checks ensure that once a philosopher dies, all threads will return asap, so our program can end
			return ;
		eatsequence(ph, i); //the eating sequence, and the checks for max eat num, in case we have a 5th arg
		print("is sleeping", i, ph); //a simple printf, but guarded by a mutex to avoid any scrambling
		if (ph->dead)
			return ;
		usleep(ph->sleep * 1000);
		if (ph->dead)
			return ;
		print("is thinking", i, ph);
	}
}

void	*testf(void *id) //The function each philosopher executes
{
	int		i;	//a local index that stores each philo's number
	t_philo	*ph; //again, declare a struct pointer to assign to our struct

	ph = (t_philo *)id; //and cast our void pointer from be4 right back to struct pointer. Now we can use our struct inside the threads.
	i = ph->g_philnum;
	ph->g_philnum++; //increment our philosophers number so the next thread created will have the right one
	if (i % 2)      //this here secures that no unecessary competition happens. Basically half the threads will w8 be4 executing so the other half will be able to grab their forks.
		usleep(800);
	loop(ph, i); //the philosopher's actions loop
	return (0);
}

void	philocreat(char *s, t_philo *philo)
{
	int	pn;
	int	i;

	pn = ft_atoi(s);
	i = 0;
	while (pn-- > 0)
	{
		pthread_create(&philo->philos[i], NULL, testf, (void *)philo); //the pthread_create function creates a thread and stores it to the memory location pointed to by the 1st argument.
		philo->timedie[i] = gettime();	//The thread will then execute the function passed as a second argument, that itself should take a void pointer passed to it as an argument (the pthread_create 3rd argument)
		//the timedie array of unsigned longs stores each philosopher's last meal timestamp. It is initialised here with the thread's creation time.
		if (!philo->philos[i])			//Here I cast my struct pointer as a void one, so I can pass and use my struct inside the thread function right at creation
		{								//A guard condition, in case any thread alloc fails
			printf("philo %d failed to join the table\n", i);
			return ;
		}
		i++;
	}
	pthread_create(&philo->calc, NULL, routine, (void *)philo); //this extra thread is the one that checks the death conditions. Running it separately from the philosophers ensures better exec speed.
}

void	forkcreat(char *s, t_philo *philo)
{
	int	pn;
	int	i;

	pn = ft_atoi(s);
	i = 0;
	while (pn-- > 0)
		pthread_mutex_init(&philo->forks[i++], NULL); //there should be a guard here, in case the init of any fork fails, but again, lazy
}
