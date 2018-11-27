/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve_quartic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 13:44:20 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/27 19:33:06 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersection.h"

static int  init_quartic(t_quartic *q, double c[5])
{
    q->a = c[3] / c[4];
    q->b = c[2] / c[4];
    q->c = c[1] / c[4];
    q->d = c[0] / c[4];
    q->sq_a = q->a * q->a;
    q->p = -3.0 / 8.0 * q->sq_a + q->b;
    q->q = 1.0 / 8.0 * q->sq_a * q->a - 1.0 / 2.0 * q->a * q->b + q->c;
    q->r = -3.0 / 256.0 * q->sq_a * q->sq_a + 1.0 / 16.0 * q->sq_a * q->b \
        - 1.0 / 4.0 * q->a * q->c + q->d;
    q->i = -1;
    return (1);
}

static void root_zero(t_quartic *q, double t[4])
{
    q->coeffs[0] = q->q;
    q->coeffs[1] = q->p;
    q->coeffs[2] = 0.0;
    q->coeffs[3] = 1.0;
    q->roots = solve_cubic(q->coeffs, t);
    t[q->roots++] = 0;//
}

static void root_non_zero(t_quartic *q, double t[4])
{
    q->coeffs[0] = 1.0 / 2.0 * q->r * q->p - 1.0 / 8.0 * q->q * q->q;
	q->coeffs[1] = -q->r;
	q->coeffs[2] = -1.0 / 2.0 * q->p;
	q->coeffs[3] = 1.0;
	solve_cubic(q->coeffs, t);
	q->z = t[0];
	q->u = q->z * q->z - q->r;
	q->v = 2.0 * q->z - q->p;
}

static void solve_resolvant_quartic(t_quartic *q, double t[4])
{
    q->coeffs[0] = q->z - q->u;
	q->coeffs[1] = (q->q < 0) ? -q->v : q->v;
	q->coeffs[2] = 1.0;
	q->roots = solve_quadratic(q->coeffs, t);
	q->coeffs[0] = q->z + q->u;
	q->coeffs[1] = (q->q < 0) ? q->v : -q->v;
	q->coeffs[2] = 1.0;
	q->roots += solve_quadratic(q->coeffs, t + q->roots);
}

int         solve_quartic(double c[5], double t[4])
{
    t_quartic   q;

    if (init_quartic(&q, c) && ISZERO(q.r))
        root_zero(&q, t);
    else
    {
        root_non_zero(&q, t);
        if (ISZERO(q.u))
            q.u = 0;
        else if (q.u > 0)
            q.u = sqrt(q.u);
        else
            return (0);
        if (ISZERO(q.v))
            q.v = 0;
        else if (q.v > 0)
            q.v = sqrt(q.v);
        else
            return (0);
        solve_resolvant_quartic(&q, t);
    }
    q.sub = 1.0 / 4.0 * q.a;
    while (++q.i < q.roots)
        t[q.i] -= q.sub;
    return (q.roots);
}
