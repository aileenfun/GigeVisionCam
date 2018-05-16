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
        public static extern int csInit();
        [DllImport(@"CCHVAPI.dll", EntryPoint = "csStart", CallingConvention = CallingConvention.Cdecl)]
        public static extern int csStart();
        [DllImport(@"CCHVAPI.dll", EntryPoint="csGetFrame",CallingConvention = CallingConvention.Cdecl)]
        public static extern int csGetFrame(IntPtr imgbuf);

        public Form1()
        {
            csInit();
            InitializeComponent();
        }
        private Bitmap bitmap8bpp(byte[]pixelValues,int width,int height)
        {
            Bitmap bmp = new Bitmap(width, height, PixelFormat.Format8bppIndexed);
            ColorPalette monopalette = bmp.Palette;
            Color []ent=monopalette.Entries;
            for (int i = 0; i < 256; i++)
            {
                ent[i] = Color.FromArgb(i, i, i);
            }
            bmp.Palette = monopalette;
            Rectangle dimension = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData picData = bmp.LockBits(dimension, ImageLockMode.ReadWrite, bmp.PixelFormat);
            IntPtr pixelStartAddress = picData.Scan0;
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
            Marshal.Copy(p_temp, pixelValues, 0,width * height);
            var bmp = bitmap8bpp(pixelValues, width, height);

            pictureBox1.Image = bmp;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            csStart();
        }
    }
}
