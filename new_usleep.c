/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_usleep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 18:26:47 by pleveque          #+#    #+#             */
/*   Updated: 2022/02/13 18:38:22 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    new_usleep(int ms)
{
	usleep(ms * MS);
}
