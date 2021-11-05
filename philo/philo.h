/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchatzip <mchatzip@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 14:39:46 by gerlingi          #+#    #+#             */
/*   Updated: 2021/10/28 16:57:40 by mchatzip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t			philos[1000];
	pthread_t			calc;
	pthread_mutex_t		forks[1000];
	pthread_mutex_t		eatstart;
	int					timeseat[1000];
	unsigned long long	timedie[1000];
	pthread_mutex_t		forkt;
	pthread_mutex_t		write;
	int					philnum;
	int					eat;
	int					sleep;
	int					die;
	int					maxeat;
	int					dead;
	int					allphsate;
	unsigned long long	time;
	int					g_philnum;
}t_philo;

int					findchr(const char c, const char *str);
int					ft_atoi(const char *str);
void				*testf(void *id);
void				philocreat(char *s, t_philo *philo);
void				forkcreat(char *s, t_philo *philo);
unsigned long long	gettime(void);
void				eatsequence(t_philo *ph, int i);
void				lockforks(t_philo *ph, int i);
int					checkeat(int *s, int i, int len);
int					allate(t_philo *ph);
int					chkdeath(t_philo *ph, int i);
void				*routine(void *id);
void				eatchecker(t_philo *philo);
void				print(char *s, int i, t_philo *ph);
void				onephilo(t_philo *philo);
void				errorprint(int argc, t_philo *philo);

#endif