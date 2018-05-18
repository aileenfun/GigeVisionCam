﻿using System;
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

        public delegate int CallBack(IntPtr buff);
        public CallBack mInstance;

        int imgwidth = 1280;
        int imgheight = 960;

        IntPtr pixelStartAddress;
        Bitmap bmp;

        public Form1()
        {

            InitializeComponent();
            initBitMap();
            mInstance = new CallBack(callbackfunc);
            csInit(mInstance, imgwidth, imgheight);

        }

        public int callbackfunc(IntPtr buff)
        {

            //buff = new IntPtr();
            byte[] pixelValues = new byte[imgwidth * imgheight];
            //int size = Marshal.SizeOf(pixelValues[0]) * pixelValues.Length;
            // buff = Marshal.AllocHGlobal(size);
            Marshal.Copy(buff, pixelValues, 0, imgwidth * imgheight);
            var bmp2 = bitmap8bpp(pixelValues, imgwidth, imgheight);

            var dispbmp = new Bitmap(bmp2);
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

            //  Rectangle dimension = new Rectangle(0, 0, bmp.Width, bmp.Height);
            // picData = bmp.LockBits(dimension, ImageLockMode.ReadWrite, bmp.PixelFormat);

            //           bmp.UnlockBits(picData);
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

            //int width = 1280;
            //int height = 960;
            int width = 1280;
            int height = 960;
            byte[] pixelValues = new byte[width * height];
            int size = Marshal.SizeOf(pixelValues[0]) * pixelValues.Length;
            IntPtr p_temp = new IntPtr();
            p_temp = Marshal.AllocHGlobal(size);
            csGetFrame(p_temp);
            Marshal.Copy(p_temp, pixelValues, 0, width * height);
            var bmp = bitmap8bpp(pixelValues, width, height);

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
