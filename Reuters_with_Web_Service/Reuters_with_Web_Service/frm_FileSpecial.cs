using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Reuters_with_Web_Service
{
    public partial class frm_FileSpecial : Form
    {   
        private frmSetParameter frmParameter;
        private String strEMSVersion;
        bool isNew;

        public frm_FileSpecial()
        {
            InitializeComponent();
            isNew = true;
            strEMSVersion = "";
        }

        public void SetConfig(String str_FileSpec, String str_Agency, bool isUnique)
        {
            isNew = false;
            tb_FileSpec.Text = str_FileSpec;
            if (strEMSVersion.Equals("EMSFBS"))
                tb_ClassName.Text = str_Agency;
            else
                cb_AgencyTable.Text = str_Agency;
            cb_Unique.Checked = isUnique;
            tb_FileSpec.Enabled = false;
            if (!strEMSVersion.Equals("EMSFBS"))
            {
                for (int i = 0; i < cb_AgencyTable.Items.Count; i++)
                {
                    if (((ComboItem)cb_AgencyTable.Items[i]).Name.Equals(str_Agency, StringComparison.OrdinalIgnoreCase))
                        cb_AgencyTable.SelectedIndex = i;
                }
            }
        }

        public void SetFrmParameter(frmSetParameter frmParameter)
        {
            this.frmParameter = frmParameter;
        }

        private void btn_OK_Click(object sender, EventArgs e)
        {
            if (!Validation())
                return;
            if (isNew)
            {
                if (!strEMSVersion.Equals("EMSFBS"))
                    frmParameter.AddFileSpec(tb_FileSpec.Text, ((ComboItem)cb_AgencyTable.Items[cb_AgencyTable.SelectedIndex]).Name, ((ComboItem)cb_AgencyTable.Items[cb_AgencyTable.SelectedIndex]).Value, cb_Unique.Checked);
                else
                    frmParameter.AddFileSpec(tb_FileSpec.Text, tb_ClassName.Text, tb_ClassName.Text, cb_Unique.Checked);
            }
            else
            {
                if (!strEMSVersion.Equals("EMSFBS"))
                    frmParameter.UpdateFileSpec(tb_FileSpec.Text, ((ComboItem)cb_AgencyTable.Items[cb_AgencyTable.SelectedIndex]).Name, ((ComboItem)cb_AgencyTable.Items[cb_AgencyTable.SelectedIndex]).Value, cb_Unique.Checked);
                else
                    frmParameter.UpdateFileSpec(tb_FileSpec.Text, tb_ClassName.Text, tb_ClassName.Text, cb_Unique.Checked);
            }
            frmParameter.DestoryFileSpecForm();
            this.Close();
        }

        private void btn_Cancal_Click(object sender, EventArgs e)
        {
            frmParameter.DestoryFileSpecForm();
            this.Close();
        }

        private void frm_FileSpecial_Load(object sender, EventArgs e)
        {
            ResizeLayout();
        }

        public void SetEMSVersion(String strEMSVersion)
        {
            this.strEMSVersion = strEMSVersion;
        }

        private void ResizeLayout()
        {
            switch (this.strEMSVersion)
            {
                case "EMS6.0":
                    break;
                case "EMS4.0":
                    break;
                case "EMSFBS":
                    FBSLayoutStyle();
                    break;
                default:
                    break;
            }
        }

        private void FBSLayoutStyle()
        {
            label2.Text = "Class Name";
            cb_AgencyTable.Visible = false;
            tb_ClassName.Visible = true;
        }

        public void SetAgencyList(List<ComboItem> lst_Agency)
        {
            for (int i = 0; i < lst_Agency.Count; i++)
            {
                cb_AgencyTable.Items.Add(new ComboItem(lst_Agency[i].Name, lst_Agency[i].Value));
            }
        }

        private bool Validation()
        {
            bool nRet = true;
            String strError = "";
            if (cb_AgencyTable.Visible == true)
            {
                if (cb_AgencyTable.Text.Length == 0)
                {
                    strError += "Please select Agency.\n";
                    nRet = false;
                }
            }
            if (tb_FileSpec.Text.Length == 0)
            {
                strError += "Please enter File Special.\n";
                nRet = false;
            }
            if (tb_ClassName.Visible == true)
            {
                if (tb_ClassName.Text.Length == 0)
                {
                    strError += "Please enter Class Name.\n";
                    nRet = false;
                }
            }

            if (isNew)
            {
                List<FileSpec> temp_lst =frmParameter.GetFileSpecList();
                FileSpec temp_spec = temp_lst.Find(delegate(FileSpec file)
                {
                    return file.strFileSpecName == tb_FileSpec.Text;
                });
                if (temp_spec != null)
                {
                    strError += "Duplicate File Special type.\n";
                    nRet = false;
                }
            }

            if (strError.Length > 0)
                MessageBox.Show(strError);
            return nRet;
        }

        private void frm_FileSpecial_Leave(object sender, EventArgs e)
        {
        }

        private void frm_FileSpecial_Shown(object sender, EventArgs e)
        {
            this.TopMost = true;
            this.Activate();
        }

        private void frm_FileSpecial_FormClosed(object sender, FormClosedEventArgs e)
        {
            frmParameter.DestoryFileSpecForm();
        }
    }
}
