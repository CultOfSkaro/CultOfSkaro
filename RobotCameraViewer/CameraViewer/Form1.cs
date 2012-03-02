using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using HeliosUsbWrapper;
using System.Runtime.InteropServices;
using System.Threading;
using System.Drawing.Imaging;

namespace CameraViewer
{    
    public partial class Form1 : Form
    {
        private usbReader usb;
        private bool captureLoop = false;
        private bool quit = false;

        public Form1()
        {
            InitializeComponent();
        }


        private void startCapture() {
            txtDebug.AppendText("Reading from usb...\r\n");
            usb.read(640 * 480 * 2); //*2 for 16 bits per pixel
        }
             
        private void button1_Click(object sender, EventArgs e)
        {
            txtDebug.AppendText("Starting capture loop...\r\n");
            captureLoop = true;
            startCapture();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
        }

        private void btnStop_Click(object sender, EventArgs e) {
            captureLoop = false;
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            usb = new usbReader(txtDebug);
            usb.gotData += new usbReader.DataEventHandler(usb_gotData);
            usb.connect(this.Handle);
        }

        void usb_gotData(object sender, DataEventArgs e) {
            byte[] data = e.data;
            txtDebug.AppendText("Got Data: " + data.Length + "\r\n");

            int height = data.Length / 640 / 2;
            txtDebug.AppendText("ImageSize: 640 x" + height + "\r\n");

            //Bitmap image = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format32bppArgb);//"C:\\Users\\Garrett\\Pictures\\ny.bmp");
        
            //convert to rgb
            byte[] pixels = new byte[data.Length * 3 / 2];  //24 bpp instead of 16
            for (int d = 0, p = 0; d < data.Length; d += 2, p += 3) {
                int pixel = data[d] << 8 | data[d + 1];
                //double h = Convert.ToDouble((data[d] & 0xFC));
                //double s = Convert.ToDouble((((data[d] & 0x03) << 6) | ((data[d + 1] & 0xE0) >> 2)));
                //double v = Convert.ToDouble((data[d + 1] & 0x1F) << 3);

                //h = h * 360 / 255;
                //s /= 255;
                //v /= 255;
                
                double h = (((pixel >> 10) & 0x3F) << (2)) / 255.0 * 360;
	            double s = (((pixel>>5)&0x1F)<<(3)) / 255.0;
	            double v = ((pixel & 0x1F)<<(3)) / 255.0;
                int r, g, b;
                ColorConvert.HsvToRgbOld(h, s, v, out r, out g, out b);

                //txtDebug.AppendText(r + "," + g + "," + b + "\r\n");
                //image.SetPixel((d / 2) % 640, (d/2)/640, Color.FromArgb(r, g, b));

                pixels[p] = (byte)b; // 255; //(byte)r;
                pixels[p + 1] = (byte)g;
                pixels[p + 2] = (byte)r; // 255; //(byte)b;
            }

            
            //create an image from the pixel data
            Bitmap image;
            unsafe {
                fixed (byte* ptr = pixels) {
                    image = new Bitmap(640, height, 640 * 3, PixelFormat.Format24bppRgb, new IntPtr(ptr));
                }
            }

            pictureBox1.Image = image;


            if (captureLoop) {
                startCapture();
            } else if (quit) {
                this.Close();
            }

            //for (int i = 0; i < e.data.Length; i++) {
            //    txtDebug.AppendText(e.data[i] + "\r\n");
            //}
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e) {
            usb.disconnet();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            //make sure we are done with reading
            if (captureLoop) {
                //wait for last capture to finish
                captureLoop = false;
                e.Cancel = true;
                quit = true;

                txtDebug.AppendText("Waiting for last capture to finish\r\n");
            }
        }

    }

    class DataEventArgs : EventArgs
    {
        public byte[] data;

        public DataEventArgs(byte[] data) {
            this.data = data;
        }

    }

    class usbReader
    {
        public delegate void DataEventHandler(object sender, DataEventArgs e);
        public event DataEventHandler gotData;

        private BackgroundWorker bw;
        private TextBox txtDebug;
        byte[] data;
        int leftToRead = 0;

        byte[] buffer;
        GCHandle bufHandle;
        IntPtr bufAddr;

        public usbReader(TextBox txtDebug) {
            this.txtDebug = txtDebug;
            
            bw = new BackgroundWorker();
            bw.DoWork += new DoWorkEventHandler(bw_DoWork);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bw_RunWorkerCompleted);

            buffer = new byte[512];
            bufHandle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            bufAddr = bufHandle.AddrOfPinnedObject();
        }

        ~usbReader() {
            bufHandle.Free();
        }


