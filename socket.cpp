
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "socket.h"

int ConexaoRawSocket(char *device)
{
    int sockfd;
    struct ifreq ir;
    struct sockaddr_ll endereco;
    struct packet_mreq mr;


    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  	/*cria socket*/
    if (sockfd == -1) {
      perror("Erro no Socket: ");
      exit(-1);
    }

    memset(&ir, 0, sizeof(struct ifreq));  	/*dispositivo eth0*/
    memcpy(ir.ifr_name, device, strlen(device));
    if (ioctl(sockfd, SIOCGIFINDEX, &ir) == -1) {
      perror("Erro no ioctl: ");
      exit(-1);
    }


    memset(&endereco, 0, sizeof(endereco)); 	/*IP do dispositivo*/
    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ir.ifr_ifindex;
    if (bind(sockfd, (struct sockaddr *)&endereco, sizeof(endereco)) == -1) {
      perror("Erro no bind: ");
      exit(-1);
    }


    memset(&mr, 0, sizeof(mr));          /*Modo Promiscuo*/
    mr.mr_ifindex = ir.ifr_ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    if (setsockopt(sockfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1)	{
      perror("Erro ao fazer setsockopt: ");
      exit(-1);
    }

    return sockfd;
}
