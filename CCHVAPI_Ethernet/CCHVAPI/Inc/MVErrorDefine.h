// -*- coding: gb2312-dos -*-
/****************************************************************************************************
 *
 * ��Ȩ��Ϣ����Ȩ���� (c) 2015, ���ݺ����������ּ����ɷ����޹�˾, ��������Ȩ��
 *
 * �ļ����ƣ�MvErrorDefine.h
 * ժ    Ҫ�������붨��
 *
 * ��ǰ�汾��1.0.0.0
 * ��    �ߣ�������
 * ��    �ڣ�2015-01-28
 * ��    ע���½�
 ***************************************************************************************************/

#ifndef _MV_ERROR_DEFINE_H_
#define _MV_ERROR_DEFINE_H_

#define MV_OK                   0           ///< �ɹ����޴���
#define MV_E_HANDLE             0x80000000  ///< �������Ч�ľ��
#define MV_E_SUPPORT            0x80000001  ///< ��֧�ֵĹ���
#define MV_E_BUFOVER            0x80000002  ///< ��������
#define MV_E_CALLORDER          0x80000003  ///< ��������˳�����
#define MV_E_PARAMETER          0x80000004  ///< ����Ĳ���
#define MV_E_RESOURCE           0x80000006  ///< ��Դ����ʧ��
#define MV_E_NETER              0x80000007  ///< ������ش���
#define MV_E_NODATA             0x80000008  ///< ������
#define MV_E_PRECONDITION       0x80000009  ///< ǰ���������󣬻����л����ѷ����仯
#define MV_E_PACKET             0x80000080  ///< ��������ݴ���
#define MV_E_SYS                0x80000081  ///< ϵͳ���ó���

// GenICamϵ�д���
#define MV_E_GC_GENERIC         0x80000100  ///< ͨ�ô���
#define MV_E_GC_ARGUMENT        0x80000101  ///< �����Ƿ�
#define MV_E_GC_RANGE           0x80000102  ///< ֵ������Χ
#define MV_E_GC_PROPERTY        0x80000103  ///< ����
#define MV_E_GC_RUNTIME         0x80000104  ///< ���л���������
#define MV_E_GC_LOGICAL         0x80000105  ///< �߼�����
#define MV_E_GC_ACCESS          0x80000106  ///< ����Ȩ������
#define MV_E_GC_TIMEOUT         0x80000107  ///< ��ʱ
#define MV_E_GC_DYNAMICCAST     0x80000108  ///< ת���쳣

//GEV_STATUS��Ӧ�Ĵ�����
#define MV_E_NOT_IMPLEMENTED    0x800000A1  ///< ������豸֧��
#define MV_E_INVALID_ADDRESS    0x800000A2  ///< ���ʵ�Ŀ���ַ������
#define MV_E_WRITE_PROTECT      0x800000A3  ///< Ŀ���ַ����д
#define MV_E_ACCESS_DENIED      0x800000A4  ///< ������Ȩ��
#define MV_E_BUSY               0x800000A5  ///< �豸æ��������Ͽ�
#define MV_E_UNKNOW             0x800000ff  ///< δ֪�Ĵ���

#define MV_E_UPG_FILE_MISMATCH     0x80000210 ///< �����̼���ƥ��
#define MV_E_UPG_LANGUSGE_MISMATCH 0x80000211 ///< �����̼����Բ�ƥ��
#define MV_E_UPG_CONFLICT          0x80000212 ///< ������ͻ���豸�Ѿ����������ٴ��������������ش˴���
#define MV_E_UPG_INNER_ERR         0x80000213 ///< ����ʱ����ڲ����ִ���

#endif //_MV_ERROR_DEFINE_H_
