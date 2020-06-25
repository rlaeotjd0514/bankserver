using System;
using System.Collections.Generic;
using System.Linq;
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

namespace bankpacketCreator
{
    /// <summary>
    /// PacketCreatorHome.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class PacketCreatorHome : Page
    {
        UInt64 amount = 0;
        struct pinfo
        {
            public byte[] ppass;
            public UInt64 pid;
        };

        pinfo cspsender, cspreceiver;
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
                return;
            }
            try
            {
                amount = UInt64.Parse(Amount.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("Amount should be digit.");
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
                        cspsender.pid *= 10;
                        cspsender.pid += Convert.ToUInt64(CspSendTextBox.Text[i]) - 48;
                    }
                }
            }
        }

        private void CspReceiverBoxUp(object sender, RoutedEventArgs e)
        {
            if(CspReceiveTextBox.Text == "Csp Receiver")
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
                        cspreceiver.pid *= 10;
                        cspreceiver.pid += Convert.ToUInt64(CspReceiveTextBox.Text[i]) - 48;
                    }
                }
            }
        }

        private void CspReceiverTextBoxLostFocus(object sender, RoutedEventArgs e)
        {
            if (CspReceiveTextBox.Text == "")
            {
                CspReceiveTextBox.Text = "Csp Receiver";
            }
        }

        private void GenerateButtonClick(object sender, RoutedEventArgs e)
        {
            
        }
    }
}
