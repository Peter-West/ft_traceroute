#include "traceroute.h"

void						ft_error(char *msg)
{
	printf("%s\n", msg);
	exit(-1);
}

struct addrinfo				*addr_infos(char **argv)
{
	static struct addrinfo	*res;
	struct addrinfo			hints;

	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = 0;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;
	getaddrinfo(argv[1], NULL, &hints, &res);
	perror("addr_infos");
	printf("res addr : %s\n", res->ai_addr->sa_data);
	return (res);
}

uint16_t in_ping_cksum(uint16_t *addr, int len)
{
	int nleft = len;
	uint16_t *w = addr;
	uint16_t answer;
	int sum = 0;
	uint16_t u = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) 
	{
		*(uint8_t *)(&u) = *(uint8_t *)w ;
		sum += u;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

void						init_sock(t_env *e)
{
	int						ttl = 64;

	if ((e->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		perror("socket");
	if (setsockopt(e->sockfd, IPPROTO_IP, IP_TTL, (void*)&ttl, sizeof(ttl)) < 0)
		perror("setsockopt");
}

void	init_req(t_env *e)
{
	static t_icmp_req	icmpreq;
	static int			id = 1;
	static int			seq = 1;
	int					i = 0;
	int					j = 0;
	struct timeval		tv;

	icmpreq.imcp_hdr.type = 8;
	icmpreq.imcp_hdr.code = 0;
	icmpreq.imcp_hdr.chksm = 0;
	icmpreq.imcp_hdr.id = id;
	icmpreq.imcp_hdr.seqnum = seq++;
	if (gettimeofday(&tv, NULL) == 0)
		icmpreq.tv = tv;
	ft_bzero(icmpreq.data, REQ_DATASIZE);
	while (i < REQ_DATASIZE) {
		if (('a' + j) > 'z')
			j = 0;
		icmpreq.data[i] = 'a' + j;
		i++;
		j++;
	}	
	icmpreq.imcp_hdr.chksm = in_ping_cksum((uint16_t *)&icmpreq, sizeof(t_icmp_req));
	e->icmpreq = &icmpreq;
}

void	send_req(t_env *e)
{	
	ssize_t	ret;

	ret = sendto(e->sockfd, (void*)e->icmpreq, sizeof(t_icmp_req), 0, e->infos->ai_addr, e->infos->ai_addrlen);
	if (ret < 0)
		perror("sendto");
}

int							main(int argc, char **argv)
{
	t_env					e;
	if (argc < 2)
		ft_error("Version 0.1");
	e.infos = addr_infos(argv);
	init_sock(&e);
	return (0);
}