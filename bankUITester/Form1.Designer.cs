namespace bankUITester
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.sendbutton = new System.Windows.Forms.Button();
            this.SendBodyBox = new System.Windows.Forms.RichTextBox();
            this.ipaddressbox = new System.Windows.Forms.TextBox();
            this.portbox = new System.Windows.Forms.TextBox();
            this.connectbutton = new System.Windows.Forms.Button();
            this.statuslabel = new System.Windows.Forms.Label();
            this.recieveBox = new System.Windows.Forms.RichTextBox();
            this.resetButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // sendbutton
            // 
            this.sendbutton.Location = new System.Drawing.Point(12, 170);
            this.sendbutton.Name = "sendbutton";
            this.sendbutton.Size = new System.Drawing.Size(75, 23);
            this.sendbutton.TabIndex = 0;
            this.sendbutton.Text = "send";
            this.sendbutton.UseVisualStyleBackColor = true;
            this.sendbutton.Click += new System.EventHandler(this.sendbutton_Click);
            // 
            // SendBodyBox
            // 
            this.SendBodyBox.Location = new System.Drawing.Point(12, 12);
            this.SendBodyBox.Name = "SendBodyBox";
            this.SendBodyBox.Size = new System.Drawing.Size(383, 152);
            this.SendBodyBox.TabIndex = 1;
            this.SendBodyBox.Text = "";
            // 
            // ipaddressbox
            // 
            this.ipaddressbox.Location = new System.Drawing.Point(594, 172);
            this.ipaddressbox.Name = "ipaddressbox";
            this.ipaddressbox.Size = new System.Drawing.Size(130, 21);
            this.ipaddressbox.TabIndex = 2;
            // 
            // portbox
            // 
            this.portbox.Location = new System.Drawing.Point(730, 172);
            this.portbox.Name = "portbox";
            this.portbox.Size = new System.Drawing.Size(58, 21);
            this.portbox.TabIndex = 3;
            // 
            // connectbutton
            // 
            this.connectbutton.Location = new System.Drawing.Point(513, 172);
            this.connectbutton.Name = "connectbutton";
            this.connectbutton.Size = new System.Drawing.Size(75, 23);
            this.connectbutton.TabIndex = 4;
            this.connectbutton.Text = "connect";
            this.connectbutton.UseVisualStyleBackColor = true;
            this.connectbutton.Click += new System.EventHandler(this.connectbutton_Click);
            // 
            // statuslabel
            // 
            this.statuslabel.AutoSize = true;
            this.statuslabel.Location = new System.Drawing.Point(259, 177);
            this.statuslabel.Name = "statuslabel";
            this.statuslabel.Size = new System.Drawing.Size(38, 12);
            this.statuslabel.TabIndex = 5;
            this.statuslabel.Text = "label1";
            // 
            // recieveBox
            // 
            this.recieveBox.Location = new System.Drawing.Point(402, 12);
            this.recieveBox.Name = "recieveBox";
            this.recieveBox.Size = new System.Drawing.Size(386, 152);
            this.recieveBox.TabIndex = 6;
            this.recieveBox.Text = "";
            // 
            // resetButton
            // 
            this.resetButton.Location = new System.Drawing.Point(432, 172);
            this.resetButton.Name = "resetButton";
            this.resetButton.Size = new System.Drawing.Size(75, 23);
            this.resetButton.TabIndex = 7;
            this.resetButton.Text = "reset";
            this.resetButton.UseVisualStyleBackColor = true;
            this.resetButton.Click += new System.EventHandler(this.resetButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 201);
            this.Controls.Add(this.resetButton);
            this.Controls.Add(this.recieveBox);
            this.Controls.Add(this.statuslabel);
            this.Controls.Add(this.connectbutton);
            this.Controls.Add(this.portbox);
            this.Controls.Add(this.ipaddressbox);
            this.Controls.Add(this.SendBodyBox);
            this.Controls.Add(this.sendbutton);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button sendbutton;
        private System.Windows.Forms.RichTextBox SendBodyBox;
        private System.Windows.Forms.TextBox ipaddressbox;
        private System.Windows.Forms.TextBox portbox;
        private System.Windows.Forms.Button connectbutton;
        private System.Windows.Forms.Label statuslabel;
        private System.Windows.Forms.RichTextBox recieveBox;
        private System.Windows.Forms.Button resetButton;
    }
}

