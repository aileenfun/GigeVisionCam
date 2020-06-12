#pragma once
#include <new>
class GigEimgFrame
{
public:
	int m_width;
	int m_height;
	int m_camNum;
	long timestamp;
	int TrigSource;
	byte* imgBuf;
	int packnum;
	int packnum_last;
	long imgtime;

	GigEimgFrame(const GigEimgFrame& b)
	{
		m_width = b.m_width;
		m_height = b.m_height;
		m_camNum = b.m_camNum;
		timestamp = b.timestamp;
		TrigSource = b.TrigSource;
		packnum = b.packnum;
		packnum_last = b.packnum_last;
		imgtime = b.imgtime;
		if (nullptr != b.imgBuf)
		{
			int count = 3;
			do
			{
				imgBuf = new(std::nothrow) byte[m_width * m_height];
				count--;
			} while (count || (nullptr == imgBuf));

			if (nullptr != imgBuf)
			{
				memcpy((void*)imgBuf, (void*)b.imgBuf, m_width * m_height * sizeof(byte));
			}
			else
			{
				imgBuf = nullptr;
			}
		}
		else
		{
			imgBuf = nullptr;
		}
	}
	GigEimgFrame& operator=(const GigEimgFrame& b)
	{
		if (&b == this) { return *this; }

		m_width = b.m_width;
		m_height = b.m_height;
		m_camNum = b.m_camNum;
		timestamp = b.timestamp;
		TrigSource = b.TrigSource;
		packnum = b.packnum;
		packnum_last = b.packnum_last;
		imgtime = b.imgtime;
		if (imgBuf != nullptr) { delete imgBuf; imgBuf = nullptr; }
		if (nullptr != b.imgBuf)
		{
			int count = 3;
			do
			{
				imgBuf = new(std::nothrow) byte[m_width * m_height];
				count--;
			} while (count || (nullptr == imgBuf));

			if (nullptr != imgBuf)
			{
				memcpy((void*)imgBuf, (void*)b.imgBuf, m_width * m_height * sizeof(byte));
			}
			else
			{
				imgBuf = nullptr;
			}
		}
		else
		{
			imgBuf = nullptr;
		}

		return *this;
	}

	GigEimgFrame(int width,int height,int camNum):m_width(width),m_height(height),m_camNum(camNum)
	{
		imgtime = 0;
		timestamp=0;
		m_camNum = 0;
		TrigSource = 0;
		packnum = 0;
		packnum_last = 0;
		imgBuf=new byte[height*width];
		memset(imgBuf,0,height*width);
	}
	~GigEimgFrame(void)
	{
		if (nullptr != imgBuf)
		{
			delete imgBuf;
			imgBuf = nullptr;
		}
	}
};
class GigEudp_buffer
{
public:
	int m_packsize = 0;
	byte * packbuffer = nullptr;
	explicit GigEudp_buffer(int packsize):m_packsize(packsize)
	{
		packbuffer=new byte[packsize];
	}

	GigEudp_buffer(const GigEudp_buffer& b)
	{
		m_packsize = b.m_packsize;
		if (nullptr != b.packbuffer && m_packsize > 0)
		{
			int count = 3;
			do
			{
				packbuffer = new(std::nothrow) byte[m_packsize];
				count--;
			} while (count || (nullptr == packbuffer));

			if (nullptr != packbuffer)
			{
				memcpy((void*)packbuffer, (void*)b.packbuffer, m_packsize * sizeof(byte));
			}
			else
			{
				packbuffer = nullptr;
			}
		}
		else
		{
			packbuffer = nullptr;
		}
	}

	GigEudp_buffer& operator=(const GigEudp_buffer& b)
	{
		if (&b == this) { return *this; }
		m_packsize = b.m_packsize;
		if (packbuffer != nullptr) { delete []packbuffer; packbuffer = nullptr; }
		if (nullptr != b.packbuffer && m_packsize > 0)
		{
			int count = 3;
			do
			{
				packbuffer = new(std::nothrow) byte[m_packsize];
				count--;
			} while (count || (nullptr == packbuffer));

			if (nullptr != packbuffer)
			{
				memcpy((void*)packbuffer, (void*)b.packbuffer, m_packsize * sizeof(byte));
			}
			else
			{
				packbuffer = nullptr;
			}
		}
		else
		{
			packbuffer = nullptr;
		}

		return *this;
	}
	int get_packsize()
	{
		return m_packsize;
	}
	~GigEudp_buffer()
	{
		if (nullptr != packbuffer)
		{
			delete []packbuffer;
			packbuffer = nullptr;
		}
	}
};
