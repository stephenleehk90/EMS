namespace MyNewService
{
    partial class frmWhitelisting
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
            this.lstv_Path = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.btn_OK = new System.Windows.Forms.Button();
            this.btn_Confirm = new System.Windows.Forms.Button();
            this.tb_Path = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_Del = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lstv_Path
            // 
            this.lstv_Path.CheckBoxes = true;
            this.lstv_Path.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.lstv_Path.Location = new System.Drawing.Point(13, 12);
            this.lstv_Path.Name = "lstv_Path";
            this.lstv_Path.Size = new System.Drawing.Size(297, 237);
            this.lstv_Path.TabIndex = 0;
            this.lstv_Path.UseCompatibleStateImageBehavior = false;
            this.lstv_Path.View = System.Windows.Forms.View.Details;
            this.lstv_Path.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.lstv_Path_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "路徑";
            this.columnHeader1.Width = 600;
            // 
            // btn_OK
            // 
            this.btn_OK.Location = new System.Drawing.Point(235, 295);
            this.btn_OK.Name = "btn_OK";
            this.btn_OK.Size = new System.Drawing.Size(75, 21);
            this.btn_OK.TabIndex = 1;
            this.btn_OK.Text = "關閉";
            this.btn_OK.UseVisualStyleBackColor = true;
            this.btn_OK.Click += new System.EventHandler(this.btn_OK_Click);
            // 
            // btn_Confirm
            // 
            this.btn_Confirm.Location = new System.Drawing.Point(12, 295);
            this.btn_Confirm.Name = "btn_Confirm";
            this.btn_Confirm.Size = new System.Drawing.Size(45, 21);
            this.btn_Confirm.TabIndex = 2;
            this.btn_Confirm.Text = "新增";
            this.btn_Confirm.UseVisualStyleBackColor = true;
            this.btn_Confirm.Click += new System.EventHandler(this.btn_Confirm_Click);
            // 
            // tb_Path
            // 
            this.tb_Path.Location = new System.Drawing.Point(13, 269);
            this.tb_Path.Name = "tb_Path";
            this.tb_Path.Size = new System.Drawing.Size(297, 22);
            this.tb_Path.TabIndex = 3;
            this.tb_Path.TextChanged += new System.EventHandler(this.tb_Path_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 251);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "路徑:";
            // 
            // btn_Del
            // 
            this.btn_Del.Location = new System.Drawing.Point(83, 295);
            this.btn_Del.Name = "btn_Del";
            this.btn_Del.Size = new System.Drawing.Size(45, 21);
            this.btn_Del.TabIndex = 5;
            this.btn_Del.Text = "刪除";
            this.btn_Del.UseVisualStyleBackColor = true;
            this.btn_Del.Click += new System.EventHandler(this.btn_Del_Click);
            // 
            // frmWhitelisting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(322, 328);
            this.Controls.Add(this.btn_Del);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tb_Path);
            this.Controls.Add(this.btn_Confirm);
            this.Controls.Add(this.btn_OK);
            this.Controls.Add(this.lstv_Path);
            this.Name = "frmWhitelisting";
            this.ShowIcon = false;
            this.Text = "略過的文件夾";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmWhitelisting_FormClosing);
            this.Load += new System.EventHandler(this.frmWhitelisting_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView lstv_Path;
        private System.Windows.Forms.Button btn_OK;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Button btn_Confirm;
        private System.Windows.Forms.TextBox tb_Path;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btn_Del;
    }
}