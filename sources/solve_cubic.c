/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve_cubic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 14:50:33 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/27 19:34:04 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersection.h"
#include <stdio.h>//

static void init_cubic(t_cubic *cub, double c[4])
{
	cub->a = c[2] / c[3];
	cub->b = c[1] / c[3];
	cub->c = c[0] / c[3];
	cub->sq_a = cub->a * cub->a;
	cub->p = 1.0 / 3.0 * (-1.0 / 3.0 * cub->sq_a + cub->b);
	cub->q = 1.0 / 2.0 * (2.0 / 27.0 * cub->a * cub->sq_a - 1.0 / 3.0
	    * cub->a * cub->b + cub->c);
	cub->cb_p = cub->p * cub->p * cub->p;
	cub->d = cub->q * cub->q + cub->cb_p;
	//cub->i = -1;
}

static void zero_d(t_cubic *cub, double t[3])
{
	double	u;

	if (ISZERO(cub->q))
	{
		t[0] = 0;
		cub->roots = 1;
	}
	else
	{
		u = CBRT(-cub->q);
		t[0] = 2.0 * u;
		t[1] = -u;
		cub->roots = 2;
	}
}

static void negative_d(t_cubic *cub, double t[3])
{
	double	phi;
	double	p;

	//printf("-cub->cb_p sign : %f\n-cub->p sign : %f\n", -cub->cb_p, -cub->p);
	phi = 1.0 / 3.0 * acos(-cub->q / sqrt(-cub->cb_p));
	p = 2.0 * sqrt(-cub->p);
	t[0] = p * cos(phi);
	t[1] = -p * cos(phi + M_PI / 3.0);
	t[2] = -p * cos(phi - M_PI / 3.0);
	cub->roots = 3;
}

static void positive_d(t_cubic *cub, double t[3])
{
	double	sqrt_d;
	double	u;
	double	v;

	sqrt_d = sqrt(cub->d);
	u = CBRT(sqrt_d - cub->q);
	v = -CBRT(sqrt_d + cub->q);
	t[0] = u + v;
	cub->roots = 1;
}

int         solve_cubic(double c[4], double t[3])
{
	t_cubic cub;

	if (ISZERO(c[3]))
		return (solve_quadratic(c, t));
	init_cubic(&cub, c);
	if (ISZERO(cub.d))
		zero_d(&cub, t);
	else if (cub.d < 0)
		negative_d(&cub, t);
	else
		positive_d(&cub, t);
	cub.sub = 1.0 / 3.0 * cub.a;
	cub.i = -1;
	while (++cub.i < cub.roots)
		t[cub.i] -= cub.sub;
	return (cub.roots);
}
