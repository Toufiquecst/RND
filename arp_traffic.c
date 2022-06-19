#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<features.h>
#include<linux/if_packet.h>
#include<linux/if_ether.h>
#include<errno.h>
#include<sys/ioctl.h>
#include<net/if.h>
#include<string.h>
#include<net/if_arp.h>
struct my_arphdr			//from /usr/include/net/if_arp.h	
  {
    unsigned short int ar_hrd;		/* Format of hardware address.  */
    unsigned short int ar_pro;		/* Format of protocol address.  */
    unsigned char ar_hln;		/* Length of hardware address.  */	
    unsigned char ar_pln;		/* Length of protocol address.  */
    unsigned short int ar_op;		/* ARP opcode (command).  */

    /* Ethernet looks like this : This bit is variable sized
       however...  */
    unsigned char ar_sha[ETH_ALEN];	/* Sender hardware address.  */
    unsigned char ar_sip[4];		/* Sender IP address.  */
    unsigned char ar_tha[ETH_ALEN];	/* Target hardware address.  */
    unsigned char ar_tip[4];		/* Target IP address.  */

  };


int CreateRawSocket()
{
	int rawsock;

	if((rawsock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP)))== -1)
	{
		perror("Error creating raw socket: ");
		exit(-1);
	}

	return rawsock;
}

int BindRawSocketToInterface(char *device, int rawsock)
{
	
	struct sockaddr_ll sll;
	struct ifreq ifr;	//interface request structure to convert device name to kernel recognizable format 			

	bzero(&sll, sizeof(sll));
	bzero(&ifr, sizeof(ifr));
	
	/* First Get the Interface Index  */


	strncpy((char *)ifr.ifr_name, device, IFNAMSIZ);	// ifr_name is the interface name
	if((ioctl(rawsock, SIOCGIFINDEX, &ifr)) == -1)		//returns interface index
	{
		printf("Error getting Interface index !\n");
		exit(-1);
	}

	/* Bind our raw socket to this interface */

	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ARP); 


	if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
	{
		perror("Error binding raw socket to interface\n");
		exit(-1);
	}

	return 1;
	
}


PrintHardwareAddress(char *mesg, unsigned char *p, int len)
{
	printf(mesg);

	while(len--)
	{
		printf("%.2X ", *p);
		p++;
	}

}


PrintArpHeader(unsigned char *packet, int len)
{
	struct ethhdr *ethernet_header;
	struct my_arphdr *aph;
	if(len > sizeof(struct ethhdr))
	{
		aph = (struct my_arphdr *)(packet+sizeof(struct ethhdr) );		//skip ethernet header

		printf("\nSender IP Address: %s\n",inet_ntoa(aph->ar_sip));
		PrintHardwareAddress("Sender Hardware Address: ", aph->ar_sha,ETH_ALEN);
		printf("\n");

		printf("\nTarget IP Address: %s\n",inet_ntoa(aph->ar_tip));
		PrintHardwareAddress("Target Hardware Address: ", aph->ar_tha,ETH_ALEN);
		printf("\n");

		
	}
	else
	{
		printf("Packet size too small !\n");
	}
}



main(int argc, char **argv)
{
	int raw;
	unsigned char packet_buffer[2048]; 
	int len;
	int packets_to_sniff;
	struct sockaddr_ll packet_info;
	int packet_info_size = sizeof(packet_info);
	char *dev,errbuf[100];

	dev="eth0";					//device for sniffing

	/* create the raw socket */

	raw = CreateRawSocket();
	/* Bind socket to interface */

	BindRawSocketToInterface(dev, raw);
	/* Get number of packets to sniff from user */

	packets_to_sniff = atoi(argv[1]);			//packets to sniff,atoi converts string to int
	/* Start Sniffing and print Hex of every packet */
	
	while(packets_to_sniff--)
	{
		if((len = recvfrom(raw, packet_buffer, 2048, 0, (struct sockaddr*)&packet_info, &packet_info_size)) == -1)
		{
			perror("Recv from returned -1: ");
			exit(-1);
		}
		else
		{
			/* Packet has been received successfully !! */
			/* Print ARP Header */
			
			PrintArpHeader(packet_buffer, len);
		}
	}	
	return 0;
}
