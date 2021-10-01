/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:33:50 by edavid            #+#    #+#             */
/*   Updated: 2021/10/01 12:14:26 by edavid           ###   ########.fr       */
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
# define CANT_STOP_EATING -1
# define INIT_TIME_IN_MICROSECONDS 100000
// debugging
// # define PRINT_HERE() (printf("Line: %d, file: %s\n", __LINE__, __FILE__))

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

// 'ateTimestamp' is in milliseconds
typedef struct s_philo_eat_info
{
	int			philosopher_index;
	long int	ateTimestamp;
}	t_philo_eat_info;

typedef struct s_philo_fork
{
	pthread_mutex_t	fork;
}	t_philo_fork;

// time_to_die, time_to_eat and time_to_sleep should be converted to microsecs
typedef struct s_philosopher_info
{
	int						phNum;
	long int				time_to_die;
	long int				time_to_eat;
	long int				time_to_sleep;
	int						nOfMeals;
	t_philo_fork			*refLeftFork;
	t_philo_fork			*refRightFork;
	struct timeval			*refStartTime;
	long int				ateTimestamp;
	long int				lastThinkTimeStamp;
	pthread_mutex_t			*ref_queue;
	t_node_binary			**meal_queue;
	pthread_mutex_t			*refStartMutex;
}	t_philosopher_info;

// time_to_die, time_to_eat and time_to_sleep are in milliseconds
typedef struct s_philosophers
{
	int					phNum;
	t_philosopher_info	*ph_arr;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nOfMeals;
	t_philo_fork		*forks;
	struct timeval		start_time;
	pthread_mutex_t		*start_mutexes;
	t_node_binary		*meal_queue;
	t_node_binary		*first_in_queue;
	pthread_mutex_t		time_left_lst_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		game_over_mutex;
	bool				game_over;
}	t_philosophers;

typedef struct s_threads
{
	pthread_t		reaper_thread;
	pthread_t		*phThreads;
	int				phNum;
	pthread_mutex_t	detach_mutex;
}	t_threads;

// Initializing functions

int					philo_init_mystruct(t_philosophers *mystruct, int argc,
						char **argv);
int					philo_destroy_mystruct(t_philosophers *mystruct);

// Thread functions

int					philo_init_threads(t_philosophers *mystruct,
						t_threads *threads);
int					philo_join_threads(t_philosophers *mystruct,
						t_threads *threads);
int					philo_detach_threads(t_philosophers *mystruct,
						t_threads *threads);

// Routines

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
t_philo_eat_info	*philo_new_philo_info(int philo_index, long int timestamp);
t_philosophers		*philo_get_mystruct(t_philosophers *mystruct);
void				philo_enqueue(t_philosopher_info *pinfo,
						long int timestamp);
void				philo_unlock_all_forks(t_philosophers *mystruct);
long int			long_int_max_of(long int a, long int b);
long int			philo_get_current_timestamp(void);
void				philo_sleep_until_timestamp(long int timestamp);
int					ft_strcmp(const char *s1, const char *s2);
bool				has_philo_died(t_philosophers *mystruct,
						t_node_binary *cur);
bool				try_left_fork(t_philosopher_info *pinfo);
bool				try_right_fork(t_philosopher_info *pinfo,
						long int timestamp);
bool				try_eating(t_philosopher_info *pinfo, long int timestamp);
bool				is_game_over2(t_philosophers *mystruct);

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
bool				ft_fifolst_is_empty(t_node_binary *lst);

// print functions

int					philo_print_status(int phNum,
						enum e_philosopher_status status, long int timestamp);

#endif
