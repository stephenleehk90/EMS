namespace Reuters_with_Web_Service
{
    partial class frm_FileSpecial
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
            this.cb_AgencyTable = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_OK = new System.Windows.Forms.Button();
            this.btn_Cancal = new System.Windows.Forms.Button();
            this.tb_FileSpec = new System.Windows.Forms.TextBox();
            this.cb_Unique = new System.Windows.Forms.CheckBox();
            this.tb_ClassName = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // cb_AgencyTable
            // 
            this.cb_AgencyTable.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_AgencyTable.FormattingEnabled = true;
            this.cb_AgencyTable.Location = new System.Drawing.Point(116, 76);
            this.cb_AgencyTable.Name = "cb_AgencyTable";
            this.cb_AgencyTable.Size = new System.Drawing.Size(187, 24);
            this.cb_AgencyTable.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 17);
            this.label1.TabIndex = 2;
            this.label1.Text = "File Special:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 76);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(99, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Agency Table:";
            // 
            // btn_OK
            // 
            this.btn_OK.Location = new System.Drawing.Point(66, 143);
            this.btn_OK.Name = "btn_OK";
            this.btn_OK.Size = new System.Drawing.Size(75, 30);
            this.btn_OK.TabIndex = 5;
            this.btn_OK.Text = "OK";
            this.btn_OK.UseVisualStyleBackColor = true;
            this.btn_OK.Click += new System.EventHandler(this.btn_OK_Click);
            // 
            // btn_Cancal
            // 
            this.btn_Cancal.Location = new System.Drawing.Point(176, 143);
            this.btn_Cancal.Name = "btn_Cancal";
            this.btn_Cancal.Size = new System.Drawing.Size(75, 30);
            this.btn_Cancal.TabIndex = 6;
            this.btn_Cancal.Text = "Cancel";
            this.btn_Cancal.UseVisualStyleBackColor = true;
            this.btn_Cancal.Click += new System.EventHandler(this.btn_Cancal_Click);
            // 
            // tb_FileSpec
            // 
            this.tb_FileSpec.Location = new System.Drawing.Point(116, 23);
            this.tb_FileSpec.Name = "tb_FileSpec";
            this.tb_FileSpec.Size = new System.Drawing.Size(187, 22);
            this.tb_FileSpec.TabIndex = 7;
            // 
            // cb_Unique
            // 
            this.cb_Unique.AutoSize = true;
            this.cb_Unique.Location = new System.Drawing.Point(10, 116);
            this.cb_Unique.Name = "cb_Unique";
            this.cb_Unique.Size = new System.Drawing.Size(202, 21);
            this.cb_Unique.TabIndex = 8;
            this.cb_Unique.Text = "Unique File Special Agency";
            this.cb_Unique.UseVisualStyleBackColor = true;
            // 
            // tb_ClassName
            // 
            this.tb_ClassName.Location = new System.Drawing.Point(116, 76);
            this.tb_ClassName.Name = "tb_ClassName";
            this.tb_ClassName.Size = new System.Drawing.Size(187, 22);
            this.tb_ClassName.TabIndex = 9;
            this.tb_ClassName.Visible = false;
            // 
            // frm_FileSpecial
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(315, 188);
            this.Controls.Add(this.tb_ClassName);
            this.Controls.Add(this.cb_Unique);
            this.Controls.Add(this.tb_FileSpec);
            this.Controls.Add(this.btn_Cancal);
            this.Controls.Add(this.btn_OK);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cb_AgencyTable);
            this.MaximumSize = new System.Drawing.Size(323, 220);
            this.MinimumSize = new System.Drawing.Size(323, 220);
            this.Name = "frm_FileSpecial";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "File Special";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frm_FileSpecial_FormClosed);
            this.Load += new System.EventHandler(this.frm_FileSpecial_Load);
            this.Shown += new System.EventHandler(this.frm_FileSpecial_Shown);
            this.Leave += new System.EventHandler(this.frm_FileSpecial_Leave);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cb_AgencyTable;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btn_OK;
        private System.Windows.Forms.Button btn_Cancal;
        private System.Windows.Forms.TextBox tb_FileSpec;
        private System.Windows.Forms.CheckBox cb_Unique;
        private System.Windows.Forms.TextBox tb_ClassName;
    }
}