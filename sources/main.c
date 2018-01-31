/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 15:10:59 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:23:44 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

int			main(void)
{
	internal_context_initialize();
	if (!ft_init_system())
		return (1);
	return (0);
}
