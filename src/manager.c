#include "../incl/defined.h"
#include "../incl/hermes.h"

#ifndef WORKER
#define WORKER(w) ((t_worker*)w->data)
#endif

int				connect_workers(t_node **workers, size_t *worker_count,
		t_node **rm_tree, int proto)
{
	t_worker		*worker;

	if (!*workers)
		return (0);
	if ((*workers)->left)
		connect_workers(&(*workers)->left, worker_count, rm_tree, proto);
	worker = (*workers)->data;
	if ((worker->sock = socket(PF_INET, SOCK_STREAM, proto)) == -1)
		hermes_error(errno, TRUE, 2, "socket()", strerror(errno));
	if (connect(worker->sock, (const struct sockaddr *)&worker->sin,
				sizeof(worker->sin)) == -1)
	{
		hermes_error(FAILURE, FALSE, 2, "error connecting to worker", inet_ntoa(worker->sin.sin_addr));
		remove_node(rm_tree, worker, worker_cmp, worker_min);
		*worker_count -= 1;
	}
	if ((*workers)->right)
		connect_workers(&(*workers)->right, worker_count, rm_tree, proto);
}

void	divide_work(t_node *tree, t_node **worktree, cmp, int count)
{
	if (count > 0)
	{
		if (tree) {
			divide_work(tree->left, worktree, cmp);
			add_node(worktree, tree->data, cmp);
			divide_work(tree->right, worktree, cmp);
		}
	}
}

int					manager(t_job *job)
{
	struct protoent	*proto;

	if ((proto = getprotobyname("tcp")) == 0)
		return (FAILURE);
	if (job->worker_list.worker_count > 0)
	{
		connect_workers(&job->worker_list.workers, &job->worker_list.worker_count,
						&job->worker_list.workers, proto->p_proto);
		printf("connected to %zu workers.\n", job->worker_list.worker_count);
		/*TODO divide work, and distribute to workers*/
	}
	/* Divide work amongst thread count, send jobs to workersspawn threads*/
	// count = job->whatever->count / threads;
	// while (threads > 0)
	// {
	//     divide_work(job->whatever->nodes, t_node &worktree, cmp_func, count);
	//	   threads--;
	// }
	// TODO : do things
	return (0);
}

#ifdef TESTING
void    prompt(char *output, char *input, int buflen)
{
	printf("%s", output);
	fgets(input, buflen, stdin);
}

static int	skip(int size, const char *s)
{
	while (s[size] == ' ' || s[size] == '\n' ||
			s[size] == '\t' || s[size] == '\0')
		size--;
	return (size);
}

char		*strtrim(const char *s)
{
	int		size;
	char	*ret;

	if (!s)
		return (0);
	while (*s == ' ' || *s == '\n' || *s == '\t' || *s == '\0' || *s == '\r')
	{
		if (s == 0 || *s == 0)
		{
			if (!(ret = (char*)memalloc(sizeof(char))))
				return (0);
			*ret = 0;
			return (ret);
		}
		s++;
	}
	size = (strlen(s));
	size = skip(size, s);
	size++;
	if (!(ret = (char*)malloc(sizeof(char) * (size + 1))))
		return (0);
	strncpy(ret, s, size);
	ret[size] = 0;
	return (ret);
	free(ret);
}

int main(void)
{
	uint32_t    ip;
	t_job       *job;
	t_node      *worker;
	char        input[20];

	ip = NULL;
	job = (t_job*)memalloc(sizeof(t_job));
	job->worker_list = *(t_workerlist*)memalloc(sizeof(t_workerlist));
	while (TRUE)
	{
		prompt("> ", input, 20);
		if (!memcmp("connect", input, 7))
		{
			manager(job);
		}
		else if (!memcmp("add", input, 3))
		{
			worker = new_node();
			worker->data = new_worker();
			prompt("ip > ", input, 20);
			if (parse_ip(&WORKER(worker)->sin.sin_addr.s_addr, strtrim(input)) < 0)
				hermes_error(INPUT_ERROR, TRUE, 2, "parsing ip", strerror(errno));
			prompt("port > ", input, 20);
			if (parse_port(&WORKER(worker)->sin.sin_port, strtrim(input)) < 0)
				hermes_error(INPUT_ERROR, TRUE, 2, "parsing port", strerror(errno));
			if (add_node(&(job->worker_list.workers), &worker, worker_cmp) < 0)
				hermes_error(INPUT_ERROR, TRUE, 2, "adding worker", strerror(errno));
			job->worker_list.worker_count++;
		}
		else if (!memcmp("del", input, 3))
		{
			prompt("ip > ", input, 20);
			if (parse_ip(&ip, strtrim(input)) < 0)
				hermes_error(INPUT_ERROR, TRUE, 2, "parsing ip", strerror(errno));
			if (remove_node(&job->worker_list.workers, ip, worker_cmp, worker_min) < 0)
				hermes_error(INPUT_ERROR, TRUE, 2, "removing worker", strerror(errno));
			job->worker_list.worker_count--;
		}
		else if (!memcmp("quit", input, 4) ||
				!memcmp("exit", input, 4))
		{
			break;
		}
		fflush(stdin);
	}
}
#endif
