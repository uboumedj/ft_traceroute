/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uboumedj <uboumedj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 02:11:13 by uboumedj          #+#    #+#             */
/*   Updated: 2018/01/10 15:46:10 by uboumedj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

int		ft_nbrlen(long long int nbr)
{
	int res;

	res = 0;
	while (nbr != 0)
	{
		nbr = nbr / 10;
		res++;
	}
	return (res);
}