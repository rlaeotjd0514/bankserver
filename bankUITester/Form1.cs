﻿using System;
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
            byte[] msg = new byte[SendBodyBox.Text.Length + 4];
            Buffer.BlockCopy(SGMK_LE, 0, msg, 0, SGMK_LE.Length);
            Buffer.BlockCopy(ASCIIEncoding.ASCII.GetBytes(SendBodyBox.Text), 0, msg, SGMK_LE.Length, ASCIIEncoding.ASCII.GetBytes(SendBodyBox.Text).Length);
            clisock1.Send(msg);

            byte[] res = new byte[255];
            clisock1.Receive(res);

            recieveBox.Text = Encoding.UTF8.GetString(res);

            clisock1.Shutdown(SocketShutdown.Both);
            clisock1.Close();
            statuslabel.Text = "connection closed";
        }

        private void resetButton_Click(object sender, EventArgs e)
        {
            if(clisock1.Connected == true)
            {
                clisock1.Shutdown(SocketShutdown.Both);
                clisock1.Close();
            }
            clisock1 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }
    }
}
