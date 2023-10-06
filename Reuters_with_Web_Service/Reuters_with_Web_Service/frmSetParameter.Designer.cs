namespace Reuters_with_Web_Service
{
    partial class frmSetParameter
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
            this.btn_OK = new System.Windows.Forms.Button();
            this.btn_Cancel = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.cb_DBTable = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.cb_ChannelOrPath = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.gb_DataFromFile = new System.Windows.Forms.GroupBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.tb_FMiddlePicPath = new System.Windows.Forms.TextBox();
            this.tb_FSmallPicPath = new System.Windows.Forms.TextBox();
            this.tb_FSourcePicPath = new System.Windows.Forms.TextBox();
            this.ckb_DeleteFile = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tb_FileCopyPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.gb_DataFromReuters = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tb_RMiddlePicPath = new System.Windows.Forms.TextBox();
            this.tb_RSmallPicPath = new System.Windows.Forms.TextBox();
            this.ckb_DownloadXML = new System.Windows.Forms.CheckBox();
            this.tb_RSourcePicPath = new System.Windows.Forms.TextBox();
            this.tb_XMLDownLoadPath = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.cb_SourceRelativePath = new System.Windows.Forms.ComboBox();
            this.cb_SmallRelativePath = new System.Windows.Forms.ComboBox();
            this.cb_MiddleRelativePath = new System.Windows.Forms.ComboBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.tb_WiresCode = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.tb_FileCode = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.gb_NamingSetting = new System.Windows.Forms.GroupBox();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.nup_Last = new System.Windows.Forms.NumericUpDown();
            this.nup_First = new System.Windows.Forms.NumericUpDown();
            this.gb_FileSpecial = new System.Windows.Forms.GroupBox();
            this.btn_Remove = new System.Windows.Forms.Button();
            this.btn_Config = new System.Windows.Forms.Button();
            this.btn_Add = new System.Windows.Forms.Button();
            this.lst_FileSpecMapping = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.gb_Priority = new System.Windows.Forms.GroupBox();
            this.btn_PRemove = new System.Windows.Forms.Button();
            this.btn_PConfig = new System.Windows.Forms.Button();
            this.btn_PAdd = new System.Windows.Forms.Button();
            this.lst_PriorityMapping = new System.Windows.Forms.ListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.cb_XMLType = new System.Windows.Forms.ComboBox();
            this.label23 = new System.Windows.Forms.Label();
            this.gb_BBIgnoreFile = new System.Windows.Forms.GroupBox();
            this.tb_ignoreStr = new System.Windows.Forms.TextBox();
            this.lbl_ignoreStr = new System.Windows.Forms.Label();
            this.lbl_movePath = new System.Windows.Forms.Label();
            this.tb_movePath = new System.Windows.Forms.TextBox();
            this.rb_moveToDir = new System.Windows.Forms.RadioButton();
            this.rb_IFDel = new System.Windows.Forms.RadioButton();
            this.cb_ignoreFile = new System.Windows.Forms.CheckBox();
            this.label24 = new System.Windows.Forms.Label();
            this.ckb_ProcessTime = new System.Windows.Forms.CheckBox();
            this.dtp_Buffer = new System.Windows.Forms.DateTimePicker();
            this.gb_DataFromFile.SuspendLayout();
            this.gb_DataFromReuters.SuspendLayout();
            this.gb_NamingSetting.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Last)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_First)).BeginInit();
            this.gb_FileSpecial.SuspendLayout();
            this.gb_Priority.SuspendLayout();
            this.gb_BBIgnoreFile.SuspendLayout();
            this.SuspendLayout();
            // 
            // btn_OK
            // 
            this.btn_OK.Location = new System.Drawing.Point(87, 725);
            this.btn_OK.Name = "btn_OK";
            this.btn_OK.Size = new System.Drawing.Size(75, 30);
            this.btn_OK.TabIndex = 0;
            this.btn_OK.Text = "OK";
            this.btn_OK.UseVisualStyleBackColor = true;
            this.btn_OK.Click += new System.EventHandler(this.btn_OK_Click);
            // 
            // btn_Cancel
            // 
            this.btn_Cancel.Location = new System.Drawing.Point(269, 725);
            this.btn_Cancel.Name = "btn_Cancel";
            this.btn_Cancel.Size = new System.Drawing.Size(75, 30);
            this.btn_Cancel.TabIndex = 1;
            this.btn_Cancel.Text = "Cancel";
            this.btn_Cancel.UseVisualStyleBackColor = true;
            this.btn_Cancel.Click += new System.EventHandler(this.btn_Cancel_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(30, 453);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(0, 12);
            this.label4.TabIndex = 9;
            // 
            // cb_DBTable
            // 
            this.cb_DBTable.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_DBTable.FormattingEnabled = true;
            this.cb_DBTable.Location = new System.Drawing.Point(16, 82);
            this.cb_DBTable.Name = "cb_DBTable";
            this.cb_DBTable.Size = new System.Drawing.Size(186, 20);
            this.cb_DBTable.TabIndex = 2;
            this.cb_DBTable.SelectedIndexChanged += new System.EventHandler(this.cb_DBTable_SelectedIndexChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(16, 58);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(81, 12);
            this.label9.TabIndex = 17;
            this.label9.Text = "Default Agency:";
            // 
            // cb_ChannelOrPath
            // 
            this.cb_ChannelOrPath.FormattingEnabled = true;
            this.cb_ChannelOrPath.Location = new System.Drawing.Point(16, 31);
            this.cb_ChannelOrPath.Name = "cb_ChannelOrPath";
            this.cb_ChannelOrPath.Size = new System.Drawing.Size(369, 20);
            this.cb_ChannelOrPath.TabIndex = 1;
            this.cb_ChannelOrPath.Leave += new System.EventHandler(this.cb_ChannelOrPath_Leave);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(16, 6);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(77, 12);
            this.label10.TabIndex = 19;
            this.label10.Text = "Channels / Path";
            // 
            // gb_DataFromFile
            // 
            this.gb_DataFromFile.Controls.Add(this.label11);
            this.gb_DataFromFile.Controls.Add(this.label12);
            this.gb_DataFromFile.Controls.Add(this.label13);
            this.gb_DataFromFile.Controls.Add(this.tb_FMiddlePicPath);
            this.gb_DataFromFile.Controls.Add(this.tb_FSmallPicPath);
            this.gb_DataFromFile.Controls.Add(this.tb_FSourcePicPath);
            this.gb_DataFromFile.Controls.Add(this.ckb_DeleteFile);
            this.gb_DataFromFile.Controls.Add(this.label1);
            this.gb_DataFromFile.Controls.Add(this.tb_FileCopyPath);
            this.gb_DataFromFile.Controls.Add(this.label2);
            this.gb_DataFromFile.Controls.Add(this.gb_DataFromReuters);
            this.gb_DataFromFile.Location = new System.Drawing.Point(16, 376);
            this.gb_DataFromFile.Name = "gb_DataFromFile";
            this.gb_DataFromFile.Size = new System.Drawing.Size(408, 202);
            this.gb_DataFromFile.TabIndex = 24;
            this.gb_DataFromFile.TabStop = false;
            this.gb_DataFromFile.Text = "Data From File Setting";
            this.gb_DataFromFile.Visible = false;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(2, 145);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(76, 12);
            this.label11.TabIndex = 43;
            this.label11.Text = "Middle Picture:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(2, 173);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(69, 12);
            this.label12.TabIndex = 42;
            this.label12.Text = "Small Picture:";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(2, 119);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(75, 12);
            this.label13.TabIndex = 41;
            this.label13.Text = "Source Picture:";
            // 
            // tb_FMiddlePicPath
            // 
            this.tb_FMiddlePicPath.Location = new System.Drawing.Point(110, 145);
            this.tb_FMiddlePicPath.Name = "tb_FMiddlePicPath";
            this.tb_FMiddlePicPath.Size = new System.Drawing.Size(287, 22);
            this.tb_FMiddlePicPath.TabIndex = 9;
            // 
            // tb_FSmallPicPath
            // 
            this.tb_FSmallPicPath.Location = new System.Drawing.Point(110, 173);
            this.tb_FSmallPicPath.Name = "tb_FSmallPicPath";
            this.tb_FSmallPicPath.Size = new System.Drawing.Size(287, 22);
            this.tb_FSmallPicPath.TabIndex = 10;
            // 
            // tb_FSourcePicPath
            // 
            this.tb_FSourcePicPath.Location = new System.Drawing.Point(110, 117);
            this.tb_FSourcePicPath.Name = "tb_FSourcePicPath";
            this.tb_FSourcePicPath.Size = new System.Drawing.Size(287, 22);
            this.tb_FSourcePicPath.TabIndex = 8;
            // 
            // ckb_DeleteFile
            // 
            this.ckb_DeleteFile.AutoSize = true;
            this.ckb_DeleteFile.Location = new System.Drawing.Point(7, 71);
            this.ckb_DeleteFile.Name = "ckb_DeleteFile";
            this.ckb_DeleteFile.Size = new System.Drawing.Size(145, 16);
            this.ckb_DeleteFile.TabIndex = 7;
            this.ckb_DeleteFile.Text = "Delete File after processed";
            this.ckb_DeleteFile.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(2, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 23;
            this.label1.Text = "File Copy Path:";
            // 
            // tb_FileCopyPath
            // 
            this.tb_FileCopyPath.Location = new System.Drawing.Point(7, 42);
            this.tb_FileCopyPath.Name = "tb_FileCopyPath";
            this.tb_FileCopyPath.Size = new System.Drawing.Size(386, 22);
            this.tb_FileCopyPath.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(2, 96);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(87, 12);
            this.label2.TabIndex = 25;
            this.label2.Text = "Photo Copy Path:";
            // 
            // gb_DataFromReuters
            // 
            this.gb_DataFromReuters.Controls.Add(this.label8);
            this.gb_DataFromReuters.Controls.Add(this.label7);
            this.gb_DataFromReuters.Controls.Add(this.label6);
            this.gb_DataFromReuters.Controls.Add(this.tb_RMiddlePicPath);
            this.gb_DataFromReuters.Controls.Add(this.tb_RSmallPicPath);
            this.gb_DataFromReuters.Controls.Add(this.ckb_DownloadXML);
            this.gb_DataFromReuters.Controls.Add(this.tb_RSourcePicPath);
            this.gb_DataFromReuters.Controls.Add(this.tb_XMLDownLoadPath);
            this.gb_DataFromReuters.Controls.Add(this.label3);
            this.gb_DataFromReuters.Controls.Add(this.label5);
            this.gb_DataFromReuters.Location = new System.Drawing.Point(0, 0);
            this.gb_DataFromReuters.Name = "gb_DataFromReuters";
            this.gb_DataFromReuters.Size = new System.Drawing.Size(408, 201);
            this.gb_DataFromReuters.TabIndex = 25;
            this.gb_DataFromReuters.TabStop = false;
            this.gb_DataFromReuters.Text = "Data Form Reuter Setting";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(4, 147);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(76, 12);
            this.label8.TabIndex = 37;
            this.label8.Text = "Middle Picture:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(4, 175);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(69, 12);
            this.label7.TabIndex = 36;
            this.label7.Text = "Small Picture:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(4, 121);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(75, 12);
            this.label6.TabIndex = 35;
            this.label6.Text = "Source Picture:";
            // 
            // tb_RMiddlePicPath
            // 
            this.tb_RMiddlePicPath.Location = new System.Drawing.Point(112, 147);
            this.tb_RMiddlePicPath.Name = "tb_RMiddlePicPath";
            this.tb_RMiddlePicPath.Size = new System.Drawing.Size(287, 22);
            this.tb_RMiddlePicPath.TabIndex = 34;
            // 
            // tb_RSmallPicPath
            // 
            this.tb_RSmallPicPath.Location = new System.Drawing.Point(112, 175);
            this.tb_RSmallPicPath.Name = "tb_RSmallPicPath";
            this.tb_RSmallPicPath.Size = new System.Drawing.Size(287, 22);
            this.tb_RSmallPicPath.TabIndex = 33;
            // 
            // ckb_DownloadXML
            // 
            this.ckb_DownloadXML.AutoSize = true;
            this.ckb_DownloadXML.Location = new System.Drawing.Point(9, 21);
            this.ckb_DownloadXML.Name = "ckb_DownloadXML";
            this.ckb_DownloadXML.Size = new System.Drawing.Size(232, 16);
            this.ckb_DownloadXML.TabIndex = 31;
            this.ckb_DownloadXML.Text = "Download XML File After Record Processed";
            this.ckb_DownloadXML.UseVisualStyleBackColor = true;
            this.ckb_DownloadXML.CheckedChanged += new System.EventHandler(this.ckb_DownloadXML_CheckedChanged);
            // 
            // tb_RSourcePicPath
            // 
            this.tb_RSourcePicPath.Location = new System.Drawing.Point(112, 119);
            this.tb_RSourcePicPath.Name = "tb_RSourcePicPath";
            this.tb_RSourcePicPath.Size = new System.Drawing.Size(287, 22);
            this.tb_RSourcePicPath.TabIndex = 30;
            // 
            // tb_XMLDownLoadPath
            // 
            this.tb_XMLDownLoadPath.Location = new System.Drawing.Point(9, 67);
            this.tb_XMLDownLoadPath.Name = "tb_XMLDownLoadPath";
            this.tb_XMLDownLoadPath.Size = new System.Drawing.Size(390, 22);
            this.tb_XMLDownLoadPath.TabIndex = 27;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(109, 12);
            this.label3.TabIndex = 29;
            this.label3.Text = "Photo Download Path:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(4, 47);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(99, 12);
            this.label5.TabIndex = 28;
            this.label5.Text = "File Download Path:";
            // 
            // cb_SourceRelativePath
            // 
            this.cb_SourceRelativePath.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_SourceRelativePath.FormattingEnabled = true;
            this.cb_SourceRelativePath.Location = new System.Drawing.Point(101, 493);
            this.cb_SourceRelativePath.Name = "cb_SourceRelativePath";
            this.cb_SourceRelativePath.Size = new System.Drawing.Size(111, 20);
            this.cb_SourceRelativePath.TabIndex = 38;
            this.cb_SourceRelativePath.Visible = false;
            // 
            // cb_SmallRelativePath
            // 
            this.cb_SmallRelativePath.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_SmallRelativePath.FormattingEnabled = true;
            this.cb_SmallRelativePath.Location = new System.Drawing.Point(100, 549);
            this.cb_SmallRelativePath.Name = "cb_SmallRelativePath";
            this.cb_SmallRelativePath.Size = new System.Drawing.Size(111, 20);
            this.cb_SmallRelativePath.TabIndex = 40;
            this.cb_SmallRelativePath.Visible = false;
            // 
            // cb_MiddleRelativePath
            // 
            this.cb_MiddleRelativePath.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_MiddleRelativePath.FormattingEnabled = true;
            this.cb_MiddleRelativePath.Location = new System.Drawing.Point(100, 521);
            this.cb_MiddleRelativePath.Name = "cb_MiddleRelativePath";
            this.cb_MiddleRelativePath.Size = new System.Drawing.Size(111, 20);
            this.cb_MiddleRelativePath.TabIndex = 39;
            this.cb_MiddleRelativePath.Visible = false;
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Checked = true;
            this.checkBox3.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox3.Location = new System.Drawing.Point(174, 72);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(180, 16);
            this.checkBox3.TabIndex = 26;
            this.checkBox3.Text = "Data From Reuters (For test only)";
            this.checkBox3.UseVisualStyleBackColor = true;
            this.checkBox3.Visible = false;
            this.checkBox3.CheckedChanged += new System.EventHandler(this.checkBox3_CheckedChanged);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(16, 337);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(300, 12);
            this.label14.TabIndex = 27;
            this.label14.Text = "It is able to add Date by using the following key words on path:";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(17, 355);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(173, 12);
            this.label15.TabIndex = 28;
            this.label15.Text = "%Y = year, %M = month, %D = day";
            // 
            // tb_WiresCode
            // 
            this.tb_WiresCode.Location = new System.Drawing.Point(16, 83);
            this.tb_WiresCode.Name = "tb_WiresCode";
            this.tb_WiresCode.Size = new System.Drawing.Size(186, 22);
            this.tb_WiresCode.TabIndex = 29;
            this.tb_WiresCode.Visible = false;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(238, 58);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(50, 12);
            this.label16.TabIndex = 30;
            this.label16.Text = "File Code";
            this.label16.Visible = false;
            // 
            // tb_FileCode
            // 
            this.tb_FileCode.Location = new System.Drawing.Point(239, 83);
            this.tb_FileCode.Name = "tb_FileCode";
            this.tb_FileCode.Size = new System.Drawing.Size(129, 22);
            this.tb_FileCode.TabIndex = 31;
            this.tb_FileCode.Visible = false;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(16, 112);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(186, 12);
            this.label17.TabIndex = 33;
            this.label17.Text = "Specify the new type to table mapping:";
            // 
            // gb_NamingSetting
            // 
            this.gb_NamingSetting.Controls.Add(this.label22);
            this.gb_NamingSetting.Controls.Add(this.label21);
            this.gb_NamingSetting.Controls.Add(this.label20);
            this.gb_NamingSetting.Controls.Add(this.label19);
            this.gb_NamingSetting.Controls.Add(this.label18);
            this.gb_NamingSetting.Controls.Add(this.nup_Last);
            this.gb_NamingSetting.Controls.Add(this.nup_First);
            this.gb_NamingSetting.Location = new System.Drawing.Point(16, 273);
            this.gb_NamingSetting.Name = "gb_NamingSetting";
            this.gb_NamingSetting.Size = new System.Drawing.Size(424, 61);
            this.gb_NamingSetting.TabIndex = 39;
            this.gb_NamingSetting.TabStop = false;
            this.gb_NamingSetting.Text = "Wires File Naming Setting";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(9, 35);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(44, 12);
            this.label22.TabIndex = 45;
            this.label22.Text = "Remove";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(364, 35);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(37, 12);
            this.label21.TabIndex = 44;
            this.label21.Text = "char(s)";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(180, 35);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(37, 12);
            this.label20.TabIndex = 43;
            this.label20.Text = "char(s)";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(248, 35);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(27, 12);
            this.label19.TabIndex = 42;
            this.label19.Text = "Last:";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(68, 35);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(28, 12);
            this.label18.TabIndex = 41;
            this.label18.Text = "First:";
            // 
            // nup_Last
            // 
            this.nup_Last.Location = new System.Drawing.Point(293, 33);
            this.nup_Last.Name = "nup_Last";
            this.nup_Last.Size = new System.Drawing.Size(69, 22);
            this.nup_Last.TabIndex = 40;
            // 
            // nup_First
            // 
            this.nup_First.Location = new System.Drawing.Point(113, 33);
            this.nup_First.Name = "nup_First";
            this.nup_First.Size = new System.Drawing.Size(61, 22);
            this.nup_First.TabIndex = 39;
            // 
            // gb_FileSpecial
            // 
            this.gb_FileSpecial.Controls.Add(this.btn_Remove);
            this.gb_FileSpecial.Controls.Add(this.btn_Config);
            this.gb_FileSpecial.Controls.Add(this.btn_Add);
            this.gb_FileSpecial.Controls.Add(this.lst_FileSpecMapping);
            this.gb_FileSpecial.Location = new System.Drawing.Point(16, 106);
            this.gb_FileSpecial.Name = "gb_FileSpecial";
            this.gb_FileSpecial.Size = new System.Drawing.Size(433, 163);
            this.gb_FileSpecial.TabIndex = 40;
            this.gb_FileSpecial.TabStop = false;
            this.gb_FileSpecial.Text = "Specify the news type to table mapping:";
            // 
            // btn_Remove
            // 
            this.btn_Remove.Location = new System.Drawing.Point(350, 119);
            this.btn_Remove.Name = "btn_Remove";
            this.btn_Remove.Size = new System.Drawing.Size(75, 30);
            this.btn_Remove.TabIndex = 37;
            this.btn_Remove.Text = "Remove";
            this.btn_Remove.UseVisualStyleBackColor = true;
            this.btn_Remove.Click += new System.EventHandler(this.btn_Remove_Click);
            // 
            // btn_Config
            // 
            this.btn_Config.Location = new System.Drawing.Point(351, 71);
            this.btn_Config.Name = "btn_Config";
            this.btn_Config.Size = new System.Drawing.Size(75, 30);
            this.btn_Config.TabIndex = 36;
            this.btn_Config.Text = "Config";
            this.btn_Config.UseVisualStyleBackColor = true;
            this.btn_Config.Click += new System.EventHandler(this.btn_Config_Click);
            // 
            // btn_Add
            // 
            this.btn_Add.Location = new System.Drawing.Point(350, 22);
            this.btn_Add.Name = "btn_Add";
            this.btn_Add.Size = new System.Drawing.Size(75, 30);
            this.btn_Add.TabIndex = 35;
            this.btn_Add.Text = "Add";
            this.btn_Add.UseVisualStyleBackColor = true;
            this.btn_Add.Click += new System.EventHandler(this.btn_Add_Click);
            // 
            // lst_FileSpecMapping
            // 
            this.lst_FileSpecMapping.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.lst_FileSpecMapping.FullRowSelect = true;
            this.lst_FileSpecMapping.Location = new System.Drawing.Point(5, 23);
            this.lst_FileSpecMapping.MultiSelect = false;
            this.lst_FileSpecMapping.Name = "lst_FileSpecMapping";
            this.lst_FileSpecMapping.Size = new System.Drawing.Size(328, 134);
            this.lst_FileSpecMapping.TabIndex = 38;
            this.lst_FileSpecMapping.UseCompatibleStateImageBehavior = false;
            this.lst_FileSpecMapping.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "File Special";
            this.columnHeader1.Width = 100;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Agency Name";
            this.columnHeader2.Width = 200;
            // 
            // gb_Priority
            // 
            this.gb_Priority.Controls.Add(this.btn_PRemove);
            this.gb_Priority.Controls.Add(this.btn_PConfig);
            this.gb_Priority.Controls.Add(this.btn_PAdd);
            this.gb_Priority.Controls.Add(this.lst_PriorityMapping);
            this.gb_Priority.Location = new System.Drawing.Point(16, 269);
            this.gb_Priority.Name = "gb_Priority";
            this.gb_Priority.Size = new System.Drawing.Size(433, 163);
            this.gb_Priority.TabIndex = 41;
            this.gb_Priority.TabStop = false;
            this.gb_Priority.Text = "Specify the priority mapping:";
            this.gb_Priority.Visible = false;
            // 
            // btn_PRemove
            // 
            this.btn_PRemove.Location = new System.Drawing.Point(350, 119);
            this.btn_PRemove.Name = "btn_PRemove";
            this.btn_PRemove.Size = new System.Drawing.Size(75, 30);
            this.btn_PRemove.TabIndex = 37;
            this.btn_PRemove.Text = "Remove";
            this.btn_PRemove.UseVisualStyleBackColor = true;
            this.btn_PRemove.Click += new System.EventHandler(this.btn_PRemove_Click);
            // 
            // btn_PConfig
            // 
            this.btn_PConfig.Location = new System.Drawing.Point(351, 71);
            this.btn_PConfig.Name = "btn_PConfig";
            this.btn_PConfig.Size = new System.Drawing.Size(75, 30);
            this.btn_PConfig.TabIndex = 36;
            this.btn_PConfig.Text = "Config";
            this.btn_PConfig.UseVisualStyleBackColor = true;
            this.btn_PConfig.Click += new System.EventHandler(this.btn_PConfig_Click);
            // 
            // btn_PAdd
            // 
            this.btn_PAdd.Location = new System.Drawing.Point(350, 22);
            this.btn_PAdd.Name = "btn_PAdd";
            this.btn_PAdd.Size = new System.Drawing.Size(75, 30);
            this.btn_PAdd.TabIndex = 35;
            this.btn_PAdd.Text = "Add";
            this.btn_PAdd.UseVisualStyleBackColor = true;
            this.btn_PAdd.Click += new System.EventHandler(this.btn_PAdd_Click);
            // 
            // lst_PriorityMapping
            // 
            this.lst_PriorityMapping.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
            this.lst_PriorityMapping.FullRowSelect = true;
            this.lst_PriorityMapping.Location = new System.Drawing.Point(5, 23);
            this.lst_PriorityMapping.MultiSelect = false;
            this.lst_PriorityMapping.Name = "lst_PriorityMapping";
            this.lst_PriorityMapping.Size = new System.Drawing.Size(328, 134);
            this.lst_PriorityMapping.TabIndex = 38;
            this.lst_PriorityMapping.UseCompatibleStateImageBehavior = false;
            this.lst_PriorityMapping.View = System.Windows.Forms.View.Details;
            this.lst_PriorityMapping.DoubleClick += new System.EventHandler(this.lst_PriorityMapping_DoubleClick);
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Tag Value";
            this.columnHeader3.Width = 100;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Priority Value";
            this.columnHeader4.Width = 200;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(87, 726);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 30);
            this.button1.TabIndex = 0;
            this.button1.Text = "OK";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.btn_OK_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(269, 725);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 30);
            this.button2.TabIndex = 1;
            this.button2.Text = "Cancel";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.btn_Cancel_Click);
            // 
            // cb_XMLType
            // 
            this.cb_XMLType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_XMLType.FormattingEnabled = true;
            this.cb_XMLType.Items.AddRange(new object[] {
            "Reuters",
            "AP",
            "Bloomberg",
            "新華社"});
            this.cb_XMLType.Location = new System.Drawing.Point(319, 3);
            this.cb_XMLType.Name = "cb_XMLType";
            this.cb_XMLType.Size = new System.Drawing.Size(121, 20);
            this.cb_XMLType.TabIndex = 43;
            this.cb_XMLType.Visible = false;
            this.cb_XMLType.SelectedIndexChanged += new System.EventHandler(this.cb_XMLType_SelectedIndexChanged);
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(250, 6);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(69, 12);
            this.label23.TabIndex = 44;
            this.label23.Text = "XML Format:";
            // 
            // gb_BBIgnoreFile
            // 
            this.gb_BBIgnoreFile.Controls.Add(this.tb_ignoreStr);
            this.gb_BBIgnoreFile.Controls.Add(this.lbl_ignoreStr);
            this.gb_BBIgnoreFile.Controls.Add(this.lbl_movePath);
            this.gb_BBIgnoreFile.Controls.Add(this.tb_movePath);
            this.gb_BBIgnoreFile.Controls.Add(this.rb_moveToDir);
            this.gb_BBIgnoreFile.Controls.Add(this.rb_IFDel);
            this.gb_BBIgnoreFile.Controls.Add(this.cb_ignoreFile);
            this.gb_BBIgnoreFile.Location = new System.Drawing.Point(16, 585);
            this.gb_BBIgnoreFile.Name = "gb_BBIgnoreFile";
            this.gb_BBIgnoreFile.Size = new System.Drawing.Size(408, 118);
            this.gb_BBIgnoreFile.TabIndex = 45;
            this.gb_BBIgnoreFile.TabStop = false;
            this.gb_BBIgnoreFile.Text = "Bloomberg file handling";
            this.gb_BBIgnoreFile.Visible = false;
            // 
            // tb_ignoreStr
            // 
            this.tb_ignoreStr.Location = new System.Drawing.Point(114, 52);
            this.tb_ignoreStr.Name = "tb_ignoreStr";
            this.tb_ignoreStr.Size = new System.Drawing.Size(283, 22);
            this.tb_ignoreStr.TabIndex = 3;
            // 
            // lbl_ignoreStr
            // 
            this.lbl_ignoreStr.AutoSize = true;
            this.lbl_ignoreStr.Location = new System.Drawing.Point(2, 55);
            this.lbl_ignoreStr.Name = "lbl_ignoreStr";
            this.lbl_ignoreStr.Size = new System.Drawing.Size(99, 12);
            this.lbl_ignoreStr.TabIndex = 5;
            this.lbl_ignoreStr.Text = "Ignore file name(,) :";
            // 
            // lbl_movePath
            // 
            this.lbl_movePath.AutoSize = true;
            this.lbl_movePath.Location = new System.Drawing.Point(2, 88);
            this.lbl_movePath.Name = "lbl_movePath";
            this.lbl_movePath.Size = new System.Drawing.Size(61, 12);
            this.lbl_movePath.TabIndex = 4;
            this.lbl_movePath.Text = "Move Path :";
            // 
            // tb_movePath
            // 
            this.tb_movePath.Enabled = false;
            this.tb_movePath.Location = new System.Drawing.Point(114, 85);
            this.tb_movePath.Name = "tb_movePath";
            this.tb_movePath.Size = new System.Drawing.Size(283, 22);
            this.tb_movePath.TabIndex = 6;
            // 
            // rb_moveToDir
            // 
            this.rb_moveToDir.AutoSize = true;
            this.rb_moveToDir.Enabled = false;
            this.rb_moveToDir.Location = new System.Drawing.Point(204, 19);
            this.rb_moveToDir.Name = "rb_moveToDir";
            this.rb_moveToDir.Size = new System.Drawing.Size(107, 16);
            this.rb_moveToDir.TabIndex = 2;
            this.rb_moveToDir.TabStop = true;
            this.rb_moveToDir.Text = "Move to directory";
            this.rb_moveToDir.UseVisualStyleBackColor = true;
            this.rb_moveToDir.CheckedChanged += new System.EventHandler(this.rb_moveToDir_CheckedChanged);
            // 
            // rb_IFDel
            // 
            this.rb_IFDel.AutoSize = true;
            this.rb_IFDel.Enabled = false;
            this.rb_IFDel.Location = new System.Drawing.Point(113, 19);
            this.rb_IFDel.Name = "rb_IFDel";
            this.rb_IFDel.Size = new System.Drawing.Size(52, 16);
            this.rb_IFDel.TabIndex = 1;
            this.rb_IFDel.TabStop = true;
            this.rb_IFDel.Text = "Delete";
            this.rb_IFDel.UseVisualStyleBackColor = true;
            // 
            // cb_ignoreFile
            // 
            this.cb_ignoreFile.AutoSize = true;
            this.cb_ignoreFile.Location = new System.Drawing.Point(7, 20);
            this.cb_ignoreFile.Name = "cb_ignoreFile";
            this.cb_ignoreFile.Size = new System.Drawing.Size(75, 16);
            this.cb_ignoreFile.TabIndex = 0;
            this.cb_ignoreFile.Text = "Ignore File";
            this.cb_ignoreFile.UseVisualStyleBackColor = true;
            this.cb_ignoreFile.CheckedChanged += new System.EventHandler(this.cb_ignoreFile_CheckedChanged);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(216, 705);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(69, 12);
            this.label24.TabIndex = 7;
            this.label24.Text = "Buffer Time :";
            // 
            // ckb_ProcessTime
            // 
            this.ckb_ProcessTime.AutoSize = true;
            this.ckb_ProcessTime.Location = new System.Drawing.Point(21, 704);
            this.ckb_ProcessTime.Name = "ckb_ProcessTime";
            this.ckb_ProcessTime.Size = new System.Drawing.Size(189, 16);
            this.ckb_ProcessTime.TabIndex = 38;
            this.ckb_ProcessTime.Text = "Use Processing Time as Photo Time";
            this.ckb_ProcessTime.UseVisualStyleBackColor = true;
            // 
            // dtp_Buffer
            // 
            this.dtp_Buffer.CustomFormat = "HH:mm";
            this.dtp_Buffer.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtp_Buffer.Location = new System.Drawing.Point(285, 700);
            this.dtp_Buffer.Name = "dtp_Buffer";
            this.dtp_Buffer.ShowUpDown = true;
            this.dtp_Buffer.Size = new System.Drawing.Size(59, 22);
            this.dtp_Buffer.TabIndex = 46;
            // 
            // frmSetParameter
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(444, 762);
            this.Controls.Add(this.dtp_Buffer);
            this.Controls.Add(this.label24);
            this.Controls.Add(this.ckb_ProcessTime);
            this.Controls.Add(this.gb_BBIgnoreFile);
            this.Controls.Add(this.label23);
            this.Controls.Add(this.cb_XMLType);
            this.Controls.Add(this.cb_SmallRelativePath);
            this.Controls.Add(this.cb_MiddleRelativePath);
            this.Controls.Add(this.cb_SourceRelativePath);
            this.Controls.Add(this.gb_Priority);
            this.Controls.Add(this.gb_FileSpecial);
            this.Controls.Add(this.gb_NamingSetting);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.tb_FileCode);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.tb_WiresCode);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.checkBox3);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.cb_ChannelOrPath);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.cb_DBTable);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.btn_Cancel);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btn_OK);
            this.Controls.Add(this.gb_DataFromFile);
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(460, 800);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(460, 800);
            this.Name = "frmSetParameter";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Parameter Setting";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmSetParameter_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmSetParameter_FormClosed);
            this.Load += new System.EventHandler(this.frmSetParameter_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.frmSetParameter_KeyPress);
            this.gb_DataFromFile.ResumeLayout(false);
            this.gb_DataFromFile.PerformLayout();
            this.gb_DataFromReuters.ResumeLayout(false);
            this.gb_DataFromReuters.PerformLayout();
            this.gb_NamingSetting.ResumeLayout(false);
            this.gb_NamingSetting.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Last)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_First)).EndInit();
            this.gb_FileSpecial.ResumeLayout(false);
            this.gb_Priority.ResumeLayout(false);
            this.gb_BBIgnoreFile.ResumeLayout(false);
            this.gb_BBIgnoreFile.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_OK;
        private System.Windows.Forms.Button btn_Cancel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cb_DBTable;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox cb_ChannelOrPath;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.GroupBox gb_DataFromFile;
        private System.Windows.Forms.CheckBox ckb_DeleteFile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tb_FileCopyPath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox gb_DataFromReuters;
        private System.Windows.Forms.CheckBox ckb_DownloadXML;
        private System.Windows.Forms.TextBox tb_XMLDownLoadPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tb_RMiddlePicPath;
        private System.Windows.Forms.TextBox tb_RSmallPicPath;
        private System.Windows.Forms.TextBox tb_RSourcePicPath;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox tb_FMiddlePicPath;
        private System.Windows.Forms.TextBox tb_FSmallPicPath;
        private System.Windows.Forms.TextBox tb_FSourcePicPath;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox tb_WiresCode;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox tb_FileCode;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.GroupBox gb_NamingSetting;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.NumericUpDown nup_Last;
        private System.Windows.Forms.NumericUpDown nup_First;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.GroupBox gb_FileSpecial;
        private System.Windows.Forms.Button btn_Remove;
        private System.Windows.Forms.Button btn_Config;
        private System.Windows.Forms.Button btn_Add;
        private System.Windows.Forms.ListView lst_FileSpecMapping;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.GroupBox gb_Priority;
        private System.Windows.Forms.Button btn_PRemove;
        private System.Windows.Forms.Button btn_PConfig;
        private System.Windows.Forms.Button btn_PAdd;
        private System.Windows.Forms.ListView lst_PriorityMapping;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ComboBox cb_SmallRelativePath;
        private System.Windows.Forms.ComboBox cb_MiddleRelativePath;
        private System.Windows.Forms.ComboBox cb_SourceRelativePath;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.ComboBox cb_XMLType;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.GroupBox gb_BBIgnoreFile;
        private System.Windows.Forms.Label lbl_movePath;
        private System.Windows.Forms.TextBox tb_movePath;
        private System.Windows.Forms.RadioButton rb_moveToDir;
        private System.Windows.Forms.RadioButton rb_IFDel;
        private System.Windows.Forms.CheckBox cb_ignoreFile;
        private System.Windows.Forms.TextBox tb_ignoreStr;
        private System.Windows.Forms.Label lbl_ignoreStr;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.CheckBox ckb_ProcessTime;
        private System.Windows.Forms.DateTimePicker dtp_Buffer;
    }
}