/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchatzip <mchatzip@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 12:12:29 by mchatzip          #+#    #+#             */
/*   Updated: 2021/11/05 09:04:19 by mchatzip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	findchr(const char c, const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	wrongarg(const char *s, t_philo *philo)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if (!findchr(s[i], "0123456789")) //a custom function that checks a char against a string and returns zero if the char does not match any of the string's elements
		{
			write(2, "Error\nInvalid argument\n", 23); //in our case, if anything in our args is not a number, return this error
			free(philo);
			return (1);
		}
	}
	return (0);
}

int	chkdeath(t_philo *ph, int i)
{
	unsigned long long	ctime;
	unsigned long long	d;

	ctime = gettime();
	d = (unsigned long long)ph->die;
	while (i < ph->philnum)
	//the loop will check each philo for death everytime this function is called
	{
		if (ph->dead)
			return (1);
		usleep(100);
		pthread_mutex_lock(&ph->eatstart); //remember, a philo can't die while eating, so no deathchecks during that time
		if (ph->timedie[i] + d < ctime) //this math here indicates that equal or more ms since time_to_die passed since the philosopher last ate (or from the start of program)
		{
			ph->dead = 1;
			pthread_mutex_lock(&ph->write);
			printf("%lld %d died\n", ctime, i + 1);
			pthread_mutex_unlock(&ph->write);
			pthread_mutex_unlock(&ph->eatstart); //even if some1 died, this should be unlocked, so the other threads can continue running untill they hit the exit "if"
			return (1);
		}
		i++;
		pthread_mutex_unlock(&ph->eatstart);//the death check may resume now
	}
	return (0);
}

static void	initialise(char **argv, t_philo *philo)
{
	int	i;

	i = 0;
	philo->die = ft_atoi(argv[2]); //the time to die in ms
	philo->eat = ft_atoi(argv[3]); //the time to eat in ms
	philo->sleep = ft_atoi(argv[4]); //the time to sleep in ms
	philo->philnum = ft_atoi(argv[1]); //the number of philosophers
	philo->dead = 0;	//a true/false index. a "1" value indicates that a philosopher has died and signals all threads to return to the main
	philo->allphsate = 0; // a true/false index. a "1" value indicates that all philosophers have eaten at least argv[5] times and signals all threads to return to the main
	if (argv[5])   //check if the 5th condition exists. If so assign it, else set it to -1 to let the threads skip this check
		philo->maxeat = ft_atoi(argv[5]);
	else
		philo->maxeat = -1;
	philo->g_philnum = 1; //a variable used to assign a number to each thread. Its value is then passed to a local variable inside each thread before being incremented to be used to the next one.
	while (i < philo->philnum)
		philo->timeseat[i++] = 0; //this array stores how many times each philosopher has eaten. We initialise the 1st philnum elements of it to 0. Could use memset for this, but guess who's not reading the subject carefully
}

int	main(int argc, char **argv)
{
	int		i;					//an index to count our loops
	t_philo	*philo;				//always declare zour struct as a pointer, saves you a lot of trouble

	i = 0;
	philo = malloc(sizeof(t_philo));
	if (!philo || (argc != 5 && argc != 6))  //as this programs can take only 4 or 5 arguments, any other input is invalid and in this case, the programm should free its resources and exit
	{
		errorprint(argc, philo);
		return (0);
	}
	while (argv[++i]) //this loop calls a function that checks for invalid arguments
		if (wrongarg(argv[i], philo))
			return (0);
	initialise(argv, philo); //initialise all the elements in our struct after the error checks come clean
	if (philo->philnum == 1)
	{
		onephilo(philo); //hard code the case of only 1 philosopher. To be absolutely following the paper, you should initialise a thread to do the job, but i'm lazy and convincing, so I did not
		return (0);
	}
	forkcreat(argv[1], philo); //create our forks. This function uses an array of mutexes in our struct and initialises the 1st philnum elements in it using the pthread_mutex_init function in a loop.
	philocreat(argv[1], philo); //create philnum number of threads(philosophers) using pthread_create in a loop with an array of thread pointers
	eatchecker(philo); //we assign to the main thread the task to count how many times each philo ate, in case we have to
	free (philo); //last, but not least, free our struct for a clean quit
	return (0);
}
