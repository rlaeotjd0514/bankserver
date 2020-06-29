using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.Sockets;
using System.Security.RightsManagement;
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
using System.Xml.XPath;
using System.Net;
using System.Runtime.InteropServices;

namespace bankpacketCreator
{
    /// <summary>
    /// PacketCreatorHome.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class PacketCreatorHome : Page
    {
        UInt64 amount = 0;
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct pinfo
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 26)]
            public byte[] ppass;
            [MarshalAs(UnmanagedType.U8)]
            public UInt64 pid;
        };
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct csp_header
        {
            public UInt32 signature;
            public UInt16 protocol_version;
            public UInt16 inquiry_type;//*
            public UInt16 body_length;
            public UInt32 secret;
            public UInt32 footer;
        };
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct csp_body
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 34)]
            public byte[] sender;//*
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 34)]
            public byte[] receiver;//*
            public UInt64 amount;//*
            public UInt64 time_null;
            public UInt32 req_loc;//parse uint32_t to sockaddr 200618
        };
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct csp_packet
        {
            public csp_header header;
            public csp_body body;
        };

        UInt32 SGMK_LE = 0x4B4D4753;
        UInt32 SGMK_BE = 0x53474D4B;
        UInt16 protocolVersion = 0x01;
        UInt32 packet_secret;
        const UInt32 P_headersig = 0x4944554A;
        const UInt32 P_headerfooter = 0x53455952;

        pinfo cspsender, cspreceiver;
        ushort jcode = ushort.MaxValue;
        bool cspsender_ch = false;
        bool cspreceiver_ch = false;
        bool amount_ch = false;

        public PacketCreatorHome()
        {
            cspsender.ppass = new byte[26];
            cspsender.pid = 0;
            cspreceiver.ppass = new byte[26];
            cspreceiver.pid = 0;
            InitializeComponent();
        }        
        bool IsNumber(string s)
        {
            foreach(var c in s)
            {
                if(!char.IsDigit(c))
                {
                    return false;
                }
            }
            return true;
        }                

        private void AmountUpdate(object sender, RoutedEventArgs e)
        {
            if(Amount.Text == "")
            {
                Amount.Text = "Amount";
                amount_ch = false;
                return;
            }
            try
            {                
                amount = UInt64.Parse(Amount.Text);
                amount_ch = true;
            }
            catch (Exception)
            {
                MessageBox.Show("Amount should be digit.");
                amount_ch = false;
                Amount.Text = "Amount";
            }
        }

        private void AmountBoxMouseUp(object sender, RoutedEventArgs e)
        {
            if (Amount.Text == "Amount")
            {
                Amount.Text = "";                    
            }
        }

        private void CspSenderBoxMouseUp(object sender, RoutedEventArgs e)
        {
            if(CspSendTextBox.Text == "Csp Sender")
            {
                CspSendTextBox.Text = "";
            }            
        }

        private void CspSendTextBoxLostFocus(object sender, RoutedEventArgs e)
        {
            if(CspSendTextBox.Text == "")
            {
                CspSendTextBox.Text = "Csp Sender";
            }            
            else
            {
                if(CspSendTextBox.Text.Length > 26)
                {
                    for (int i = 0; i < 26; i++)
                    {
                        cspsender.ppass[i] = (byte)CspSendTextBox.Text[i];
                    }
                    for (int i = 26; i < CspSendTextBox.Text.Length; i++)
                    {
                        if(!char.IsDigit(CspSendTextBox.Text[i]))
                        {
                            MessageBox.Show("Sender Csp information is not valid!");
                            CspSendTextBox.Text = "Csp Sender";
                            return;
                        }
                        cspsender.pid *= 10;
                        cspsender.pid += Convert.ToUInt64(CspSendTextBox.Text[i]) - 48;
                    }
                    cspsender_ch = true;
                }
                else
                {
                    MessageBox.Show("CSP information length should be more then 26 char.");
                    CspSendTextBox.Text = "Csp Sender";
                    cspsender_ch = false;
                    return;
                }
            }
        }

        private void CspReceiverBoxUp(object sender, RoutedEventArgs e)
        {
            if (CspReceiveTextBox.Text == "Csp Receiver")
            {
                CspReceiveTextBox.Text = "";
            }
        }

        private void CspReceiverTextBoxLostFocus(object sender, RoutedEventArgs e)
        {
            if (CspReceiveTextBox.Text == "Csp Receiver")
            {
                CspReceiveTextBox.Text = "";
            }
            else
            {
                if (CspReceiveTextBox.Text.Length > 26)
                {
                    for (int i = 0; i < 26; i++)
                    {
                        cspreceiver.ppass[i] = (byte)CspReceiveTextBox.Text[i];
                    }
                    for (int i = 26; i < CspReceiveTextBox.Text.Length; i++)
                    {
                        if (!char.IsDigit(CspReceiveTextBox.Text[i]))
                        {
                            MessageBox.Show("Receiver Csp information is not valid!");
                            CspReceiveTextBox.Text = "Csp Receiver";
                            return;
                        }
                        cspreceiver.pid *= 10;
                        cspreceiver.pid += Convert.ToUInt64(CspReceiveTextBox.Text[i]) - 48;
                    }
                    cspreceiver_ch = true;
                }
                else
                {
                    MessageBox.Show("CSP information length should be more then 26 char.");
                    CspReceiveTextBox.Text = "Csp Sender";
                    cspreceiver_ch = false;
                    return;
                }
            }            
        }

        const UInt64 u64max = UInt64.MaxValue;
        const uint pinfo_size = 26 + 8;
        const uint header_size = 4 + 2 + 2 + 2 + 4 + 4;
        const uint body_size = pinfo_size + pinfo_size + 8 + 8 + 4;

        private void JobSelection(object sender, SelectionChangedEventArgs e)
        {
            if(jList.SelectedIndex == -1)
            {
                jcode = ushort.MaxValue;
            }
            else
            {
                jcode = (ushort)jList.SelectedIndex;
            }
        }

        byte[] FindCurrentLocation()
        {
            IPHostEntry server = Dns.GetHostEntry(Dns.GetHostName());
            foreach(var ip in server.AddressList)
            {
                if(ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    return ip.GetAddressBytes();
                }
            }
            return null;
        }

        static byte[] CreatePacketByteArray<T>(T p)
        {
            int size = Marshal.SizeOf(p);
            byte[] arr = new byte[size];
            IntPtr p_ = Marshal.AllocHGlobal(size);
            Marshal.StructureToPtr(p, p_, true);
            Marshal.Copy(p_, arr, 0, size);
            Marshal.FreeHGlobal(p_);
            return arr;
        }        

        private void GenerateButtonClick(object sender, RoutedEventArgs e)
        {
            if(jcode == ushort.MaxValue)
            {
                MessageBox.Show("Please choice job code");
                return;
            }
            if(!cspsender_ch || !cspreceiver_ch || !amount_ch)
            {
                MessageBox.Show("information incorrect!");
                return;
            }
            
            csp_packet p1 = new csp_packet();
            p1.body.sender = CreatePacketByteArray<pinfo>(cspsender);
            p1.body.receiver = CreatePacketByteArray<pinfo>(cspreceiver);
            p1.body.amount = amount;
            p1.body.req_loc = BitConverter.ToUInt32(FindCurrentLocation(), 0);

            p1.header.signature = 0x4944554A;
            p1.header.protocol_version = 0x01;
            p1.header.inquiry_type = (ushort)(jcode + 1);
            p1.header.body_length = (UInt16)body_size;
            p1.header.secret = (uint)(new Random().Next(1 << 30)) << 2 | (uint)(new Random().Next(1 << 2));
            p1.header.footer = P_headerfooter;

            byte[] packet_bytes = CreatePacketByteArray<csp_packet>(p1);
            ResultBox.Document.Blocks.Clear();
            ResultBox.Document.Blocks.Add(new Paragraph(new Run(BitConverter.ToString(packet_bytes))));
        }
    }
}
