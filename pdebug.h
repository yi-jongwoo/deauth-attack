void hexdump(char* data,int len){
	std::cout<<"[start]";
	for(int i=0;i<len;i++){
		if(i%16==0)
			std::cout<<endl;
		std::cout<<std::hex<<(data[i]>>4&0xf)<<(data[i]&0xf)<<' '<<std::dec;
	}
	std::cout<<std::endl<<"[end]"<<std::endl;
}
#define pcap_sendpacket(a,b,c) hexdump((char*)b,c)
