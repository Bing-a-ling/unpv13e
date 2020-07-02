#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	// struct sockaddr_in
	// {
	// 	sa_family_t      sin_family; //地址族
	// 	uint64_t         sin_port;  // 16位TCP/UDP端口号
	// 	struct  in_addr  sin_addr;  //struct in_addr {In_addr_t s_addr}; // 32位IPV4地址
	// 	char             sin_zero[8]; //不使用
	// }
	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");
	// stdin,stdout,stderr: 0,1,2
	// INVALID_SOCKET: -1
	// other files: > 2
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");
	// void bzero(void *s, int n)
	// 将字符串s的前几个字节置零，n通常取sizeof(s)
	// 已经被标记为遗留函数不推荐使用
	// 可改用void* memset(void *s, int ch, size_t n);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// Network Byte Order(NBO)
	// sin_port和sin_addr都必须是NBO
	// 一般可视化的数字都是HBO
	servaddr.sin_port   = htons(9999);	/* daytime server */
	// inet_pton 将点分十进制转化为二进制整数，地址无效返回0，协议簇不合法返回-1
	// 	int inet_pton(int af, const char *src, void *dst);
	// inet_ntop 将二进制整数转化为点分十进制
	// 	const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);
	// int connect (int sockfd, struct sockaddr * serv_addr, int serv_addr_len);
	// 作为参数时将sockaddr_in类型指针转换为sockaddr类型指针传入
	// 因为历史原因（sockaddr是更早的标准）
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");
	int count = 0;
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		count++;
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	printf("count:%d\n", count);
	if (n < 0)
		err_sys("read error");

	exit(0);
}
