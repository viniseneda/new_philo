/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvarussa <vvarussa@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 13:49:27 by vvarussa          #+#    #+#             */
/*   Updated: 2022/04/20 23:32:00 by vvarussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	return ((c > 47) && (c < 58));
}

static int	is_white(int character)
{
	return ((character == '\n') | (character == '\t')
		| (character == '\v') | (character == '\r')
		| (character == '\f') | (character == ' '));
}

static int	gen_num(const char *temp, int p, int sum)
{
	while (ft_isdigit(*temp) != 0)
	{
		if ((*(temp + 1) == '\0') | !ft_isdigit(*(temp + 1)))
		{
			sum = sum + (*temp - '0');
			return (sum * p);
		}
		else
			sum = (sum + (*temp - '0')) * 10;
		temp++;
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int			p;
	int			sum;
	const char	*temp;

	p = 1;
	sum = 0;
	temp = nptr;
	while (is_white(*temp) != 0)
		temp++;
	if ((*temp == '+') | (*temp == '-'))
	{
		if (*temp == '-')
			p = -1;
		temp++;
	}
	return (gen_num(temp, p, sum));
}
