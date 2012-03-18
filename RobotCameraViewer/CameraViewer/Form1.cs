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
using System.Diagnostics;

namespace CameraViewer
{    
    public partial class Form1 : Form
    {
        private usbReader usb;
        private pixelConverter converter;
        private bool captureLoop = false;
        private bool quit = false;
        private Stopwatch timer = new Stopwatch();
        private byte[] header;
        private int numBlobs;
        private Blob[] blobs = new Blob[200];
        private byte[] hsv;
        private int hsvOffset;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e) {
            usb = new usbReader(txtDebug);
            usb.gotData += new DataEventHandler(usb_gotData);
            usb.connect(this.Handle);

            converter = new pixelConverter();
            converter.finished += new ImageEventHandler(converter_finished);

            header = new byte[64 + 64];
            for (int i = 0; i < 64; i++) {
                header[i] = 0;
            }
            for (int i = 64; i < 64+64; i++) {
                header[i] = 255;
            }

            usb.packetHeader = header;
        }

        private void startCapture() {
            txtDebug.AppendText("Reading from usb...");
            //usb.read(640 * 480 * 2); //*2 for 16 bits per pixel
            //usb.findHeader(header);
            usb.readPacket();
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

        void converter_finished(object sender, ImageEventArgs e) {
            pictureBox1.Image = e.image;

            timer.Stop();
            long mil = timer.ElapsedMilliseconds;
            timer.Restart();
            
            double fps = 1 / (mil / 1000.0);
            lblFps.Text = "fps: " + fps;
        }

        void usb_gotData(object sender, DataEventArgs e) {
            if (captureLoop) {
                startCapture();
            } else if (quit) {
                this.Close();
            }

            txtDebug.AppendText("Done\r\n");
            int imageSize = ByteConvert.toUint32(e.data, 0);
            int dataSize = ByteConvert.toUint32(e.data, 4);
            numBlobs = ByteConvert.toUint32(e.data, 8 + imageSize);
          
            txtDebug.AppendText("Image size: " + imageSize + "\r\n");
            txtDebug.AppendText("Data size: " + dataSize + "\r\n");
            txtDebug.AppendText("num Blobs: " + numBlobs + "\r\n");
            
            
            int blobsStart = 8 + imageSize + 4;
            for (int i = 0; i < numBlobs; i++) {
                int blobStart = blobsStart + i * 20; //5 ints per blob
                int type = ByteConvert.toUint32(e.data, blobStart);
                int left = ByteConvert.toUint32(e.data, blobStart + 4);
                int top = ByteConvert.toUint32(e.data, blobStart + 8);
                int width = ByteConvert.toUint32(e.data, blobStart + 12);
                int height = ByteConvert.toUint32(e.data, blobStart + 16);

                blobs[i] = new Blob(type, left, top, width, height);
            }

            /*
            for (int i = 0; i < 16; i++) {
                txtDebug.AppendText(e.data[i] + " ");
            }
            txtDebug.AppendText("\r\n");
            for (int i = 0; i < 16; i++) {
                txtDebug.AppendText(e.data[i + 16] + " ");
            }
            txtDebug.AppendText("\r\n");
            */

            //save hsv data for mouse over
            hsv = e.data;
            hsvOffset = 8;

            converter.convert(e.data, 8, imageSize);
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

        private static Pen penRed = new Pen(Color.Red, 2);
        private static Pen penBlue = new Pen(Color.Blue, 2);
        private static Pen[] pens = { penBlue, penRed };

        private void pictureBox1_Paint(object sender, PaintEventArgs e) {
            Graphics g = e.Graphics;

            if (numBlobs <= 0) return;

            //txtDebug.AppendText("Draw Blobs\r\n");
            for (int i = 0; i < numBlobs; i++) {
                //txtDebug.AppendText("x: " + blobs[i].left + ", y: " + blobs[i].top +
                //    ", width: " + blobs[i].width + ", height: " + blobs[i].height + "\r\n");

                g.DrawRectangle(pens[blobs[i].type], blobs[i].left, blobs[i].top, 
                    blobs[i].width, blobs[i].height);
            }
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e) {
            if (hsv == null) return;

            int pixelOffset = hsvOffset + (e.Y * 640 + e.X) * 2;
            int pixel = hsv[pixelOffset] << 8 | hsv[pixelOffset + 1];
            /*
            double h = (((pixel >> 10) & 0x3F) << (2)) / 255.0 * 360;
            double s = (((pixel >> 5) & 0x1F) << (3)) / 255.0;
            double v = ((pixel & 0x1F) << (3)) / 255.0;
            */

            int h = (pixel >> 10) & 0x3F;
            int s = (pixel >> 5) & 0x1F;
            int v = pixel & 0x1F;
                  
            lblHsv.Text = "h: " + h + ", s: " + s + ", v: " + v;
        }

    }

    class Blob
    {
        public int type, left, top, width, height;
        public Blob(int type, int left, int top, int width, int height) {
            this.type = type;
            this.left = left;
            this.top = top;
            this.width = width;
            this.height = height;
        }
    }

    delegate void DataEventHandler(object sender, DataEventArgs e);
    class DataEventArgs : EventArgs{
        public byte[] data;
        public DataEventArgs(byte[] data) {
            this.data = data;
        }

    }

    delegate void ImageEventHandler(object sender, ImageEventArgs e);
    class ImageEventArgs : EventArgs
    {
        public Bitmap image;
        public ImageEventArgs(Bitmap image) {
            this.image = image;
        }

    }

    class pixelConverter {
        public event ImageEventHandler finished;
        private BackgroundWorker bw;
        private byte[] raw;
        private int start;
        private int length;
        Bitmap image;
        
        public pixelConverter() {
            bw = new BackgroundWorker();
            bw.DoWork += new DoWorkEventHandler(bw_DoWork);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bw_RunWorkerCompleted);

        }

        public void convert(byte[] raw, int start, int length) {
            if (!bw.IsBusy) {
                this.raw = raw;
                this.start = start;
                this.length = length;
                bw.RunWorkerAsync();
            } else {
                throw new Exception("Already converting!");
            }
        }


        private void bw_DoWork(object sender, DoWorkEventArgs e) {
            byte[] pixels = new byte[length * 3 / 2];  //24 bpp instead of 16
            
            //convert to rgb
            for (int d = start, p = 0; d < start + length; d += 2, p += 3) {
                int pixel = raw[d] << 8 | raw[d + 1];
                double h = (((pixel >> 10) & 0x3F) << (2)) / 255.0 * 360;
                double s = (((pixel >> 5) & 0x1F) << (3)) / 255.0;
                double v = ((pixel & 0x1F) << (3)) / 255.0;
                int r, g, b;
                ColorConvert.HsvToRgb(h, s, v, out r, out g, out b);

                pixels[p] = (byte)b;
                pixels[p + 1] = (byte)g;
                pixels[p + 2] = (byte)r;
            }

            //create an image from the pixel data
            unsafe {
                fixed (byte* ptr = pixels) {
                    image = new Bitmap(640, 480, 640 * 3, 
                        PixelFormat.Format24bppRgb, new IntPtr(ptr));
                }
            }
        }

        private void bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
            if (finished != null) {
                finished(this, new ImageEventArgs(image));
            }
        }
    }

    class usbReader
    {
        public event DataEventHandler gotData;

        public byte[] packetHeader;

        private enum Mode { READ, FIND_HEADER };
        private Mode mode = Mode.READ;
        private bool gettingPacket = false;
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


        private void bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
            if (gotData != null) {
                gotData(this, new DataEventArgs(data));
            }
        }

        //blocking read of usb
        private void bw_DoWork(object sender, DoWorkEventArgs e) {
            int curLoc = 0;

            while (true) {
                int numRead = HeliosUsb.Read(bufAddr);

                /*
                if (numRead != 512) {
                    txtDebug.AppendText("WrongSize: " + numRead + "!\r\n");

                    for (int i = 0; i < numRead; i++) {
                        for (int j = 0; j < 8 && i < numRead; i++, j++) {
                            txtDebug.AppendText(buffer[i] + " ");
                        }
                        txtDebug.AppendText("\r\n");
                    }
                }
                */

                if (mode == Mode.READ) {
                    Buffer.BlockCopy(buffer, 0, data, curLoc,
                        Math.Min(numRead, leftToRead));
                    leftToRead -= numRead;
                    curLoc += numRead;
                    if (leftToRead <= 0) break;
                
                } else if (mode == Mode.FIND_HEADER) {
                    bool found = false;
                    int headerLoc;
                    for (headerLoc = 0; headerLoc < numRead; headerLoc++) {
                        //check every posistion in the read data
                        int j;
                        for (j = 0; j < data.Length; j++) {
                            //check the whole header
                            if (headerLoc + j >= numRead) break;
                            if (buffer[headerLoc + j] != data[j]) break;
                        }

                        if (j == data.Length) {
                            found = true;
                            break;
                        }
                    }

                    if (found == true) {
                        if (gettingPacket) {
                            int dataLoc = headerLoc + data.Length;
                            int size = ByteConvert.toUint32(buffer, dataLoc);
                            int alreadyRead = numRead - dataLoc - 4;
                            data = new byte[size];
                            Buffer.BlockCopy(buffer, dataLoc + 4, data, 0,
                                alreadyRead);
                            mode = Mode.READ;
                            gettingPacket = false;
                            leftToRead = size - alreadyRead;
                            curLoc = alreadyRead;
                        } else {
                            data = new byte[] { (byte)headerLoc };
                            break;
                        }
                    }
                }
            }
        }

        public void connect(IntPtr windowHandle) {
            if (HeliosUsb.Open(windowHandle)) {
                txtDebug.AppendText("Connected to Helios USB!\r\n");
            } else {
                txtDebug.AppendText("Failed to connect to Helios USB!\r\n");
            }
        }

        public void disconnet() {
            HeliosUsb.Close();
        }

        public void waitForRead() {
            while (bw.IsBusy) {
                Thread.Sleep(10);
            }
        }

        public void readPacket() {
            if (packetHeader == null) {
                throw new Exception("packetHeader NULL!");
            }
            gettingPacket = true;
            findHeader(packetHeader);
        }

        public void findHeader(byte[] header) {
            if (!bw.IsBusy) {
                mode = Mode.FIND_HEADER;
                data = header;
                bw.RunWorkerAsync();
                //bw_DoWork(null, null);
            } else {
                throw new Exception("Already reading!");
            }
        }

        public void read(int bytes) {
            if (!bw.IsBusy) {
                data = new byte[bytes];
                mode = Mode.READ;
                leftToRead = bytes;
                bw.RunWorkerAsync();
            } else {
                throw new Exception("Already reading!");
            }
        }
    }

    class ByteConvert
    {
        public static int toUint32(byte[] data, int start) {
            int val = 0;
            for (int i = 0; i < 4; i++) {
                val |= (int)data[start + i] << (3 - i) * 8;
            }

            return val;
        }
    }

    class ColorConvert
    {
        /// <summary>
        /// Convert HSV to RGB
        /// h is from 0-360
        /// s,v values are 0-1
        /// r,g,b values are 0-255
        /// Based upon http://ilab.usc.edu/wiki/index.php/HSV_And_H2SV_Color_Space#HSV_Transformation_C_.2F_C.2B.2B_Code_2
        /// </summary>
        public static void HsvToRgb(double h, double S, double V, 
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
