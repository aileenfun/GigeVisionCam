#pragma once

#ifdef	 CYUSB_EXPORTS
#define  CYUSB_API extern "C" _declspec(dllexport)
#else
#define  CYUSB_API extern "C" _declspec(dllimport)
#endif

enum CYUSB_DRIVER
{
	NEW_DRIVER,OLD_DRIVER,DRIVER_NUM
};

typedef struct USB_ORDER_S
{
	UCHAR			Target;		//0:TGT_DEVICE;1:TGT_INIFC;2:TGT_ENDPT;3:TGT_OTHER
	UCHAR			ReqType;	//0:REQ_STD;1:REQ_CLASS;2:REQ_VENDOR
	UCHAR			Direction;	//0:DIR_TO_DEVICE;1:DIR_FROM_DEVICE
	UCHAR           ReqCode;
	WORD            Value;
	WORD			Index;

	BYTE*			pData;
	WORD			DataBytes;
	USB_ORDER_S()
	{
		Target=0;
		ReqType=2;
		Direction=0;
		ReqCode=0;
		Value=0;
		Index=0;
		pData=NULL;
		DataBytes=0;
	}
}USB_ORDER,*PUSB_ORDER;

CYUSB_API int CyUsb_Init(CYUSB_DRIVER type=NEW_DRIVER);
CYUSB_API int CyUsb_Destroy();
CYUSB_API int OpenUsb();						    
CYUSB_API int CloseUsb();							
CYUSB_API int ReadData(char* pbuff,LONG &lBytes);
CYUSB_API int WriteData(char* pbuff,LONG &lBytes);
CYUSB_API int SendOrder(PUSB_ORDER pOrder);
CYUSB_API int Reset();