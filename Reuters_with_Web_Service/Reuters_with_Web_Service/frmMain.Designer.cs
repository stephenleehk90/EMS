namespace Reuters_with_Web_Service
{
    partial class frm_Main
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frm_Main));
            this.btn_Start = new System.Windows.Forms.Button();
            this.btn_Clear = new System.Windows.Forms.Button();
            this.btn_Exit = new System.Windows.Forms.Button();
            this.btn_Stop = new System.Windows.Forms.Button();
            this.rtb_Result = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_Config = new System.Windows.Forms.Button();
            this.lv_Config = new System.Windows.Forms.ListView();
            this.colh_Entry = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.btn_AddConfig = new System.Windows.Forms.Button();
            this.nup_TimeInterval = new System.Windows.Forms.NumericUpDown();
            this.btn_Remove = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.lv_Detail = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.cb_SelectAll = new System.Windows.Forms.CheckBox();
            this.btn_ClearAll = new System.Windows.Forms.Button();
            this.bgWorker = new System.ComponentModel.BackgroundWorker();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.nup_TimeInterval)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btn_Start
            // 
            this.btn_Start.Location = new System.Drawing.Point(113, 283);
            this.btn_Start.Name = "btn_Start";
            this.btn_Start.Size = new System.Drawing.Size(75, 30);
            this.btn_Start.TabIndex = 0;
            this.btn_Start.Text = "Start";
            this.btn_Start.UseVisualStyleBackColor = true;
            this.btn_Start.Click += new System.EventHandler(this.btn_Start_Click);
            // 
            // btn_Clear
            // 
            this.btn_Clear.Location = new System.Drawing.Point(404, 283);
            this.btn_Clear.Name = "btn_Clear";
            this.btn_Clear.Size = new System.Drawing.Size(169, 30);
            this.btn_Clear.TabIndex = 1;
            this.btn_Clear.Text = "Clear Screen Message";
            this.btn_Clear.UseVisualStyleBackColor = true;
            this.btn_Clear.Click += new System.EventHandler(this.btn_Clear_Click);
            // 
            // btn_Exit
            // 
            this.btn_Exit.Location = new System.Drawing.Point(637, 283);
            this.btn_Exit.Name = "btn_Exit";
            this.btn_Exit.Size = new System.Drawing.Size(75, 30);
            this.btn_Exit.TabIndex = 2;
            this.btn_Exit.Text = "Exit";
            this.btn_Exit.UseVisualStyleBackColor = true;
            this.btn_Exit.Click += new System.EventHandler(this.btn_Exit_Click);
            // 
            // btn_Stop
            // 
            this.btn_Stop.Location = new System.Drawing.Point(249, 283);
            this.btn_Stop.Name = "btn_Stop";
            this.btn_Stop.Size = new System.Drawing.Size(75, 30);
            this.btn_Stop.TabIndex = 3;
            this.btn_Stop.Text = "Stop";
            this.btn_Stop.UseVisualStyleBackColor = true;
            this.btn_Stop.Click += new System.EventHandler(this.btn_Stop_Click);
            // 
            // rtb_Result
            // 
            this.rtb_Result.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.rtb_Result.Location = new System.Drawing.Point(0, 338);
            this.rtb_Result.Name = "rtb_Result";
            this.rtb_Result.ReadOnly = true;
            this.rtb_Result.Size = new System.Drawing.Size(892, 301);
            this.rtb_Result.TabIndex = 4;
            this.rtb_Result.Text = "";
            this.rtb_Result.TextChanged += new System.EventHandler(this.rtb_Result_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 314);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 17);
            this.label1.TabIndex = 6;
            this.label1.Text = "Result:";
            // 
            // btn_Config
            // 
            this.btn_Config.Location = new System.Drawing.Point(800, 74);
            this.btn_Config.Name = "btn_Config";
            this.btn_Config.Size = new System.Drawing.Size(87, 30);
            this.btn_Config.TabIndex = 5;
            this.btn_Config.Text = "Config";
            this.btn_Config.UseVisualStyleBackColor = true;
            this.btn_Config.Click += new System.EventHandler(this.btn_Config_Click);
            // 
            // lv_Config
            // 
            this.lv_Config.CheckBoxes = true;
            this.lv_Config.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colh_Entry});
            this.lv_Config.FullRowSelect = true;
            this.lv_Config.Location = new System.Drawing.Point(15, 27);
            this.lv_Config.Name = "lv_Config";
            this.lv_Config.Size = new System.Drawing.Size(383, 199);
            this.lv_Config.TabIndex = 9;
            this.lv_Config.UseCompatibleStateImageBehavior = false;
            this.lv_Config.View = System.Windows.Forms.View.Details;
            this.lv_Config.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.lv_Config_ItemCheck);
            this.lv_Config.SelectedIndexChanged += new System.EventHandler(this.lv_Config_SelectedIndexChanged);
            // 
            // colh_Entry
            // 
            this.colh_Entry.Text = "Channel/Path";
            this.colh_Entry.Width = 379;
            // 
            // btn_AddConfig
            // 
            this.btn_AddConfig.Location = new System.Drawing.Point(800, 38);
            this.btn_AddConfig.Name = "btn_AddConfig";
            this.btn_AddConfig.Size = new System.Drawing.Size(87, 30);
            this.btn_AddConfig.TabIndex = 10;
            this.btn_AddConfig.Text = "Add";
            this.btn_AddConfig.UseVisualStyleBackColor = true;
            this.btn_AddConfig.Click += new System.EventHandler(this.btn_AddConfig_Click);
            // 
            // nup_TimeInterval
            // 
            this.nup_TimeInterval.Location = new System.Drawing.Point(219, 254);
            this.nup_TimeInterval.Maximum = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.nup_TimeInterval.Minimum = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.nup_TimeInterval.Name = "nup_TimeInterval";
            this.nup_TimeInterval.Size = new System.Drawing.Size(87, 22);
            this.nup_TimeInterval.TabIndex = 14;
            this.nup_TimeInterval.Value = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.nup_TimeInterval.ValueChanged += new System.EventHandler(this.nup_TimeInterval_ValueChanged);
            // 
            // btn_Remove
            // 
            this.btn_Remove.Location = new System.Drawing.Point(800, 153);
            this.btn_Remove.Name = "btn_Remove";
            this.btn_Remove.Size = new System.Drawing.Size(87, 30);
            this.btn_Remove.TabIndex = 16;
            this.btn_Remove.Text = "Remove";
            this.btn_Remove.UseVisualStyleBackColor = true;
            this.btn_Remove.Click += new System.EventHandler(this.btn_Remove_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 254);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(201, 17);
            this.label2.TabIndex = 17;
            this.label2.Text = "Check Time Intervals (Second)";
            // 
            // lv_Detail
            // 
            this.lv_Detail.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.lv_Detail.Location = new System.Drawing.Point(404, 27);
            this.lv_Detail.Name = "lv_Detail";
            this.lv_Detail.Size = new System.Drawing.Size(390, 200);
            this.lv_Detail.TabIndex = 18;
            this.lv_Detail.UseCompatibleStateImageBehavior = false;
            this.lv_Detail.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Parameter";
            this.columnHeader1.Width = 127;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Value";
            this.columnHeader2.Width = 600;
            // 
            // cb_SelectAll
            // 
            this.cb_SelectAll.AutoSize = true;
            this.cb_SelectAll.Location = new System.Drawing.Point(12, 230);
            this.cb_SelectAll.Name = "cb_SelectAll";
            this.cb_SelectAll.Size = new System.Drawing.Size(88, 21);
            this.cb_SelectAll.TabIndex = 22;
            this.cb_SelectAll.Text = "Select All";
            this.cb_SelectAll.UseVisualStyleBackColor = true;
            this.cb_SelectAll.CheckedChanged += new System.EventHandler(this.cb_SelectAll_CheckedChanged);
            // 
            // btn_ClearAll
            // 
            this.btn_ClearAll.Location = new System.Drawing.Point(800, 189);
            this.btn_ClearAll.Name = "btn_ClearAll";
            this.btn_ClearAll.Size = new System.Drawing.Size(87, 30);
            this.btn_ClearAll.TabIndex = 23;
            this.btn_ClearAll.Text = "Clear All";
            this.btn_ClearAll.UseVisualStyleBackColor = true;
            this.btn_ClearAll.Click += new System.EventHandler(this.btn_ClearAll_Click);
            // 
            // timer1
            // 
            this.timer1.Interval = 500;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // menuStrip1
            // 
            this.menuStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(892, 26);
            this.menuStrip1.TabIndex = 24;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(58, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // frm_Main
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.ClientSize = new System.Drawing.Size(892, 639);
            this.Controls.Add(this.btn_ClearAll);
            this.Controls.Add(this.cb_SelectAll);
            this.Controls.Add(this.lv_Detail);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btn_Remove);
            this.Controls.Add(this.nup_TimeInterval);
            this.Controls.Add(this.btn_AddConfig);
            this.Controls.Add(this.lv_Config);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_Config);
            this.Controls.Add(this.rtb_Result);
            this.Controls.Add(this.btn_Stop);
            this.Controls.Add(this.btn_Exit);
            this.Controls.Add(this.btn_Clear);
            this.Controls.Add(this.btn_Start);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(900, 671);
            this.MinimumSize = new System.Drawing.Size(900, 671);
            this.Name = "frm_Main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Founder Distributing Telegraph Software Reuter";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmMain_FormClosed);
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.frm_Main_KeyPress);
            ((System.ComponentModel.ISupportInitialize)(this.nup_TimeInterval)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_Start;
        private System.Windows.Forms.Button btn_Clear;
        private System.Windows.Forms.Button btn_Exit;
        private System.Windows.Forms.Button btn_Stop;
        private System.Windows.Forms.RichTextBox rtb_Result;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btn_Config;
        private System.Windows.Forms.ListView lv_Config;
        private System.Windows.Forms.ColumnHeader colh_Entry;
        private System.Windows.Forms.Button btn_AddConfig;
        private System.Windows.Forms.NumericUpDown nup_TimeInterval;
        private System.Windows.Forms.Button btn_Remove;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ListView lv_Detail;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.CheckBox cb_SelectAll;
        private System.Windows.Forms.Button btn_ClearAll;
        private System.ComponentModel.BackgroundWorker bgWorker;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;

    }
}