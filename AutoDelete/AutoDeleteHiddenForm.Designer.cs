namespace MyNewService
{
    partial class AutoDeleteHiddenForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AutoDeleteHiddenForm));
            this.rtb_Result = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lb_Count = new System.Windows.Forms.Label();
            this.mynotifyicon = new System.Windows.Forms.NotifyIcon(this.components);
            this.btn_Start = new System.Windows.Forms.Button();
            this.btn_Stop = new System.Windows.Forms.Button();
            this.nud_Interval = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.ckb_FullLog = new System.Windows.Forms.CheckBox();
            this.ckb_AutoStart = new System.Windows.Forms.CheckBox();
            this.ckb_AutoClose = new System.Windows.Forms.CheckBox();
            this.ckb_SimpleDisplay = new System.Windows.Forms.CheckBox();
            this.btn_EmptyFolder = new System.Windows.Forms.Button();
            this.ckb_Del_RO = new System.Windows.Forms.CheckBox();
            this.btn_Whitelisting = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.nud_Interval)).BeginInit();
            this.SuspendLayout();
            // 
            // rtb_Result
            // 
            this.rtb_Result.Location = new System.Drawing.Point(13, 34);
            this.rtb_Result.Name = "rtb_Result";
            this.rtb_Result.ReadOnly = true;
            this.rtb_Result.Size = new System.Drawing.Size(516, 261);
            this.rtb_Result.TabIndex = 0;
            this.rtb_Result.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Result:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(317, 12);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "Process Count";
            this.label2.Visible = false;
            // 
            // lb_Count
            // 
            this.lb_Count.AutoSize = true;
            this.lb_Count.Location = new System.Drawing.Point(399, 12);
            this.lb_Count.Name = "lb_Count";
            this.lb_Count.Size = new System.Drawing.Size(11, 12);
            this.lb_Count.TabIndex = 3;
            this.lb_Count.Text = "0";
            this.lb_Count.Visible = false;
            // 
            // mynotifyicon
            // 
            this.mynotifyicon.Icon = ((System.Drawing.Icon)(resources.GetObject("mynotifyicon.Icon")));
            this.mynotifyicon.Text = "Auto Delete Running";
            this.mynotifyicon.Visible = true;
            this.mynotifyicon.DoubleClick += new System.EventHandler(this.mynotifyicon_DoubleClick);
            // 
            // btn_Start
            // 
            this.btn_Start.Location = new System.Drawing.Point(351, 299);
            this.btn_Start.Name = "btn_Start";
            this.btn_Start.Size = new System.Drawing.Size(75, 21);
            this.btn_Start.TabIndex = 4;
            this.btn_Start.Text = "開始";
            this.btn_Start.UseVisualStyleBackColor = true;
            this.btn_Start.Click += new System.EventHandler(this.btn_Start_Click);
            // 
            // btn_Stop
            // 
            this.btn_Stop.Location = new System.Drawing.Point(454, 299);
            this.btn_Stop.Name = "btn_Stop";
            this.btn_Stop.Size = new System.Drawing.Size(75, 21);
            this.btn_Stop.TabIndex = 5;
            this.btn_Stop.Text = "停止";
            this.btn_Stop.UseVisualStyleBackColor = true;
            this.btn_Stop.Click += new System.EventHandler(this.btn_Stop_Click);
            // 
            // nud_Interval
            // 
            this.nud_Interval.Location = new System.Drawing.Point(80, 301);
            this.nud_Interval.Name = "nud_Interval";
            this.nud_Interval.Size = new System.Drawing.Size(88, 22);
            this.nud_Interval.TabIndex = 6;
            this.nud_Interval.ValueChanged += new System.EventHandler(this.nud_Interval_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(16, 303);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "檢查間距:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(175, 303);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "分鐘";
            // 
            // ckb_FullLog
            // 
            this.ckb_FullLog.AutoSize = true;
            this.ckb_FullLog.Location = new System.Drawing.Point(16, 327);
            this.ckb_FullLog.Name = "ckb_FullLog";
            this.ckb_FullLog.Size = new System.Drawing.Size(96, 16);
            this.ckb_FullLog.TabIndex = 9;
            this.ckb_FullLog.Text = "記錄詳細日誌";
            this.ckb_FullLog.UseVisualStyleBackColor = true;
            this.ckb_FullLog.CheckedChanged += new System.EventHandler(this.ckb_FullLog_CheckedChanged);
            // 
            // ckb_AutoStart
            // 
            this.ckb_AutoStart.AutoSize = true;
            this.ckb_AutoStart.Location = new System.Drawing.Point(16, 346);
            this.ckb_AutoStart.Name = "ckb_AutoStart";
            this.ckb_AutoStart.Size = new System.Drawing.Size(72, 16);
            this.ckb_AutoStart.TabIndex = 10;
            this.ckb_AutoStart.Text = "自動運行";
            this.ckb_AutoStart.UseVisualStyleBackColor = true;
            this.ckb_AutoStart.CheckedChanged += new System.EventHandler(this.ckb_AutoStart_CheckedChanged);
            // 
            // ckb_AutoClose
            // 
            this.ckb_AutoClose.AutoSize = true;
            this.ckb_AutoClose.Location = new System.Drawing.Point(121, 346);
            this.ckb_AutoClose.Name = "ckb_AutoClose";
            this.ckb_AutoClose.Size = new System.Drawing.Size(108, 16);
            this.ckb_AutoClose.TabIndex = 11;
            this.ckb_AutoClose.Text = "完成後自動離開";
            this.ckb_AutoClose.UseVisualStyleBackColor = true;
            this.ckb_AutoClose.CheckedChanged += new System.EventHandler(this.ckb_AutoClose_CheckedChanged);
            // 
            // ckb_SimpleDisplay
            // 
            this.ckb_SimpleDisplay.AutoSize = true;
            this.ckb_SimpleDisplay.Location = new System.Drawing.Point(252, 346);
            this.ckb_SimpleDisplay.Name = "ckb_SimpleDisplay";
            this.ckb_SimpleDisplay.Size = new System.Drawing.Size(96, 16);
            this.ckb_SimpleDisplay.TabIndex = 12;
            this.ckb_SimpleDisplay.Text = "顯示簡單信息";
            this.ckb_SimpleDisplay.UseVisualStyleBackColor = true;
            this.ckb_SimpleDisplay.CheckedChanged += new System.EventHandler(this.ckb_SimpleDisplay_CheckedChanged);
            // 
            // btn_EmptyFolder
            // 
            this.btn_EmptyFolder.Location = new System.Drawing.Point(454, 341);
            this.btn_EmptyFolder.Name = "btn_EmptyFolder";
            this.btn_EmptyFolder.Size = new System.Drawing.Size(75, 21);
            this.btn_EmptyFolder.TabIndex = 13;
            this.btn_EmptyFolder.Text = "工具";
            this.btn_EmptyFolder.UseVisualStyleBackColor = true;
            this.btn_EmptyFolder.Click += new System.EventHandler(this.btn_EmptyFolder_Click);
            // 
            // ckb_Del_RO
            // 
            this.ckb_Del_RO.AutoSize = true;
            this.ckb_Del_RO.Location = new System.Drawing.Point(121, 327);
            this.ckb_Del_RO.Name = "ckb_Del_RO";
            this.ckb_Del_RO.Size = new System.Drawing.Size(96, 16);
            this.ckb_Del_RO.TabIndex = 14;
            this.ckb_Del_RO.Text = "清除唯讀檔案";
            this.ckb_Del_RO.UseVisualStyleBackColor = true;
            this.ckb_Del_RO.CheckedChanged += new System.EventHandler(this.ckb_Del_RO_CheckedChanged);
            // 
            // btn_Whitelisting
            // 
            this.btn_Whitelisting.Location = new System.Drawing.Point(351, 341);
            this.btn_Whitelisting.Name = "btn_Whitelisting";
            this.btn_Whitelisting.Size = new System.Drawing.Size(75, 21);
            this.btn_Whitelisting.TabIndex = 15;
            this.btn_Whitelisting.Text = "白名單";
            this.btn_Whitelisting.UseVisualStyleBackColor = true;
            this.btn_Whitelisting.Click += new System.EventHandler(this.btn_Whitelisting_Click);
            // 
            // AutoDeleteHiddenForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(541, 368);
            this.Controls.Add(this.btn_Whitelisting);
            this.Controls.Add(this.ckb_Del_RO);
            this.Controls.Add(this.btn_EmptyFolder);
            this.Controls.Add(this.ckb_SimpleDisplay);
            this.Controls.Add(this.ckb_AutoClose);
            this.Controls.Add(this.ckb_AutoStart);
            this.Controls.Add(this.ckb_FullLog);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.nud_Interval);
            this.Controls.Add(this.btn_Stop);
            this.Controls.Add(this.btn_Start);
            this.Controls.Add(this.lb_Count);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.rtb_Result);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "AutoDeleteHiddenForm";
            this.ShowIcon = false;
            this.Text = "Auto Delete";
            this.Load += new System.EventHandler(this.AutoDeleteHiddenForm_Load);
            this.Resize += new System.EventHandler(this.AutoDeleteHiddenForm_Resize);
            ((System.ComponentModel.ISupportInitialize)(this.nud_Interval)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox rtb_Result;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lb_Count;
        private System.Windows.Forms.NotifyIcon mynotifyicon;
        private System.Windows.Forms.Button btn_Start;
        private System.Windows.Forms.Button btn_Stop;
        private System.Windows.Forms.NumericUpDown nud_Interval;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox ckb_FullLog;
        private System.Windows.Forms.CheckBox ckb_AutoStart;
        private System.Windows.Forms.CheckBox ckb_AutoClose;
        private System.Windows.Forms.CheckBox ckb_SimpleDisplay;
        private System.Windows.Forms.Button btn_EmptyFolder;
        private System.Windows.Forms.CheckBox ckb_Del_RO;
        private System.Windows.Forms.Button btn_Whitelisting;
    }
}