/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:33:50 by edavid            #+#    #+#             */
/*   Updated: 2021/09/30 19:19:40 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include <stdbool.h>
# include <semaphore.h>
# include <signal.h>
# define CANT_STOP_EATING -1
# define INIT_TIME_IN_MICROSECONDS 200000
# define SEM_FORKS "/forks"
# define SEM_FINISH "/finish"
# define SEM_QUEUE "/queue"
# define SEM_DONE_EATING "/doneeating"
# define SEM_PRINT "/print"
# define SEM_FINISHED_EATING "/finishedeating"
// debugging
# define PRINT_HERE() (printf("Line: %d, file: %s\n", __LINE__, __FILE__))
// # define PRINT_PHILO() (printf("Line: %d, file: %s, philonum: %d\n",
// __LINE__, __FILE__, pinfo->phNum))

enum e_philosopher_status
{
	PHEAT,
	PHSL,
	PHTH,
	PHILO_FORK,
	PHILO_DIED
};

typedef struct s_node_binary
{
	void					*content;
	struct s_node_binary	*next;
	struct s_node_binary	*prev;
}	t_node_binary;

// time_to_die, time_to_eat and time_to_sleep should be converted to microsecs
typedef struct s_philosopher_info
{
	int						phNum;
	long int				time_to_die;
	long int				time_to_eat;
	long int				time_to_sleep;
	long int				time_to_think;
	int						nOfMeals;
	long int				startTime;
	long int				ateTimestamp;
	long int				lastThinkTimeStamp;
	t_node_binary			*meal_queue;
	pthread_t				reaper;
	sem_t					*semQueue;
	char					*semQueueName;
	sem_t					*semDoneEating;
	char					*semDoneEatingName;
	bool					hasDoneEating;
	t_node_binary			*first_in_queue;
}	t_philosopher_info;

// time_to_die, time_to_eat and time_to_sleep are in milliseconds
typedef struct s_philosophers
{
	int					phNum;
	t_philosopher_info	*ph_arr;
	pid_t				*process_ids;
	sem_t				*forks;
	sem_t				*semFinish;
	sem_t				**semQueue;
	char				**semQueueNames;
	sem_t				**semDoneEating;
	char				**semDoneEatingNames;
	sem_t				*semPrint;
	sem_t				*semFinishedEating;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nOfMeals;
	long int			startTime;
	pthread_t			endCondThread;
	bool				everyoneHasEaten;
}	t_philosophers;

// Initializing functions

int					philo_init_mystruct(t_philosophers *mystruct, int argc,
						char **argv);
int					philo_destroy_mystruct(t_philosophers *mystruct);
int					philo_kill_processes(t_philosophers *mystruct);

// Routines

int					philo_process(t_philosophers *mystruct,
						t_philosopher_info *pinfo);
void				*philo_routine(void *info);
void				*reaper_routine(void *mystructPtr);
void				*start_routine(void *mystructPtr);

// Utils functions

int					philo_my_free(void **pointer);
int					philo_atoi(char *str);
bool				philo_is_valid_positive_number(char *str);
long int			philo_calc_microseconds_difference(struct timeval *current,
						struct timeval *previous);
long int			philo_add_timeval(struct timeval *t1, struct timeval *t2);
long int			philo_timval_to_microseconds(struct timeval *t);
void				*philo_calloc(int number_of_elements,
						int size_of_each_element);
int					philo_try_calloc(void **ret, int n_of_els, int size_of_el);
int					philo_try_malloc(void **ret, int size);
t_philosophers		*philo_get_mystruct(t_philosophers *mystruct);
void				philo_enqueue(t_philosopher_info *pinfo,
						long int timestamp);
void				philo_unlock_all_forks(t_philosophers *mystruct);
long int			long_int_max_of(long int a, long int b);
long int			philo_get_current_timestamp(void);
void				philo_sleep_until_timestamp(long int timestamp);
int					ft_strcmp(const char *s1, const char *s2);
bool				try_left_fork(t_philosopher_info *pinfo);
bool				try_right_fork(t_philosopher_info *pinfo,
						long int timestamp);
bool				try_eating(t_philosopher_info *pinfo, long int timestamp);
bool				is_game_over2(t_philosophers *mystruct);
sem_t				*philo_sem_init(char *name, unsigned int value);
int					philo_init_processes(t_philosophers *mystruct);
void				philo_unlock_all_forks(t_philosophers *mystruct);
int					ft_strlen(const char *s);
char				*ft_strdup(const char *s);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
char				*ft_strjoin_free(char *s1, char *s2);
int					ft_intlen(int d);
char				*ft_itoa(int n);
long int			*ft_lintdup(long int li);

// Philosopher actions
void				philo_fork(t_philosophers *mystruct,
						t_philosopher_info *pinfo);
void				philo_eat(t_philosophers *mystruct,
						t_philosopher_info *pinfo, long int timestamp);
void				philo_sleep(t_philosopher_info *pinfo, long int timestamp);
void				philo_think(t_philosopher_info *pinfo, long int timestamp);

// t_node_binary functions

t_node_binary		*ft_nodbinnew(void *content);
void				ft_nodbinadd_front(t_node_binary **lst, t_node_binary *new);
void				ft_nodbinclear(t_node_binary **lst, void (*del)(void *),
						int n);
void				ft_nodbindelone(t_node_binary *lst, void (*del)(void *));
void				ft_nodbindel(void *node);

// queue ADT functions

void				ft_fifonodbinenqueue(t_node_binary **lst,
						t_node_binary *new);
t_node_binary		*ft_fifonodbindequeue(t_node_binary **lst,
						t_node_binary *nullable_last_node);

// print functions

int					philo_print_status(int phNum,
						enum e_philosopher_status status, long int timestamp);

#endif
