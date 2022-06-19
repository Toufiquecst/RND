#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<net/if.h>
#include<features.h>
#include<linux/if_packet.h>
#include<linux/if_ether.h>
#include<sys/socket.h>
#include<sys/ioctl.h>

main(int argc, char **argv){
	int rawsock_des,length,count=0,timeinsec;
	unsigned char packet_buffer[2048]; 
	struct sockaddr_ll packet_info;
	int packet_info_size = sizeof(packet_info);
	struct sockaddr_ll sll;
	struct ifreq ifr;
	time_t t1,t2,t3;
	bzero(&sll, sizeof(sll));
	bzero(&ifr, sizeof(ifr));
      

	/* creating raw socket */

	if((rawsock_des = socket(PF_PACKET, SOCK_RAW,ETH_P_ALL ))== -1)  //PF_PACKET family,type,ETH_P_ALL protocol
	{
		perror("Error creating raw socket: ");// to receive all types of packets
		exit(-1);
	}


	
	/* binding socket to interface on which we want to sniff */

		/*  getting the Interface Index which the kernel understands */

	strncpy((char *)ifr.ifr_name,argv[1],IFNAMSIZ); //copying interface name to ifr_name field of the str
	if((ioctl(rawsock_des, SIOCGIFINDEX, &ifr)) == -1) // will return filling ifr_ifindex by kernel
	{
		printf("Error getting Interface index !\n");
		exit(-1);
	}


       	/* bind the raw socket to this interface */
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL); // host byte order->network byte order

	if((bind(rawsock_des, (struct sockaddr *)&sll, sizeof(sll)))== -1)
	{
		perror("Error binding raw socket to interface\n");
		exit(-1);
	}
	
     /* given a particular device on a system,we want to find out the packets that have been sent to that device;here the first ethernet card  */
	t1=time(&t1);
	t2=t1+atoi(argv[2])*60;
	t3=time(&t3);
	while(t3<t2){
		if((length = recvfrom(rawsock_des, packet_buffer, 2048, 0, (struct sockaddr*)&packet_info, &packet_info_size)) == -1)
		{
			perror("Recv from returned -1: ");
			exit(-1);
		}
		else{
	        
			if( Check_Ethernet_Header_ARP(packet_buffer, length))      // ARP packet received
                        {
                            	  printf("ARP Packet no. :  %d ",++count);                  //increment the Count
			          PrintPacketInHex(packet_buffer, length);                      //display each Packet
                        }
		     }
             t3=time(&t3);  
			 } 
	printf("NUMBER OF ARP PACKET= %d" , count);
						} //  main ends

int Check_Ethernet_Header_ARP(unsigned char *packet, int length){
	char ARP_prot_no[2]={8,6}; //as ARP protocol No. is 0x0806
	struct ethhdr *ethernet_header;
	if(length > sizeof(struct ethhdr)){
		ethernet_header = (struct ethhdr *)packet;
		if( memcmp(&ethernet_header->h_proto,ARP_prot_no,2) == 0)  {// is it matches?
			/* First set of 6 bytes are Destination MAC */

			PrintInHex("Destination MAC: ", ethernet_header->h_dest, 6);
			printf("\n");
		
			/* Second set of 6 bytes are Source MAC */

			PrintInHex("Source MAC: ", ethernet_header->h_source, 6);
			printf("\n");
			return 1;
												}
		else
			return 0;
						           }
	else{
	
		printf("Packet size is  too small \n\n\n");
		return 0;
	   }
											}

PrintInHex(char *mesg, unsigned char *p, int length)
{
	printf(mesg);

	while(length--)
	{
		printf("%.2X ", *p);
		p++;
	}

}

PrintPacketInHex(unsigned char *packet, int length)
{
	unsigned char *p = packet;

	printf("\n\n---------Packet---Starts----\n\n");
	
	while(length--)
	{
		printf("%.2x ", *p);
		p++;
	}

	printf("\n\n--------Packet---Ends-----\n\n");

}


