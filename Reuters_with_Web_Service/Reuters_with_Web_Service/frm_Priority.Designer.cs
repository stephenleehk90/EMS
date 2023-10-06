namespace Reuters_with_Web_Service
{
    partial class frm_Priority
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
            this.btn_Cancal = new System.Windows.Forms.Button();
            this.btn_OK = new System.Windows.Forms.Button();
            this.cb_DefaultValue = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.nup_Priority = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Priority)).BeginInit();
            this.SuspendLayout();
            // 
            // btn_Cancal
            // 
            this.btn_Cancal.Location = new System.Drawing.Point(146, 119);
            this.btn_Cancal.Name = "btn_Cancal";
            this.btn_Cancal.Size = new System.Drawing.Size(75, 30);
            this.btn_Cancal.TabIndex = 8;
            this.btn_Cancal.Text = "Cancel";
            this.btn_Cancal.UseVisualStyleBackColor = true;
            this.btn_Cancal.Click += new System.EventHandler(this.btn_Cancal_Click);
            // 
            // btn_OK
            // 
            this.btn_OK.Location = new System.Drawing.Point(42, 119);
            this.btn_OK.Name = "btn_OK";
            this.btn_OK.Size = new System.Drawing.Size(75, 30);
            this.btn_OK.TabIndex = 7;
            this.btn_OK.Text = "OK";
            this.btn_OK.UseVisualStyleBackColor = true;
            this.btn_OK.Click += new System.EventHandler(this.btn_OK_Click);
            // 
            // cb_DefaultValue
            // 
            this.cb_DefaultValue.FormattingEnabled = true;
            this.cb_DefaultValue.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8"});
            this.cb_DefaultValue.Location = new System.Drawing.Point(130, 25);
            this.cb_DefaultValue.Name = "cb_DefaultValue";
            this.cb_DefaultValue.Size = new System.Drawing.Size(110, 24);
            this.cb_DefaultValue.TabIndex = 12;
            this.cb_DefaultValue.MouseLeave += new System.EventHandler(this.cb_DefaultValue_MouseLeave);
            this.cb_DefaultValue.MouseHover += new System.EventHandler(this.cb_DefaultValue_MouseHover);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 17);
            this.label1.TabIndex = 14;
            this.label1.Text = "Tag Value:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 72);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(96, 17);
            this.label2.TabIndex = 15;
            this.label2.Text = "Priority Value:";
            // 
            // nup_Priority
            // 
            this.nup_Priority.Location = new System.Drawing.Point(130, 72);
            this.nup_Priority.Maximum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.nup_Priority.Name = "nup_Priority";
            this.nup_Priority.Size = new System.Drawing.Size(110, 22);
            this.nup_Priority.TabIndex = 16;
            // 
            // frm_Priority
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(273, 164);
            this.Controls.Add(this.nup_Priority);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cb_DefaultValue);
            this.Controls.Add(this.btn_Cancal);
            this.Controls.Add(this.btn_OK);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frm_Priority";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Priority Setting";
            ((System.ComponentModel.ISupportInitialize)(this.nup_Priority)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_Cancal;
        private System.Windows.Forms.Button btn_OK;
        private System.Windows.Forms.ComboBox cb_DefaultValue;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown nup_Priority;
    }
}