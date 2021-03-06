#ifndef HERMES_DEFINES
# define HERMES_DEFINES

/*
**   All of these default settings for timeing and rate are borrowed from
**   the original nmap project written by Gordon "Fyodor" Lyon.
**   Im positive that these defaults are product of years of research
**   and trial and error.
*/

# define TTL_MAX 256
# define MTU_MAX 1500
# define BUFF_SIZE 1028
# define PORT_MAX (65535)
# define MAX_PKT_DATA (65535 - 20 - 20)
# define MAX_THREADS (256)
# define MAX_HOSTGROUP (4096)
# define MIN_HOSTGROUP (1)

# ifndef fast_swap_ints
# define fast_swap_ints(x, y) ((x ^= y),(y ^= x),(x ^= y))
# endif

# define DEF_TCP_PROBE_PORT (80)
# define DEF_SCTP_PROBE_PORT (80)
# define DEF_UDP_PROBE_PORT (40125)

/*
**   We wait at least 100 ms for a response by default - while that
**   seems aggressive, waiting too long can cause us to fail to detect
**   drops until many probes later on extremely low-latency
**   networks (such as localhost scans).
*/
# define DEF_MAX_RETRIES (10)
# define DEF_INIT_SCAN_DELAY (0)
# define DEF_SCAN_DELAY (1000)
# define DEF_MIN_RTT_TIMEOUT (100) /* ms */
# define DEF_INIT_RTT_TIMEOUT (1000) /* ms */
# define DEF_MAX_RTT_TIMEOUT (10000) /* ms */


#endif