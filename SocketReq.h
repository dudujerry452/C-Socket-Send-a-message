#ifndef _SOCKETREQ_H_ 
#define _SOCKETREQ_H_
 
#include<winsock.h>
#include<cstring>
#include<sstream>
#include<iostream>
#include<vector>

#define UnknownError -1
#define SendOutError 0 
#define ResponseError 1 
#define OK 2 

using namespace std;

#pragma comment(lib,"ws2_32")

struct RESP{
	int ErrorCod;
	int Http1,Http2;
	int RESPcod;
	string Sresp;
};


class HTTPRES{
	public:
		void decver(int h1,int h2);
		void dectype(const char* type);
		void adddata(const char* data);
		void dechost(const char* host);
		void addhead(const char* head,const char* type);
		void addurl(const char* url);
		RESP hsend();
		void clear();
	private:
		string Data;
		string Type;
		string Heads;
		string Host;
		int Ver[2];
		string Url;
		string bufSend;
		char* GetIpByDomainName(const char* szHost);
		RESP toRESP(const char* RECV);
};

char* HTTPRES::GetIpByDomainName(const char *szHost)
{
	
	
	
    WSADATA        wsaData;
	/*
	string ss(szIp);
	char* tszIp=NULL;

	memcpy(tszIp,ss.c_str(),ss.length()+1);
	*/
	char* szIp=NULL;
	
    HOSTENT   *pHostEnt;
    int             nAdapter = 0;
    struct       sockaddr_in   sAddr;
    
    if (WSAStartup(0x0101, &wsaData))
    {
        printf(" gethostbyname error for host:\n");
        return NULL;
    }
    pHostEnt = gethostbyname(szHost);
    if (pHostEnt)
    {
        if (pHostEnt->h_addr_list[nAdapter])
        {
            memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);
            szIp=inet_ntoa(sAddr.sin_addr);
        }
    }
    else
    {
        //      DWORD  dwError = GetLastError();
        //      CString  csError;
        //      csError.Format("%d", dwError);
    }
    
    WSACleanup();
    return szIp;
}

RESP HTTPRES::hsend()
{
	
	bufSend=Type+" "+Url+" "+"HTTP/"+(char)(Ver[0]+48)+"."+char(Ver[1]+48)+"\r\n";
	bufSend=bufSend+Heads+"\r\n";
	if(Type=="POST"){
		bufSend=bufSend+Data+"\r\n\r\n";
	}
	
	RESP re;
	
	if(bufSend==""){
		re.ErrorCod=SendOutError;
		return re;
	}
     //开始进行socket初始化;
    WSADATA wData;  
    ::WSAStartup(MAKEWORD(2,2),&wData);  

    SOCKET clientSocket = socket(AF_INET,1,0);      
    struct sockaddr_in ServerAddr = {0};  
    int Ret=0;  
    int AddrLen=0;  
    HANDLE hThread=0; 
	
    
	
    //const char *bufSend = www.baidu.com;

    const char* addIp = GetIpByDomainName(Host.c_str());
    ServerAddr.sin_addr.s_addr = inet_addr(addIp);  
    ServerAddr.sin_port = htons(80);;  
    ServerAddr.sin_family = AF_INET;  
    char bufRecv[3069] = {0};  
    int errNo = 0;  
    int Cod=0;
    
    errNo = connect(clientSocket,(sockaddr*)&ServerAddr,sizeof(ServerAddr));  
	if(errNo==0)  
    {  
    	RESP re;
        if(send(clientSocket,bufSend.c_str() ,bufSend.length(),0)<=0){  
        	re.ErrorCod=SendOutError;
            return re;
        } 
        if(recv(clientSocket,bufRecv,3069,0)<=0)  
        {  
        	re.ErrorCod=ResponseError;
    		return re;
        }  
        re=toRESP(bufRecv);
    }  
    else  
    {  
        errNo=WSAGetLastError();
		re.ErrorCod=UnknownError;
		return re;  
    }  
    //socket环境清理;
    ::WSACleanup();  
    return re;
}
RESP HTTPRES::toRESP(const char* RECV)
{
	string St(RECV);
	cout<<St;
	RESP Re;
	Re.Sresp=RECV;
	Re.ErrorCod=OK;
	Re.Http1=St[5]-48;
	Re.Http2=St[7]-48;
	
	char rECod[4]={St[9],St[10],St[11],'\0'};
	Re.RESPcod=atoi(rECod);
	
	return Re; 
}
void HTTPRES::dectype(const char* type){
	string Ty(type);
	Type=Ty;
}
void HTTPRES::adddata(const char* data){
	Data=data;
}
void HTTPRES::addhead(const char* head,const char* type){
	Heads=Heads+head+":"+type+"\r\n";
}
void HTTPRES::dechost(const char* host){
	if(Host.empty()){
		Heads=Heads+"Host:"+host+"\r\n";
	}
	Host=host;
}
void HTTPRES::decver(int h1,int h2){
	Ver[0]=h1;
	Ver[1]=h2;
}
void HTTPRES::addurl(const char* url){
	Url=url;
}
void HTTPRES::clear(){
	Data.clear();
	Type.clear();
	Heads.clear();
	Host.clear();
	bufSend.clear();
}
#endif
