#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// servaddr.sin_port        = htons(13);	/* daytime server */
	servaddr.sin_port        = htons(9999);	/* daytime server */
	// 在一般情况下，对于服务器进程问题需要调用bind()函数，对于客户进程则不需要调用bind()函数。
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		connfd = Accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);
		// int snprintf(char *str, size_t size, const char *format, ...)
		// 将可变参数 “…” 按照format的格式格式化为字符串，然后再将其拷贝至str中。
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		// 如果改为按字节写入 那么客户端将会多次收到read
		// 每次收到的长度不确定
		// int i = 0;
		// for(; i < strlen(buff); i++){
		// 	Write(connfd, buff+i, 1);
		// }
        Write(connfd, buff, strlen(buff));
		Close(connfd);
	}
}