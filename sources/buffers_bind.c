/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers_bind.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 21:06:52 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 21:48:41 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static unsigned int	check_framebuffer_status(const char *fbo_type)
{
	GLenum	status;
	char	*err_msg;

	err_msg = ft_strjoin(fbo_type, FRAMEBUFFER_INCOMPLETE_ERROR);
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		free(err_msg);
		return (1);
	}
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		log_error_free(ft_strjoin_lf(err_msg, " (incomplete attachment)"));
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
		log_error_free(ft_strjoin_lf(err_msg, " (missing attachment)"));
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
		log_error_free(ft_strjoin_lf(err_msg, " (samples do not match)"));
	else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
		log_error_free(ft_strjoin_lf(err_msg, " (unsupported format)"));
	else
		log_error_free(err_msg);
	return (0);
}

unsigned int		generate_framebuffers(t_gl_buffers *buffers, \
	unsigned int win_w, unsigned int win_h)
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffers->ms_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, buffers->rbo[0]);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB8, win_w, win_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, \
			GL_RENDERBUFFER, buffers->rbo[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, buffers->rbo[1]);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH_COMPONENT, \
			win_w, win_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, \
			GL_RENDERBUFFER, buffers->rbo[1]);
	if (check_framebuffer_status("multisample ") == 0)
		return (0);
	glBindFramebuffer(GL_FRAMEBUFFER, buffers->pick_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, buffers->rbo[2]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, win_w, win_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, \
			GL_RENDERBUFFER, buffers->rbo[2]);
	glBindRenderbuffer(GL_RENDERBUFFER, buffers->rbo[3]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, win_w, win_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, \
			GL_RENDERBUFFER, buffers->rbo[3]);
	return (check_framebuffer_status("color pick "));
}
