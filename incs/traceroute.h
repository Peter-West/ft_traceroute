#ifndef TRACEROUTE_H
# define TRACEROUTE_H
# define REQ_DATASIZE	64

# include "libft.h"
# include <sys/socket.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <netdb.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <sys/select.h>

typedef struct	s_icmp_hdr
{
	uint8_t		type;
	uint8_t		code;
	uint16_t	chksm;
	uint16_t	id;
	uint16_t	seqnum;
}				t_icmp_hdr;

typedef struct	s_icmp_req
{
	t_icmp_hdr	imcp_hdr;
	struct timeval	tv;
	char		data[REQ_DATASIZE];
}				t_icmp_req;

typedef struct			s_env
{
	struct addrinfo		*infos;
	t_icmp_req			*icmpreq;
	int					sockfd;
}						t_env;

#endif