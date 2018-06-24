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
        //enable=1, roi is open. enable =0, roi is close;
        //xend-xstart=odd number, yend-ystart=odd number, 
        //for example, xstart=0. xend=639, ystart=0, yend=479, enable=1;
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csSetExpo", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csSetExpo(uint value, int isauto);
        //isauto=1,->auto expo. isauto=0,->expo=value;
        Rectangle dimension;
        public delegate int CallBack(IntPtr buff);
        public CallBack mInstance;

        //change resolution here before set roi
        //if xend-xstart=639, then imgwidth=640
        //if yend-ystart=479, then imgheight=480
        int imgwidth = 1280;
        int imgheight = 960;
        int camerasize = 6;
        IntPtr pixelStartAddress;
        Bitmap bmp;
        Bitmap bmp2;
        int idx = 0;
        byte[] imgbuff;
        byte[] pixelValues;
        int dispidx = 0;
        Bitmap dispbmp;
        public Form1()
        {

            InitializeComponent();
            listBox1.Items.Add("0");
            listBox1.Items.Add("1");
            listBox1.Items.Add("2");
            listBox1.Items.Add("3");
            listBox1.Items.Add("4");
            listBox1.Items.Add("5");
            initBitMap();
            mInstance = new CallBack(callbackfunc);
            csInit(mInstance, imgwidth, imgheight);
            imgbuff = new byte[imgwidth * imgheight * camerasize];
              pixelValues= new byte[imgwidth * imgheight];
            dimension = new Rectangle(0, 0, imgwidth, imgheight);
        }

        public int callbackfunc(IntPtr buff)
        {
            /*相机传上来的数据保存在buff中，数据的长度为height*width*camsize,
	例如，假如您的系统为6个相机，每个相机的分辨率为1280*960，那么传上来的数据总长度就是1280*960*6，
	各个相机的图像数据从0号相机到5号相机依次排列，例如2号相机的图像就位于1280*960*2,长度为1280*960,
	只需要对该数据块按照分辨率进行切割，就是对应的相机的图像。*/
            if (idx < 0)
                idx = 0;
            int offset = idx * imgwidth * imgheight;
           
            
            Marshal.Copy(buff, imgbuff, 0, imgwidth * imgheight*camerasize);
            Buffer.BlockCopy(imgbuff, offset, pixelValues, 0,imgheight*imgwidth);
            try
            {
                 bmp2 = bitmap8bpp(pixelValues, imgwidth, imgheight);
            }
            catch
            {

            }
            finally
            {

            }
            
            dispbmp =(Bitmap) bmp2.Clone();
           //var dispbmp = new Bitmap(bmp2);
            pictureBox1.Image = dispbmp;
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

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            idx = listBox1.SelectedIndex;
        }
    }
}
