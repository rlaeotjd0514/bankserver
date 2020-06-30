using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;
using System.Threading;

namespace bankUITester
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            statuslabel.Text = "";
        }

        Socket clisock1 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        Socket clisock2 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        byte[] SGMK_LE = { 0x4B, 0x4D, 0x47, 0x53 };

        private void connectbutton_Click(object sender, EventArgs e)
        {
            IPEndPoint servadd = new IPEndPoint(IPAddress.Parse(ipaddressbox.Text), Convert.ToInt32(portbox.Text));
            try
            {
                clisock1.Connect(servadd);
                statuslabel.Text = "connection succeed";
            }
            catch(Exception ex)
            {
                statuslabel.Text = ex.ToString() + "\nconnection failed";
            }            
        }

        private void sendbutton_Click(object sender, EventArgs e)
        {
            string cpass = SendBodyBox.Text.Substring(0, 26);
            UInt64 cid_s = Convert.ToUInt64(SendBodyBox.Text.Substring(26));
            byte[] CpassByteTray = ASCIIEncoding.ASCII.GetBytes(cpass);            
            byte[] CidByteTray = BitConverter.GetBytes(cid_s);
            byte[] msg = new byte[CpassByteTray.Length + CidByteTray.Length + 4];
            Buffer.BlockCopy(SGMK_LE, 0, msg, 0, SGMK_LE.Length);
            Buffer.BlockCopy(CpassByteTray, 0, msg, SGMK_LE.Length, CpassByteTray.Length);
            Buffer.BlockCopy(CidByteTray, 0, msg, SGMK_LE.Length + CpassByteTray.Length, CidByteTray.Length);
            clisock1.Send(msg);

            byte[] res = new byte[255];
            clisock1.Receive(res);

            string resv = Encoding.UTF8.GetString(res);
            recieveBox.Text = resv;

            EndPoint lep = clisock1.LocalEndPoint;

            int acport = ((IPEndPoint)clisock1.LocalEndPoint).Port;

            clisock1.Shutdown(SocketShutdown.Both);
            clisock1.Close();

            if (resv.Trim('\0') == "queued")
            {
                //MessageBox.Show("connecting to " + acport.ToString());                
                clisock2.Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), acport - 1));                
                byte[] server_res = new byte[1024];                
                byte[] test_req = new byte[] { 0x4A, 0x55, 0x44, 0x49, 0x01, 0x00, 0x01, 0x00, 0x58, 0x00, 0x3B, 0x1E, 0x40, 0xC4, 0x52, 0x59, 0x45, 0x53, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x15, 0xCD, 0x5B, 0x07, 0x00, 0x00, 0x00, 0x00, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x39, 0xB1, 0x7F, 0x39, 0x05, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0xDB, 0x64 };
                clisock2.Send(test_req);
                //clisock2.Receive(server_res);
                MessageBox.Show(String.Format("connection succeeded!\nServer Response:{0}", Encoding.UTF8.GetString(server_res)));                
            }

            
            statuslabel.Text = "connection closed";
            resetButton_Click(null, EventArgs.Empty);
        }

        private void resetButton_Click(object sender, EventArgs e)
        {
            if(clisock1.Connected == true)
            {
                clisock1.Shutdown(SocketShutdown.Both);
                clisock1.Close();
            }
            clisock1 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            if (clisock2.Connected == true)
            {
                clisock2.Shutdown(SocketShutdown.Both);
                clisock2.Close();
            }
            clisock2 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }
    }
}
