#pragma once
struct GigEcamPropStruct
{
	int trigMode;
	//_int16 expo;
	int expo;
	int brightness;
	int mirror;
	//_int16 row;
	//_int16 col;
	int row;
	int col;
	/*camPropStruct():trigMode(0),
		expo(40),
		brightness(0x10),
		mirror(0),
		row(0x01e0),
		col(0x0280){}*/
};
class GigEcamProp
{
public:
	GigEcamProp(void);
	~GigEcamProp(void);
	int setCamProp(GigEcamPropStruct prop);
	int getCamProp(GigEcamPropStruct *prop);
	int setRow(int row);
	int getRow();
	int setCol(int col);
	int getCol();
	int setBrightness(int brightness);
	int getBrightness();
	int setMirror(int setMirror);
	int getMirror();
	int setExp(int exp);
	int getExp();
	int setTriggerMode(int tri);
	int getTriggerMode();
	int getBuffer(char* destbuffer);
private:
	GigEcamPropStruct memCamProp;
	int initBuffer();
	char *sendbuff;
	int triggerMode;
	int exp;
	int brightness;
	int mirror;
	int row;
	int col;

};

