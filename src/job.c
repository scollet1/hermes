# include "job.h"

t_job		*new_job(void)
{
	t_job	*job;

	if (!(job = memalloc(sizeof(t_job))))
		hermes_error(errno, TRUE, 2, "malloc()", strerror(errno));
	return (job);
}