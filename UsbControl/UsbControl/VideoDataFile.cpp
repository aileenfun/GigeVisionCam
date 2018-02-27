#include "StdAfx.h"
#include "VideoDataFile.h"


CVideoDataFile::CVideoDataFile(void)
{
	m_pFile=NULL;
	m_OpenType=VWRITE;
	m_iReadFrameNum=0;
}

CVideoDataFile::~CVideoDataFile(void)
{
	CloseFile();
}

int CVideoDataFile::Open( CString strFile,VOPENTYPE OpenType/*=VWRITE*/ )
{
	CloseFile();
	m_pFile=new CFile();
	m_OpenType=OpenType;
	BOOL bOpen=FALSE;
	switch(m_OpenType)
	{
	case VREAD:
		bOpen=m_pFile->Open(strFile,CFile::modeRead);
		if(bOpen)
		{
			int iRet=0;
			iRet=m_pFile->Read(&m_vFileHead,sizeof(VFileHead));
			if(iRet!=sizeof(VFileHead))
			{
				bOpen=FALSE;
			}
			else
			{
				m_iReadFrameNum=m_vFileHead.iFrameNum;
			}
		}
		break;
	case VWRITE:
	default:
		bOpen=m_pFile->Open(strFile,CFile::modeWrite|CFile::modeCreate);
		m_OpenType=VWRITE;
		break;
	}
	if(!bOpen)
		return -1;
	return 0;
}

int CVideoDataFile::Close()
{
	return 0;
}

void CVideoDataFile::CloseFile()
{
	if(m_pFile!=NULL)
	{
		if(m_pFile->m_hFile!=CFile::hFileNull)
		{
			m_pFile->Close();
		}
		delete m_pFile;
		m_pFile=NULL;
	}
}

int CVideoDataFile::WriteFrame( void* pData,int iFrameSize )
{
	ASSERT(m_pFile!=NULL&&m_pFile->m_hFile!=CFile::hFileNull);
	if(iFrameSize!=m_vFileHead.iFrameSize)
		return -1;
	else
	{
		int iSeek=0;
		iSeek=sizeof(VFileHead)+m_vFileHead.iFrameNum*m_vFileHead.iFrameSize;
		m_pFile->Seek(iSeek,CFile::begin);
		m_pFile->Write(pData,m_vFileHead.iFrameSize);
		return 0;
	}
}

int CVideoDataFile::ReadFrame( void* pData,int iFrameSize )
{
	ASSERT(m_pFile!=NULL&&m_pFile->m_hFile!=CFile::hFileNull);
	if(iFrameSize!=m_vFileHead.iFrameSize||m_iReadFrameNum<=0)
		return -1;
	else
	{
		int iRet=0;
		int iSeek=0;
		iSeek=sizeof(VFileHead)+(m_vFileHead.iFrameNum-m_iReadFrameNum)*m_vFileHead.iFrameSize;
		m_pFile->Seek(iSeek,CFile::begin);
		iRet=m_pFile->Read(pData,m_vFileHead.iFrameSize);
		--m_iReadFrameNum;
		return	iRet;
	}
}

int CVideoDataFile::SetFrameSize( int iFrameSize )
{
	ASSERT(iFrameSize>0);
	m_vFileHead.iFrameSize=iFrameSize;
	return 0;
}

VOPENTYPE CVideoDataFile::GetOpenType()
{
	return m_OpenType;
}
