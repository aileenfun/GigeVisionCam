#pragma once

typedef struct VFileHead_S
{
	int iFrameNum;//Ö¡Êý
	int iFrameSize;
	VFileHead_S()
	{
		memset(this,0,sizeof(*this));
	}
}VFileHead,*PVFileHead;

enum VOPENTYPE
{
	VWRITE,VREAD
};

class CVideoDataFile
{
public:
	CVideoDataFile(void);
	~CVideoDataFile(void);

public:
	int Open(CString strFile,VOPENTYPE OpenType=VWRITE);
	int Close();
	int WriteFrame(void* pData,int iFrameSize);
	int ReadFrame(void* pData,int iFrameSize);
	int SetFrameSize(int iFrameSize);
	VOPENTYPE GetOpenType();

private:
	void CloseFile();

private:
	CFile* m_pFile;
	VOPENTYPE m_OpenType;
	VFileHead m_vFileHead;
	int m_iReadFrameNum;	//¿É¶ÁÖ¡Êý
};

