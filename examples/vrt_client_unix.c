#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <inttypes.h>

#include "vrt.h"
#include "base64.h"

#define RECV_BUFFER_LEN 1024

// https://github.com/cloudflare/roughtime/blob/master/ecosystem.json

#define CHECK(x)                                                               \
  do {                                                                         \
    int ret;                                                                   \
    if ((ret = x) != VRT_SUCCESS) {                                            \
      return (ret);                                                            \
    }                                                                          \
  } while (0)

int prepare_socket(void)
{
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  assert(sockfd >= 0);
  return sockfd;
}

void prepare_servaddr(struct sockaddr_in *servaddr, const char *host, unsigned port)
{
  struct hostent *he;
  he = gethostbyname(host);
  assert(he != NULL);

  bzero((char *)servaddr, sizeof(*servaddr));

  char **ip_addr;
  memcpy(&ip_addr, &(he->h_addr_list[0]), sizeof(void *));
  memcpy(&servaddr->sin_addr.s_addr, ip_addr, sizeof(struct in_addr));

  servaddr->sin_family = AF_INET;
  servaddr->sin_port = htons(port);
}

int main(int argc, char **argv) {
  uint32_t recv_buffer[RECV_BUFFER_LEN / 4] = {0};
  uint8_t query[VRT_QUERY_PACKET_LEN] = {0};
  struct sockaddr_in servaddr;
  const char *host;
  int port;
  uint8_t *public_key;
  int public_key_len;

  if (argc != 4) {
    fprintf(stderr, "Usage: client host port public_key\n");
    exit(1);
  }
  host = argv[1];
  port = atoi(argv[2]);
  public_key = unbase64(argv[3], strlen(argv[3]), &public_key_len);
  if (public_key == NULL || public_key_len != 32) {
    fprintf(stderr, "invalid public key\n");
    exit(1);
  }
  
  int sockfd = prepare_socket();
  prepare_servaddr(&servaddr, host, port);

  /* prepare query */
  uint8_t nonce[VRT_NONCE_SIZE] = "preferably a random byte buffer";
  CHECK(vrt_make_query(nonce, 64, query, sizeof query));

  /* send query */
  int n = sendto(sockfd, (const char *)query, sizeof query, 0,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));

  /* receive packet */
  assert(n==sizeof query);
  do {
    n = recv(sockfd, recv_buffer, (sizeof recv_buffer) * sizeof recv_buffer[0], 0 /* flags */);
  } while (n == -1 && errno == EINTR);

  /* parse response */
  uint64_t out_midpoint;
  uint32_t out_radii;

  CHECK(vrt_parse_response(nonce, 64, recv_buffer,
                            sizeof recv_buffer * sizeof recv_buffer[0],
                            //public_key_google, &out_midpoint,
                            public_key, &out_midpoint,
                            &out_radii));
  printf("midp %" PRIu64 " radi %u\n", out_midpoint, out_radii);
  close(sockfd);

  free(public_key);
  
  return 0;
}
