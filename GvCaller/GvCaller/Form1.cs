using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
namespace GvCaller
{

    public partial class Form1 : Form
    {
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csInit", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csInit(CallBack cb, int w, int h);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csStart", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csStart();
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csGetFrame", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csGetFrame(IntPtr imgbuf);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetROI", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetROI(int xstart, int xend, int ystart, int yend, int enable);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetGaussianA", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetGaussianA(char a);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetGaussianB", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetGaussianB(char b);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetGaussianC", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetGaussianC(char c);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetMaxBrightnessThreshold", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetMaxBrightnessThreshold(char c);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetMaxLineWidth", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetMaxLineWidth(int data);
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetMinLineWidth", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetMinLineWidth(char data);
        //enable=1, roi is open. enable =0, roi is close;
        //xend-xstart=odd number, yend-ystart=odd number, 
        //for example, xstart=0. xend=639, ystart=0, yend=479, enable=1;
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetExpo", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetExpo(uint value, int isauto);
        //isauto=1,->auto expo. isauto=0,->expo=value;

        public delegate int CallBack(IntPtr buff);
        public CallBack mInstance;

        //change resolution here before set roi
        //if xend-xstart=639, then imgwidth=640
        //if yend-ystart=479, then imgheight=480
        int imgwidth = 1280;
        int imgheight = 1024;
        int coordlen = 1280 * 4;
        IntPtr pixelStartAddress;
        Bitmap bmp;

        int xtemp;
        int ytemp;
        public Form1()
        {

            InitializeComponent();
            initBitMap();
            mInstance = new CallBack(callbackfunc);
            csInit(mInstance, imgwidth, imgheight);

        }

        public int callbackfunc(IntPtr buff)
        {
            //buffer stores [1280*1024+1280*4]
            //in which 1280*1024 is image data
            //1280*4 is coordinates.
            //coords data formates is [16b y, 16b x]
            //[0x00 0x01, 0x00 0x03]; 
            //On y=1, x= 3, is the laser point.
            //if the result is invalid, x will be 0x07 0xff,
            // such as
            //[0x00 0x01, 0x07 0xff]
            byte[] coords = new byte[coordlen];//stores 
            byte[] pixelValues = new byte[imgwidth * imgheight];
            Marshal.Copy(buff, pixelValues, 0, imgwidth * imgheight);
            Marshal.Copy(buff, coords, imgwidth * imgheight, coordlen);
            var bmp2 = bitmap8bpp(pixelValues, imgwidth, imgheight);
            var dispbmp = new Bitmap(bmp2);
            pictureBox1.Image = dispbmp;

            for (int i = 0; i < coordlen; i += 4)
            {
                ytemp = coords[i] << 8;
                ytemp += coords[i + 1];
                xtemp = coords[i + 2] << 8;
                xtemp += coords[i + 3];
                if (xtemp > imgwidth)
                {
                    //for invalide result
                    continue;
                }
                //pseudocode:
                /*
                 * cv::Point pt;
                 * pt.x = xtemp;
		         * pt.y = ytemp-1;
                 * circle(frameRGB, pt, 1, cv::Scalar(0, 0, 255));
                 */

            }

            return 1;
        }
        int initBitMap()
        {
            bmp = new Bitmap(imgwidth, imgheight, PixelFormat.Format8bppIndexed);
            ColorPalette monopalette = bmp.Palette;
            Color[] ent = monopalette.Entries;
            for (int i = 0; i < 256; i++)
            {
                ent[i] = Color.FromArgb(i, i, i);
            }
            bmp.Palette = monopalette;
            return 1;
        }
        private Bitmap bitmap8bpp(byte[] pixelValues, int width, int height)
        {
            Rectangle dimension = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData picData = bmp.LockBits(dimension, ImageLockMode.ReadWrite, bmp.PixelFormat);
            pixelStartAddress = picData.Scan0;
            System.Runtime.InteropServices.Marshal.Copy(pixelValues, 0, pixelStartAddress, pixelValues.Length);
            bmp.UnlockBits(picData);
            return bmp;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Random r = new Random();
            byte[] pixelValues = new byte[imgwidth * imgheight];
            int size = Marshal.SizeOf(pixelValues[0]) * pixelValues.Length;
            IntPtr p_temp = new IntPtr();
            p_temp = Marshal.AllocHGlobal(size);
            csGetFrame(p_temp);
            Marshal.Copy(p_temp, pixelValues, 0, imgwidth * imgheight);
            var bmp = bitmap8bpp(pixelValues, imgwidth, imgheight);

            pictureBox1.Image = bmp;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            csStart();
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            int xstart, xend, ystart, yend, enableroi;
            int.TryParse(tb_xstart.Text, out xstart);
            int.TryParse(tb_xend.Text, out xend);
            int.TryParse(tb_ystart.Text, out ystart);
            int.TryParse(tb_yend.Text, out yend);
            enableroi = cb_roienable.Checked == true ? 1 : 0;
            csSetROI(xstart, xend, ystart, yend, enableroi);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int value;
            int.TryParse(tb_expo.Text, out value);
            int auto = cb_expoauto.Checked == true ? 1 : 0;
            csSetExpo((uint)value, auto);
        }
    }
}
