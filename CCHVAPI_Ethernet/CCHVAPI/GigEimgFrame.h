#pragma once
class GigEimgFrame
{
public:
	int m_width;
	int m_height;
	int m_camNum;
	long timestamp;
	int status;
	byte* imgBuf;
	int packnum;
	int packnum_last;
	GigEimgFrame(int width,int height,int camNum):m_width(width),m_height(height),m_camNum(camNum)
	{
		timestamp=0;
		imgBuf=new byte[height*width];
		memset(imgBuf,0,height*width);
	}
	~GigEimgFrame(void)
	{
		delete imgBuf;
	}
};
class GigEudp_buffer
{
public:
	int m_packsize;
	byte * packbuffer;
	GigEudp_buffer(int packsize):m_packsize(packsize)
	{
		packbuffer=new byte[packsize];
	}
	int get_packsize()
	{
		return m_packsize;
	}
	~GigEudp_buffer()
	{
		delete packbuffer;
	}
};
