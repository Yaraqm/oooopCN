#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>  // Add this line at the top

int main(int argc, char *argv[]) {
    struct sockaddr_in fsin;  /* Client address */
    char buf[100];            /* Input buffer */
    char *pts;                /* Pointer to time string */
    int s;                    /* Server socket */
    time_t now;               /* Current time */
    socklen_t alen;           /* Address length */
    struct sockaddr_in sin;   /* Server address */
    int port = 3000;          /* Default port */

    /* Handle command-line arguments */
    if (argc == 2) {
        port = atoi(argv[1]);
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    /* Create a UDP socket */
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket creation failed");
        exit(1);
    }

    /* Configure server address */
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    /* Bind the socket */
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind failed");
        exit(1);
    }

    printf("UDP Time Server started on port %d...\n", port);

    alen = sizeof(fsin);
    while (1) {
        /* Receive message */
        if (recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&fsin, &alen) < 0) {
            perror("recvfrom error");
            continue;
        }

        /* Get current time */
        time(&now);
        pts = ctime(&now);

        /* Send response */
        if (sendto(s, pts, strlen(pts), 0, (struct sockaddr *)&fsin, alen) < 0) {
            perror("sendto error");
        }
    }

    close(s);
    return 0;
}