        void bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
            if (gotData != null) {
                gotData(this, new DataEventArgs(data));
            }
        }

        //blocking read of usb
        void bw_DoWork(object sender, DoWorkEventArgs e) {
            //txtDebug.AppendText("run");
            
            int curLoc = 0;
            while (leftToRead > 0) {
                int numRead = HeliosUsb.Read(bufAddr);
                Buffer.BlockCopy(buffer, 0, data, curLoc, 
                    Math.Min(numRead, leftToRead));
                leftToRead -= numRead;
                curLoc += numRead;
            }
            /*
            txtDebug.AppendText("got data:\r\n");
            for (int i = 0; i < 10; i++) {
                txtDebug.AppendText(data[i] + "\r\n");
            }
            */
        }

        public void connect(IntPtr windowHandle)
        {
            if (HeliosUsb.Open(windowHandle)) {
                txtDebug.AppendText("Connected to Helios USB!\r\n");
            } else {
                txtDebug.AppendText("Failed to connect to Helios USB!\r\n");
            }
        }

        public void disconnet()
        {
            HeliosUsb.Close();
        }

        public void waitForRead() {
            while (bw.IsBusy) {
                Thread.Sleep(10);
            }
        }

        public void read(int bytes)
        {
            if (!bw.IsBusy) {
                data = new byte[bytes];
                leftToRead = bytes;
                bw.RunWorkerAsync();
                //bw_DoWork(null, null);
            } else {
                throw new Exception("Already reading!");
            }
        }
    }

    class ColorConvert
    {
        public static void HsvToRgb(double h, double s, double v, 
            out int r, out int g, out int b) {
            h /= 60.0;
            //s /= 255.0;
            //v /= 255.0;
            double C = s * v;
            double X = C * (1 - Math.Abs((h % 2) - 1));
            double m = v - C;
            if (0 <= h && h < 1)
            {
                r = Convert.ToInt32((C + m) * 255);
                g = Convert.ToInt32((X + m) * 255);
                b = Convert.ToInt32((m) * 255);
            }
            else if (1 <= h && h < 2)
            {
                r = Convert.ToInt32((X + m) * 255);
                g = Convert.ToInt32((C + m) * 255);
                b = Convert.ToInt32((m) * 255);
            }
            else if (2 <= h && h < 3)
            {
                r = Convert.ToInt32((m) * 255);
                g = Convert.ToInt32((C + m) * 255);
                b = Convert.ToInt32((X + m) * 255);
            }
            else if (3 <= h && h < 4)
            {
                r = Convert.ToInt32((m) * 255);
                g = Convert.ToInt32((X + m) * 255);
                b = Convert.ToInt32((C + m) * 255);
            }
            else if (4 <= h && h < 5)
            {
                r = Convert.ToInt32((X + m) * 255);
                g = Convert.ToInt32((m) * 255);
                b = Convert.ToInt32((C + m) * 255);
            }
            else if (5 <= h && h < 6)
            {
                r = Convert.ToInt32((C + m) * 255);
                g = Convert.ToInt32((m) * 255);
                b = Convert.ToInt32((X + m) * 255);
            }
            else
            {
                r = Convert.ToInt32((m) * 255);
                g = Convert.ToInt32((m) * 255);
                b = Convert.ToInt32((m) * 255);
            }
        }
        
        /// <summary>
        /// Convert HSV to RGB
        /// h is from 0-360
        /// s,v values are 0-1
        /// r,g,b values are 0-255
        /// Based upon http://ilab.usc.edu/wiki/index.php/HSV_And_H2SV_Color_Space#HSV_Transformation_C_.2F_C.2B.2B_Code_2
        /// </summary>
        public static void HsvToRgbOld(double h, double S, double V, 
            out int r, out int g, out int b) {
            
            double H = h;
            while (H < 0) { H += 360; };
            while (H >= 360) { H -= 360; };
            double R, G, B;
            if (V <= 0) { R = G = B = 0; } else if (S <= 0) {
                R = G = B = V;
            } else {
                double hf = H / 60.0;
                int i = (int)Math.Floor(hf);
                double f = hf - i;
                double pv = V * (1 - S);
                double qv = V * (1 - S * f);
                double tv = V * (1 - S * (1 - f));
                switch (i) {

                    // Red is the dominant color

                    case 0:
                        R = V;
                        G = tv;
                        B = pv;
                        break;

                    // Green is the dominant color

                    case 1:
                        R = qv;
                        G = V;
                        B = pv;
                        break;
                    case 2:
                        R = pv;
                        G = V;
                        B = tv;
                        break;

                    // Blue is the dominant color

                    case 3:
                        R = pv;
                        G = qv;
                        B = V;
                        break;
                    case 4:
                        R = tv;
                        G = pv;
                        B = V;
                        break;

                    // Red is the dominant color

                    case 5:
                        R = V;
                        G = pv;
                        B = qv;
                        break;

                    // Just in case we overshoot on our math by a little, we put these here. Since its a switch it won't slow us down at all to put these here.

                    case 6:
                        R = V;
                        G = tv;
                        B = pv;
                        break;
                    case -1:
                        R = V;
                        G = pv;
                        B = qv;
                        break;

                    // The color is not defined, we should throw an error.

                    default:
                        //LFATAL("i Value error in Pixel conversion, Value is %d", i);
                        R = G = B = V; // Just pretend its black/white
                        break;
                }
            }
            r = Clamp((int)(R * 255.0));
            g = Clamp((int)(G * 255.0));
            b = Clamp((int)(B * 255.0));
        }

        /// <summary>
        /// Clamp a value to 0-255
        /// </summary>
        private static int Clamp(int i) {
            if (i < 0) return 0;
            if (i > 255) return 255;
            return i;
        }
    }
}
