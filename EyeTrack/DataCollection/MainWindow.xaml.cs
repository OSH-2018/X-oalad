using AForge.Video.DirectShow;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml;

namespace DataCollection
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        XmlDocument info;
        int sampleSize;
        int x;
        int y;
        public MainWindow()
        {
            InitializeComponent();

            Directory.CreateDirectory(@".\dataset");
            Directory.CreateDirectory(@".\dataset\picture");
            Directory.CreateDirectory(@".\dataset\position");

            info = new XmlDocument();
            info.Load(@".\dataset\datasetInfo.xml");
            XmlElement sizeNode = (XmlElement)info.DocumentElement.SelectSingleNode("sampleSize");
            if (sizeNode == null)
                sampleSize = 0;
            else
                sampleSize = int.Parse(sizeNode.InnerText);

            movePoint();
            comboBoxVideoDevices.ItemsSource = new FilterInfoCollection(FilterCategory.VideoInputDevice);
            comboBoxVideoDevices.DisplayMemberPath = "Name";
            if (comboBoxVideoDevices.Items.Count > 0)
            {
                comboBoxVideoDevices.SelectedIndex = 0;
                if (comboBoxVideoDevices.Items.Count == 1) comboBoxVideoDevices.IsEnabled = false;
            }
            vsp.VideoSource = new VideoCaptureDevice(((FilterInfo)comboBoxVideoDevices.SelectedItem).MonikerString);
            vsp.Start();
        }

        private void movePoint()
        {
            Random r = new Random();
            int width = (int)ellActivePoint.Width;
            int height = (int)ellActivePoint.Height;
            int screenWidth = (int)SystemParameters.WorkArea.Width;
            int screenHeight = (int)SystemParameters.WorkArea.Height;
            x = r.Next(0, screenWidth);
            y = r.Next(0, screenHeight);
            int nextLeft = x - width / 2;
            int nextTop = y - height / 2;
            ellActivePoint.Margin = new Thickness(nextLeft, nextTop, 0, 0);
            labCentre.Margin = ellActivePoint.Margin;
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
            {
                vsp.Stop();
                this.Close();
            }
            else if (e.Key == Key.Space)
            {
                movePoint();
            }
            else if (e.Key == Key.Enter)
            {
                capturePicture();
                movePoint();
            }
            else if (e.Key == Key.H)
            {
                if (stp.Visibility == Visibility.Hidden)
                {
                    stp.Visibility = Visibility.Visible;
                }
                else
                {
                    stp.Visibility = Visibility.Hidden;
                }
            }
        }

        private void capturePicture()
        {

            BitmapSource bs = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
                   vsp.GetCurrentVideoFrame().GetHbitmap(),
                   IntPtr.Zero,
                   Int32Rect.Empty,
                   BitmapSizeOptions.FromEmptyOptions());
            BmpBitmapEncoder encoder = new BmpBitmapEncoder();
            using (var stream = new FileStream(@".\dataset\picture\" + sampleSize.ToString() + ".bmp", FileMode.Create))
            {
                encoder.Frames.Add(BitmapFrame.Create(bs));
                encoder.Save(stream);
            }

            using (var stream = new StreamWriter(@".\dataset\position\" + sampleSize.ToString() + ".txt", false))
            {
                stream.Write(x.ToString() + " " + y.ToString());
            }
            /*
            XmlElement positionNode = (XmlElement)info.DocumentElement.SelectSingleNode(sampleSize.ToString());
            if (positionNode == null)
            {
                positionNode = info.CreateElement(sampleSize.ToString());
                XmlElement xNode = info.CreateElement("x");
                xNode.InnerText = x.ToString();
                positionNode.AppendChild(xNode);
                XmlElement yNode = info.CreateElement("y");
                xNode.InnerText = y.ToString();
                positionNode.AppendChild(yNode);

                positionNode.InnerText = sampleSize.ToString();
                info.DocumentElement.AppendChild(positionNode);
            }
            else
            {
                positionNode.ChildNodes[0].InnerText = x.ToString();
                positionNode.ChildNodes[1].InnerText = y.ToString();
            }*/
            sampleSize++;
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.Focus();
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            XmlElement sizeNode = (XmlElement)info.DocumentElement.SelectSingleNode("sampleSize");
            if (sizeNode == null)
            {
                sizeNode = info.CreateElement("sampleSize");
                sizeNode.InnerText = sampleSize.ToString();
                info.DocumentElement.AppendChild(sizeNode);
            }
            else
                sizeNode.InnerText = sampleSize.ToString();
            info.Save(@".\dataset\datasetInfo.xml");

            using (var stream = new StreamWriter(@".\dataset\sampleSize.txt" , false))
            {
                stream.Write(sampleSize);
            }

        }
    }
}
