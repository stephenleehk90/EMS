namespace Reuters_with_Web_Service
{
    partial class frmLogin
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmLogin));
            this.btn_Cancel = new System.Windows.Forms.Button();
            this.btn_Login = new System.Windows.Forms.Button();
            this.tb_ReutersUserName = new System.Windows.Forms.TextBox();
            this.tb_ReutersPassword = new System.Windows.Forms.TextBox();
            this.ckb_IsWindowAuth = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tb_Database = new System.Windows.Forms.TextBox();
            this.tb_Server = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tb_DBUserName = new System.Windows.Forms.TextBox();
            this.tb_DBPassword = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.ckb_AutoLogin = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btn_Cancel
            // 
            this.btn_Cancel.Location = new System.Drawing.Point(206, 193);
            this.btn_Cancel.Margin = new System.Windows.Forms.Padding(2);
            this.btn_Cancel.Name = "btn_Cancel";
            this.btn_Cancel.Size = new System.Drawing.Size(56, 26);
            this.btn_Cancel.TabIndex = 1;
            this.btn_Cancel.Text = "Exit";
            this.btn_Cancel.UseVisualStyleBackColor = true;
            this.btn_Cancel.Click += new System.EventHandler(this.btn_Cancel_Click);
            // 
            // btn_Login
            // 
            this.btn_Login.Location = new System.Drawing.Point(121, 193);
            this.btn_Login.Margin = new System.Windows.Forms.Padding(2);
            this.btn_Login.Name = "btn_Login";
            this.btn_Login.Size = new System.Drawing.Size(56, 26);
            this.btn_Login.TabIndex = 0;
            this.btn_Login.Text = "Login";
            this.btn_Login.UseVisualStyleBackColor = true;
            this.btn_Login.Click += new System.EventHandler(this.btn_Login_Click);
            // 
            // tb_ReutersUserName
            // 
            this.tb_ReutersUserName.Location = new System.Drawing.Point(267, 28);
            this.tb_ReutersUserName.Margin = new System.Windows.Forms.Padding(2);
            this.tb_ReutersUserName.Name = "tb_ReutersUserName";
            this.tb_ReutersUserName.Size = new System.Drawing.Size(22, 20);
            this.tb_ReutersUserName.TabIndex = 2;
            this.tb_ReutersUserName.Text = "takungpao_rccd";
            this.tb_ReutersUserName.Visible = false;
            // 
            // tb_ReutersPassword
            // 
            this.tb_ReutersPassword.Location = new System.Drawing.Point(267, 55);
            this.tb_ReutersPassword.Margin = new System.Windows.Forms.Padding(2);
            this.tb_ReutersPassword.Name = "tb_ReutersPassword";
            this.tb_ReutersPassword.PasswordChar = '*';
            this.tb_ReutersPassword.Size = new System.Drawing.Size(22, 20);
            this.tb_ReutersPassword.TabIndex = 3;
            this.tb_ReutersPassword.Text = "Rtrt4Kun6PA0";
            this.tb_ReutersPassword.Visible = false;
            // 
            // ckb_IsWindowAuth
            // 
            this.ckb_IsWindowAuth.AutoSize = true;
            this.ckb_IsWindowAuth.Location = new System.Drawing.Point(21, 89);
            this.ckb_IsWindowAuth.Margin = new System.Windows.Forms.Padding(2);
            this.ckb_IsWindowAuth.Name = "ckb_IsWindowAuth";
            this.ckb_IsWindowAuth.Size = new System.Drawing.Size(257, 17);
            this.ckb_IsWindowAuth.TabIndex = 6;
            this.ckb_IsWindowAuth.Text = "Using Windows Authentication login to Database";
            this.ckb_IsWindowAuth.UseVisualStyleBackColor = true;
            this.ckb_IsWindowAuth.CheckedChanged += new System.EventHandler(this.ckb_IsWindowAuth_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 22);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Server:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 50);
            this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(70, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "Data Source:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.tb_Database);
            this.groupBox1.Controls.Add(this.tb_Server);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(21, 6);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(2);
            this.groupBox1.Size = new System.Drawing.Size(242, 81);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Login Server";
            // 
            // tb_Database
            // 
            this.tb_Database.Location = new System.Drawing.Point(89, 50);
            this.tb_Database.Margin = new System.Windows.Forms.Padding(2);
            this.tb_Database.Name = "tb_Database";
            this.tb_Database.Size = new System.Drawing.Size(141, 20);
            this.tb_Database.TabIndex = 13;
            // 
            // tb_Server
            // 
            this.tb_Server.Location = new System.Drawing.Point(89, 22);
            this.tb_Server.Margin = new System.Windows.Forms.Padding(2);
            this.tb_Server.Name = "tb_Server";
            this.tb_Server.Size = new System.Drawing.Size(141, 20);
            this.tb_Server.TabIndex = 12;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tb_DBUserName);
            this.groupBox2.Controls.Add(this.tb_DBPassword);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Location = new System.Drawing.Point(21, 106);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(2);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(2);
            this.groupBox2.Size = new System.Drawing.Size(242, 81);
            this.groupBox2.TabIndex = 14;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Login Detail";
            // 
            // tb_DBUserName
            // 
            this.tb_DBUserName.Location = new System.Drawing.Point(89, 20);
            this.tb_DBUserName.Margin = new System.Windows.Forms.Padding(2);
            this.tb_DBUserName.Name = "tb_DBUserName";
            this.tb_DBUserName.Size = new System.Drawing.Size(141, 20);
            this.tb_DBUserName.TabIndex = 2;
            this.tb_DBUserName.Text = "sa";
            this.tb_DBUserName.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb_DBUserName_KeyPress);
            // 
            // tb_DBPassword
            // 
            this.tb_DBPassword.Location = new System.Drawing.Point(89, 50);
            this.tb_DBPassword.Margin = new System.Windows.Forms.Padding(2);
            this.tb_DBPassword.Name = "tb_DBPassword";
            this.tb_DBPassword.PasswordChar = '*';
            this.tb_DBPassword.Size = new System.Drawing.Size(141, 20);
            this.tb_DBPassword.TabIndex = 3;
            this.tb_DBPassword.Text = "@^!!$!!!";
            this.tb_DBPassword.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb_DBPassword_KeyPress);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(22, 50);
            this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(56, 13);
            this.label7.TabIndex = 11;
            this.label7.Text = "Password:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(22, 22);
            this.label8.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(63, 13);
            this.label8.TabIndex = 7;
            this.label8.Text = "User Name:";
            // 
            // ckb_AutoLogin
            // 
            this.ckb_AutoLogin.AutoSize = true;
            this.ckb_AutoLogin.Location = new System.Drawing.Point(21, 190);
            this.ckb_AutoLogin.Margin = new System.Windows.Forms.Padding(2);
            this.ckb_AutoLogin.Name = "ckb_AutoLogin";
            this.ckb_AutoLogin.Size = new System.Drawing.Size(77, 17);
            this.ckb_AutoLogin.TabIndex = 15;
            this.ckb_AutoLogin.Text = "Auto Login";
            this.ckb_AutoLogin.UseVisualStyleBackColor = true;
            this.ckb_AutoLogin.CheckedChanged += new System.EventHandler(this.ckb_AutoLogin_CheckedChanged);
            // 
            // frmLogin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(290, 227);
            this.Controls.Add(this.tb_ReutersUserName);
            this.Controls.Add(this.tb_ReutersPassword);
            this.Controls.Add(this.ckb_AutoLogin);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.ckb_IsWindowAuth);
            this.Controls.Add(this.btn_Login);
            this.Controls.Add(this.btn_Cancel);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(2);
            this.MaximumSize = new System.Drawing.Size(298, 259);
            this.MinimumSize = new System.Drawing.Size(298, 259);
            this.Name = "frmLogin";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Login";
            this.Activated += new System.EventHandler(this.frmLogin_Activated);
            this.Load += new System.EventHandler(this.frmLogin_Load);
            this.Shown += new System.EventHandler(this.frmLogin_Shown);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.frmLogin_KeyPress);
            this.Validated += new System.EventHandler(this.frmLogin_Validated);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_Cancel;
        private System.Windows.Forms.Button btn_Login;
        private System.Windows.Forms.TextBox tb_ReutersUserName;
        private System.Windows.Forms.TextBox tb_ReutersPassword;
        private System.Windows.Forms.CheckBox ckb_IsWindowAuth;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tb_DBUserName;
        private System.Windows.Forms.TextBox tb_DBPassword;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.CheckBox ckb_AutoLogin;
        private System.Windows.Forms.TextBox tb_Database;
        private System.Windows.Forms.TextBox tb_Server;
    }
}