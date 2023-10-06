namespace Reuters_with_Web_Service
{
    partial class frmRestart
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
            this.ckb_SelAll = new System.Windows.Forms.CheckBox();
            this.listWindows = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btn_OK
            // 
            this.btn_OK.Location = new System.Drawing.Point(248, 58);
            this.btn_OK.Name = "btn_OK";
            this.btn_OK.Size = new System.Drawing.Size(75, 34);
            this.btn_OK.TabIndex = 0;
            this.btn_OK.Text = "OK";
            this.btn_OK.UseVisualStyleBackColor = true;
            // 
            // btn_Cancel
            // 
            this.btn_Cancel.Location = new System.Drawing.Point(248, 114);
            this.btn_Cancel.Name = "btn_Cancel";
            this.btn_Cancel.Size = new System.Drawing.Size(75, 34);
            this.btn_Cancel.TabIndex = 1;
            this.btn_Cancel.Text = "Cancel";
            this.btn_Cancel.UseVisualStyleBackColor = true;
            this.btn_Cancel.Click += new System.EventHandler(this.btn_Cancel_Click);
            // 
            // ckb_SelAll
            // 
            this.ckb_SelAll.AutoSize = true;
            this.ckb_SelAll.Location = new System.Drawing.Point(235, 174);
            this.ckb_SelAll.Name = "ckb_SelAll";
            this.ckb_SelAll.Size = new System.Drawing.Size(88, 21);
            this.ckb_SelAll.TabIndex = 2;
            this.ckb_SelAll.Text = "Select All";
            this.ckb_SelAll.UseVisualStyleBackColor = true;
            // 
            // listWindows
            // 
            this.listWindows.FormattingEnabled = true;
            this.listWindows.ItemHeight = 16;
            this.listWindows.Location = new System.Drawing.Point(13, 51);
            this.listWindows.Name = "listWindows";
            this.listWindows.Size = new System.Drawing.Size(216, 164);
            this.listWindows.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(144, 17);
            this.label1.TabIndex = 4;
            this.label1.Text = "Select Watch Window";
            // 
            // frmRestart
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(337, 228);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.listWindows);
            this.Controls.Add(this.ckb_SelAll);
            this.Controls.Add(this.btn_Cancel);
            this.Controls.Add(this.btn_OK);
            this.Name = "frmRestart";
            this.Text = "Select Watch Window";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_OK;
        private System.Windows.Forms.Button btn_Cancel;
        private System.Windows.Forms.CheckBox ckb_SelAll;
        private System.Windows.Forms.ListBox listWindows;
        private System.Windows.Forms.Label label1;
    }
}