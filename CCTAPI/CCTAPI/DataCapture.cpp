#include "StdAfx.h"
#include "DataCapture.h"
#include <tchar.h>
#include <windows.h>
/*
#include <cv.hpp>
#include <opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
*/
//#define _SAVEFILE
//#define _READFILE
//#define _USELINE
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#ifdef _W1280
//#define TOTALPACK 152  //for 1280*960
#endif
#ifdef _W640
//#define TOTALPACK 39	//for 640*480
#endif
CDataCapture::CDataCapture(wqueue<imgFrame*>&queue,CDataProcess *dp)
	:m_queue(queue)
{
	m_pDataProcess=NULL;
	m_iCount=0;		
	m_iRowIndex=0;		
	m_bFindDbFive=FALSE;	
	m_pInData=NULL;		
	m_pOutData=NULL;	
	dataCnt=0;
	frameCnt=0;
	socketSrv=-1;
	this_clientProp=new clientProp();
	this_camprop=new camProp();
	softtrigmode=0;
	m_pDataProcess=dp;
}

CDataCapture::~CDataCapture(void)
{
	Close();
}
int CDataCapture::Open(int height,int width)// 
{
	
	g_height=height;
#ifdef _USELINE
	g_width=width+4;
#else
	g_width=width;
#endif
	TOTALPACK = height*width / 8176+1;
	unsigned int residue = (height*width) % 8176;//residue//8192-16
	if (residue > 0)
	{
		TOTALPACK++;
	}
	g_width_L=g_width;
	//m_pOutData=new byte[g_height*g_width_L];
	//m_pInData=new byte[(ReadDataBytes+g_width_L+3)];
	m_pInData=new byte[ReadDataBytes];
	memset(m_pInData,0,ReadDataBytes*sizeof(byte));
	m_bCapture=TRUE;

	char *sendbuff=new char[32];
	sendbuff[0]=0x56;
	sendbuff[1]=0xab;
	sendbuff[2]=0x04;
	sendbuff[3]=0x00;//stop
	sendbuff[30]=0x57;
	sendbuff[31]=0xac;
	int rst = sendto(socketSrv, sendbuff, 32, 0, (struct sockaddr*)&addrClient, sizeof(struct sockaddr));//stop
	Sleep(10);
	sendbuff[3] = 0x01;//start
	rst=sendto(socketSrv,sendbuff,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
	if (rst < 0)
	{
		unsigned long dw = WSAGetLastError();
		return -dw;
	}
	m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadProcess,this,0,NULL);
	int temp=SetThreadPriority(m_hThread,THREAD_PRIORITY_TIME_CRITICAL);
#ifdef _SAVEFILE
	//savefile.open("C:\\c6udp\\udpfileproces.bin",std::ios::out|std::ios::binary);
	savefile2.open("C:\\c6udp\\udpfile.bin",std::ios::out|std::ios::binary);
#endif
	return 0;
}
int CDataCapture::Close()
{
	char *sendbuff=new char[32];
	sendbuff[0]=0x56;
	sendbuff[1]=0xab;
	sendbuff[2]=0x04;
	sendbuff[3]=0x00;
	sendbuff[30]=0x57;
	sendbuff[31]=0xac;
	int rst=sendto(socketSrv,sendbuff,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));


	m_bCapture=FALSE;
	Sleep(100);
	if(m_pOutData!=NULL)
	{
		delete[] m_pOutData;
		m_pOutData=NULL;
	}
	if(m_pInData!=NULL)
	{
		delete[] m_pInData;
		m_pInData=NULL;
	}
	closeUDP();
	return 0;
}
int CDataCapture::initUDP(int *s)
{
	socketSrv=*s;
	*s=-1;
#ifdef _READFILE
	return 0;
#endif
	
	int rst=-1;
	 len=sizeof(SOCKADDR);
	if(WSAStartup(MAKEWORD(2,2),&wsd) != 0)
    {
        printf("WSAStartup failed !\n");
        return -3;
    }

	socketSrv = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(socketSrv<0)
	{
		perror("create socket error");
		return -4;
	}
    //set server ip
	addrSrv.sin_addr.S_un.S_addr = this_clientProp->getpcIP();
	unsigned long long temp= htonl(inet_addr("192.168.2.7"));
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(8080);
	
	addrClient.sin_addr.S_un.S_addr= htonl(this_clientProp->getcamIP());
	unsigned long long temp1=inet_addr("192.168.2.2");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(8080);
    // bind socket server
    rst = bind(socketSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    if(SOCKET_ERROR == rst)   
    {   
		unsigned long dw = WSAGetLastError();
        printf("bind failed !\n");   
		closeUDP();
		Close();
        return -1;   
    }
	int nRecvBuf = 38400*4096;//38400*4096;//
	if (setsockopt(socketSrv, SOL_SOCKET, SO_RCVBUF, (const char *)&nRecvBuf, sizeof(int)) == -1) {
		printf("Set receive buffer size failed\n");
		exit(EXIT_FAILURE);
	}
	struct timeval tv_out;
	tv_out.tv_sec=5;
	tv_out.tv_usec=0;
	int timeout=100;
	if(setsockopt(socketSrv,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(tv_out))==-1)
	{
		printf("set timeout error");
		exit(EXIT_FAILURE);
	}
	*s=socketSrv;
	return 1;
}

unsigned int __stdcall CDataCapture::get_udp_data_wrapper(void *handle)
{
	CDataCapture* pThis=(CDataCapture*)handle;
	pThis->get_udp_data();
	return 0;
}
void CDataCapture::get_udp_data()
{
	long nRet;
	char* recv_buf=new char[PACKSIZE];
	int cliaddr_len=sizeof(client_addr1); 
	long already_get_num=0;
	char resendbuf[32];
	ZeroMemory(resendbuf,0);
	udp_buffer *this_udpbuffer;
	byte* p_udpbuf;
	long packnum=0;
	long packnum_last=0;
	unsigned int camNum=0;
	unsigned int camNum_last=0;
	long timestamp=0;
	long timestamp_last=0;
	//udp_queue.setlimit(4096);
	unsigned int trailpacknum=39;
	int lostpacks=0;
	bool b_init=false;
	volatile int  resendPackCnt=0;
	volatile int getResendPackCnt=0;
	while (m_bCapture)
	{
#ifdef _READFILE
		nRet=fromfile->Read(recv_buf,PACKSIZE);
		if(nRet>0)
		{
			this_udpbuffer=new udp_buffer(nRet);
			memcpy(this_udpbuffer->packbuffer,recv_buf,this_udpbuffer->m_packsize);
			udp_queue.add(this_udpbuffer);
		}
#else
		//sendto(socketSrv,tempbuf,10,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));

		nRet = recvfrom(socketSrv, recv_buf, PACKSIZE, 0, (struct sockaddr*)&client_addr1, &cliaddr_len); 
		dataCnt += PACKSIZE;
			if(nRet>0)
			{
				this_udpbuffer=new udp_buffer(nRet);
				p_udpbuf=this_udpbuffer->packbuffer; 
				memcpy(p_udpbuf,recv_buf,this_udpbuffer->m_packsize);
				if(p_udpbuf[0]==0x34)
				{
					getResendPackCnt++;
				}
				if(p_udpbuf[0]==0x33)
				{
					camNum=p_udpbuf[2];
					timestamp=p_udpbuf[3]<<16;
					timestamp+=p_udpbuf[4]<<8;
					timestamp+=p_udpbuf[5];
					packnum=p_udpbuf[10]<<8;
					packnum+=p_udpbuf[11];
					
				}
				if(!b_init)
				{
					camNum_last=camNum;
					packnum_last=packnum;
					timestamp_last=timestamp;
					b_init=true;
					continue;
				}
				
				//todo: for single cam, camNum won't change. need better rules
				if((camNum!=camNum_last)&&(p_udpbuf[0]==0x33))//boarder pack check,new frame came
				{
					lostpacks=TOTALPACK-packnum_last;
					if((lostpacks>0)&&(lostpacks<3))//last pack lost
					{
						resendbuf[2]=0x03;//resend cmd
						resendbuf[3]=camNum_last;//camNum
						resendbuf[4]=(timestamp_last>>16)&0xff;//frame cnt H
						resendbuf[5]=(timestamp_last>>8)&0xff;
						resendbuf[6]=timestamp_last&0xff;//frame cnt L
						packnum_last++;
						resendbuf[7]=(packnum_last>>8)&0xff;//resend pack start numH
						resendbuf[8]=packnum_last&0xff;//resend pack start num H
						resendbuf[9]=lostpacks;//resend pack start num L
						resendbuf[30]=0x57;
						resendbuf[31]=0xac;
						sendto(socketSrv,resendbuf,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
						resendPackCnt+=lostpacks;
					}
					lostpacks=packnum-1;
					if((lostpacks>0)&&(lostpacks<3))
					{
						resendbuf[2]=0x03;//resend order
						resendbuf[3]=p_udpbuf[2];//camNum
						resendbuf[4]=p_udpbuf[3];//frame cnt H
						resendbuf[5]=p_udpbuf[4];
						resendbuf[6]=p_udpbuf[5];//frame cnt L
						resendbuf[7]=0;//resend pack start numH
						resendbuf[8]=1;//resend pack start num H
						resendbuf[9]=lostpacks;//resend pack start num L
						resendbuf[30]=0x57;
						resendbuf[31]=0xac;
						sendto(socketSrv,resendbuf,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
						resendPackCnt+=lostpacks;
					}

				}
				else if(p_udpbuf[0]==0x33&&packnum!=0)
				{
					lostpacks=packnum-packnum_last-1;
					if((lostpacks>0)&&(lostpacks<3))
					{
						resendbuf[0]=0x56;
						resendbuf[1]=0xab;
						resendbuf[2]=0x03;//resend order
						resendbuf[3]=p_udpbuf[2];//camNum
						resendbuf[4]=p_udpbuf[3];//frame cnt H
						resendbuf[5]=p_udpbuf[4];
						resendbuf[6]=p_udpbuf[5];//frame cnt L
						packnum_last++;
						resendbuf[7]=(packnum_last>>8)&0xff;//resend pack start numH
						resendbuf[8]=packnum_last&0xff;//resend pack start num H
						resendbuf[9]=lostpacks;//resend pack start num L
						resendbuf[30]=0x57;
						resendbuf[31]=0xac;
						sendto(socketSrv,resendbuf,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
						resendPackCnt+=lostpacks;

					}
				}


				/*
				lostpacks=packnum-packnum_last-1;
				if(p_udpbuf[0]==0x33&&packnum!=0&&(lostpacks>0)&&(lostpacks<3))
				{
					resendbuf[0]=0x56;
					resendbuf[1]=0xab;
					resendbuf[2]=0x03;//resend order
					resendbuf[3]=p_udpbuf[2];//camNum
					resendbuf[4]=p_udpbuf[3];//frame cnt H
					resendbuf[5]=p_udpbuf[4];
					resendbuf[6]=p_udpbuf[5];//frame cnt L
					packnum_last++;
					resendbuf[7]=(packnum_last>>8)&0xff;//resend pack start numH
					resendbuf[8]=packnum_last&0xff;//resend pack start num H
					resendbuf[9]=lostpacks;//resend pack start num L
					resendbuf[30]=0x57;
					resendbuf[31]=0xac;
					sendto(socketSrv,resendbuf,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));

				}
				*/
				camNum_last=camNum;
				packnum_last=packnum;
				timestamp_last=timestamp;
				udp_queue.add(this_udpbuffer);
				
			}

#endif
	}//while

	udp_queue.reset();
	delete recv_buf;
}
int CDataCapture::ThreadProcessFunction()
{
	long nRet=0;
	char recv_buf[1400]="";
	int cliaddr_len=sizeof(client_addr1); 
	char tempbuf[10];
	ZeroMemory(tempbuf,10);
	byte * imgbuf=new byte[g_width*g_height];

#ifdef _READFILE
	fromfile=new CFile();//("d:\\udpfile.bin");
	//CString filename=_L;
	if(!fromfile->Open(_T("C:\\c6udp\\udpfile.bin"),CFile::modeRead))
		return -1;
	//direct read from file, it works, but I'm testing double buffer
	nRet=ReadDataBytes;
	nRet=fromfile->Read(m_pInData,nRet);
	HANDLE m_hThread1 = (HANDLE)_beginthreadex(NULL,0,get_udp_data_wrapper,this,0,NULL);
	//bool temp=SetThreadPriority(m_hThread1,THREAD_PRIORITY_TIME_CRITICAL);
	
#else
		HANDLE m_hThread1 = (HANDLE)_beginthreadex(NULL,0,get_udp_data_wrapper,this,0,NULL);
		int temp=SetThreadPriority(m_hThread1,THREAD_PRIORITY_TIME_CRITICAL);
		sendto(socketSrv,tempbuf,10,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
		nRet=getData(m_pInData,0,ReadDataBytes);
#endif
		int i=0;
		int camNum=-1;
		int camNum_last=-1;
		long row=-1;
		long col=-1;
		thisImgFrame=NULL;
		volatile bool head=FALSE;
		int lastRowIdx=0;
		udp_buffer* this_udp_pack=NULL;
		byte* temppack;
		unsigned long packnum_last=0;
		unsigned long packnum=0;
		unsigned long timestamp_last=0;
		unsigned long cpylen=0;
		vector<imgFrame*> vframe;
		long timestamp;
		//while(!nRet&&m_bCapture)
		//{
		//	nRet=getData(m_pInData,0,ReadDataBytes);
		//	//Sleep(1);
		//}
		
		//m_queue.setlimit(30);
	while(m_bCapture )
	{
		
		
		this_udp_pack=udp_queue.remove();
		if(this_udp_pack==NULL)
		{
			continue;
		}
#ifdef _SAVEFILE
		savefile2.write((char*)this_udp_pack->packbuffer,16);//this_udp_pack->m_packsize);
#endif
		char pack1=this_udp_pack->packbuffer[0];
		char pack2=this_udp_pack->packbuffer[1];
		
		if(this_udp_pack->packbuffer[0]!=0x34)
		{//normal frame process
			camNum=this_udp_pack->packbuffer[2];
			timestamp=this_udp_pack->packbuffer[3]<<16;
			timestamp+=this_udp_pack->packbuffer[4]<<8;
			timestamp+=this_udp_pack->packbuffer[5];
			if(camNum!=camNum_last||timestamp!=timestamp_last)
			{//new frame arrived, judged by camNum and timestamp
				thisImgFrame=new imgFrame(g_width,g_height,camNum);
				thisImgFrame->timestamp=timestamp;
				thisImgFrame->status=1;
				thisImgFrame->packnum=0;
				vframe.push_back(thisImgFrame);
				head=true;
			}
		}
		
		/*
		if((pack1==0x30))//&&(pack2==0xc0))//new frame pack
		{
			camNum=this_udp_pack->packbuffer[2];
			thisImgFrame=new imgFrame(640,480,camNum);
			timestamp=this_udp_pack->packbuffer[3]<<16;
			timestamp+=this_udp_pack->packbuffer[4]<<8;
			timestamp+=this_udp_pack->packbuffer[5];
			thisImgFrame->timestamp=timestamp;
			thisImgFrame->status=1;
			thisImgFrame->packnum=0;
			vframe.push_back(thisImgFrame);
			//thisImgFrame=NULL;
			head=true;
		}
		*/
		if(head&&(this_udp_pack->packbuffer[0]!=0x30))//((this_udp_pack->packbuffer[0]==0x33)||(this_udp_pack->packbuffer[0]==0x34)||(this_udp_pack->packbuffer[0]==0x3f)))
		{
			camNum=this_udp_pack->packbuffer[2];
			
			timestamp=this_udp_pack->packbuffer[3]<<16;
			timestamp+=this_udp_pack->packbuffer[4]<<8;
			timestamp+=this_udp_pack->packbuffer[5];

			packnum=this_udp_pack->packbuffer[10]<<8;
			packnum+=(unsigned int)this_udp_pack->packbuffer[11];
			
			//ASSERT(packnum>0);
			//ASSERT(packnum<=TOTALPACK);
			//ASSERT(camNum<6);
			
			if((packnum==0||packnum>TOTALPACK))
			{
				continue;
			}
			if(camNum>5)
			{
				continue;
			}
			cpylen=this_udp_pack->packbuffer[12]<<8;
			cpylen+=(unsigned int)this_udp_pack->packbuffer[13];
			for(int i=0;i<vframe.size();i++)
			{
				//house keeping
				if(timestamp-vframe[i]->timestamp>3)
				{
					delete vframe[i];
					vframe.erase(vframe.begin()+i);
				}

				if((vframe[i]->m_camNum==camNum)&&(vframe[i]->timestamp==timestamp))
				{
					if(this_udp_pack->packbuffer[0]!=0x3f)
					{
						if((this_udp_pack->packbuffer[0]==0x34)&&(this_udp_pack->packbuffer[2]==0))
						{
							int k=0;
						}
						vframe[i]->packnum++;
						packnum=this_udp_pack->packbuffer[11]-1;//£¿£¿£¿£¿
						
						memcpy(vframe[i]->imgBuf+packnum*PAYLOADSIZE,this_udp_pack->packbuffer+16,cpylen);
					}
					if((vframe[i]->packnum>=TOTALPACK-1))//||(this_udp_pack->packbuffer[0]==0x3f))
					{
						m_queue.add(vframe[i]);
						vframe.erase(vframe.begin()+i);
						
						frameCnt++;
						head=false;
					}
				}
			}

		}
		camNum_last=camNum;
		timestamp_last=timestamp;
		delete this_udp_pack;
		

		/*
		this_udp_pack=udp_queue.remove();
		if(this_udp_pack==NULL)
		{
			continue;
		}
#ifdef _SAVEFILE
		savefile2.write((char*)this_udp_pack->packbuffer,this_udp_pack->m_packsize);
#endif
		char pack1=this_udp_pack->packbuffer[0];
		char pack2=this_udp_pack->packbuffer[1];
		if(pack1==0x30)
		{
			pack2=0xc0;
		}
		if((pack1==0x30))//&&(pack2==0xc0))//new frame pack
		{
			camNum=this_udp_pack->packbuffer[2];
			thisImgFrame=new imgFrame(640,480,camNum);
			long timestamp=this_udp_pack->packbuffer[3]<<16;
			timestamp+=this_udp_pack->packbuffer[4]<<8;
			timestamp+=this_udp_pack->packbuffer[5];
			thisImgFrame->timestamp=timestamp;
			thisImgFrame->status=1;
			thisImgFrame->packnum=0;
			head=true;
		}
		if(head&&((this_udp_pack->packbuffer[0]==0x33)||(this_udp_pack->packbuffer[0]==0x34)))//&&(this_udp_pack->packbuffer[1]==0xcc))
		{
			camNum=this_udp_pack->packbuffer[2];
			packnum=this_udp_pack->packbuffer[10]<<8;
			packnum+=(unsigned int)this_udp_pack->packbuffer[11];
			cpylen=this_udp_pack->packbuffer[12]<<8;
			cpylen+=(unsigned int)this_udp_pack->packbuffer[13];
			packnum=packnum-1;
			memcpy(thisImgFrame->imgBuf+packnum*PAYLOADSIZE,this_udp_pack->packbuffer+16,cpylen);
		}
		if(head&&(this_udp_pack->packbuffer[0]==0x3f))//&&(this_udp_pack->packbuffer[1]==0xcf))
		{
			m_queue.add(thisImgFrame);
			thisImgFrame=NULL;
			frameCnt++;
			head=false;
		}
		delete this_udp_pack;
		*/

	 }//while
	
#ifdef _SAVEFILE
	//savefile.close();
	savefile2.close();
#endif
#ifdef _READFILE
	fromfile->Close();
#endif
	m_queue.reset();
	return 0;
}
int CDataCapture::closeUDP()
{
	closesocket(socketSrv);   
	WSACleanup(); 
	return 1;
}
unsigned int __stdcall CDataCapture::ThreadProcess( void* handle )
{
	CDataCapture* pThis=(CDataCapture*)handle;
	pThis->ThreadProcessFunction();
	return 0;
}
long CDataCapture::getData(byte * buffer,long startpos,long len,long packsize)
{
	long rst=-1;
	long already_get_num=0;
	long nRet=0;
	int cliaddr_len=sizeof(client_addr1); 
	char tempbuf[10];
	ZeroMemory(tempbuf,10);
	udp_buffer* this_udp_buffer;
	long diff=0;
	for(already_get_num=0;already_get_num<len;)
	{
		this_udp_buffer=udp_queue.remove();
		if(this_udp_buffer==NULL)
		{
			break;
		}
//#ifdef _SAVEFILE
//		savefile2.write((char*)this_udp_buffer->packbuffer,this_udp_buffer->m_packsize);
//#endif

		diff=len-already_get_num;
		if(diff<this_udp_buffer->m_packsize)
		{
			memcpy(buffer+startpos+already_get_num,this_udp_buffer->packbuffer,diff);
			udp_buffer *partial_buffer=new udp_buffer(this_udp_buffer->m_packsize-diff);
			memcpy(partial_buffer->packbuffer,this_udp_buffer->packbuffer+diff,this_udp_buffer->m_packsize-diff);
			udp_queue.insert_front(partial_buffer);
			already_get_num+=diff;
			delete this_udp_buffer;
		}
		else
		{
			memcpy(buffer+startpos+already_get_num,this_udp_buffer->packbuffer,this_udp_buffer->m_packsize);
			already_get_num+=this_udp_buffer->m_packsize;
			delete this_udp_buffer;
			if((already_get_num+PACKSIZE)>len)
			break;
		}
		
	}
	dataCnt+=already_get_num;
	//write file tested here, seems good
	rst=already_get_num;
	return rst;
}

unsigned long CDataCapture::getDataCnt()
{
	return dataCnt;
}
unsigned long CDataCapture::getFrameCnt()
{
	return frameCnt;	
}
int CDataCapture::getProp(clientPropStruct* prop)
{
	int buffsize=32;
	char *sendbuff=new char[buffsize];
	sendbuff[0]=0x56;
	sendbuff[1]=0xab;
	sendbuff[2]=0x05;
	sendbuff[30]=0x57;
	sendbuff[31]=0xac;
	char* recv_buf=new char[PACKSIZE];
	int cliaddr_len=sizeof(client_addr1); 
	sendto(socketSrv,sendbuff,buffsize,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
	for(int i=0;i<5;i++)
	{
		int rst=recvfrom(socketSrv, recv_buf, PACKSIZE, 0, (struct sockaddr*)&client_addr1, &cliaddr_len); 
		if(rst>-1)
		{
			if(recv_buf[0]==0x40)
				if((byte)recv_buf[1]==0xe0)
				{
					int camnum=recv_buf[2];
					int trigmode=recv_buf[5];
					int width=(unsigned int)(recv_buf[6]<<8);

					int temp=(unsigned char)recv_buf[7];
					width=temp+width;
					int height=(unsigned int)(recv_buf[8]<<8);
					temp=(unsigned char)recv_buf[9];
					height=height+temp;
					int expotime=(unsigned int)recv_buf[10]<<8;
					temp=(unsigned char)recv_buf[11];
					expotime=expotime+temp;
					int brightness=recv_buf[12];
					int mirror=recv_buf[13];
					prop->width=width;
					prop->height=height;
					prop->camCnt=camnum;
					if(camnum<1)
						return -1;
					return camnum;
				}
			
		}
	}
	return -2;

}
int CDataCapture::sendProp(clientPropStruct prop)
{
	
	this_clientProp->setClientProp(prop);
	m_pDataProcess->setCamSize(prop.camCnt);
	return 0;
}
int CDataCapture::sendOrder(camPropStruct camprop,int s)
{
	char *sendbuff=new char[32];

	this_camprop->setCamProp(camprop);
	this_camprop->getBuffer(sendbuff);
	if(s==0)
	sendto(socketSrv,sendbuff,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
	return 0;
}
unsigned __int32 CDataCapture::ReadReg(unsigned __int32 addr)
{

	return -1;
}
unsigned __int32 CDataCapture::WriteReg(unsigned __int32 addr, unsigned __int32 data)
{
	char *sendbuff=new char[32];
	memset(sendbuff,32,0);
	sendbuff[0]=0x42;
	sendbuff[1]=0x01;
	sendbuff[2]=0x00;
	sendbuff[3]=0x82;
	sendbuff[4]=0x00;
	sendbuff[5]=0x08;
	sendbuff[6]=0x00;
	sendbuff[7]=0x02;
	sendbuff[8]=(addr&(0xff<<3*8))>>(3*8);
	sendbuff[9]=(addr&(0xff<<2*8))>>(2*8);
	sendbuff[10]=(addr&(0xff<<1*8))>>(1*8);
	sendbuff[11]=(addr&(0xff<<0*8))>>(0*8);
	sendbuff[12]=(data&(0xff<<3*8))>>(3*8);
	sendbuff[13]=(data&(0xff<<2*8))>>(2*8);
	sendbuff[14]=(data&(0xff<<1*8))>>(1*8);
	sendbuff[15]=(data&(0xff<<0*8))>>(0*8);
	sendto(socketSrv,sendbuff,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
	return 1;
}
int CDataCapture::sendSoftTrig(int s)
{
	char *sendbuff=new char[32];
	switch (s)
	{
	case 0://disable soft trig mode
		m_pDataProcess->setsofttrigmode(0);
		softtrigmode=0;
		break;
	case 1://open soft trig mode
		softtrigmode=1;
		udp_queue.clear();
		m_queue.clear();
		m_pDataProcess->setsofttrigmode(1);
		break;
	case 2://trig once
		//m_pDataProcess->softtrigonce();
		sendbuff[0]=0x56;
		sendbuff[1]=0xab;
		sendbuff[2]=0x02;
		sendbuff[30]=0x57;
		sendbuff[31]=0xac;
		sendto(socketSrv,sendbuff,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
		break;
	case 3://trig once
		sendbuff[0]=0x56;
		sendbuff[1]=0xab;
		sendbuff[2]=0x03;
		sendbuff[30]=0x57;
		sendbuff[31]=0xac;
		sendto(socketSrv,sendbuff,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
		break;
	case 4:
		sendbuff[0]=0x56;
		sendbuff[1]=0xab;
		sendbuff[2]=0x04;
		sendbuff[30]=0x57;
		sendbuff[31]=0xac;
		sendto(socketSrv,sendbuff,32,0,(struct sockaddr*)&addrClient,sizeof(struct sockaddr));
		Sleep(100);
	default:
		m_pDataProcess->setsofttrigmode(0);
		softtrigmode=0;

	}
delete sendbuff;	
	return s;
}