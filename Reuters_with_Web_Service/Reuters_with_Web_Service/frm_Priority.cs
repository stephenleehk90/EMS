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
    public partial class frm_Priority : Form
    {
        private frmSetParameter frmParameter;
        ToolTip tooltip;
        bool isNew;

        public frm_Priority()
        {
            InitializeComponent();
            isNew = true;
        }

        public void SetFrmParameter(frmSetParameter frmParameter)
        {
            this.frmParameter = frmParameter;
        }

        private void btn_Cancal_Click(object sender, EventArgs e)
        {
            frmParameter.DestoryPriorityForm();
            this.Close();
        }

        private void btn_OK_Click(object sender, EventArgs e)
        {
            if (!Validation())
                return;
            int iTagValue = int.Parse(cb_DefaultValue.Text);
            int iPriority = (int)nup_Priority.Value;
            if (isNew)
                frmParameter.AddPriority(iTagValue, iPriority);
            else
                frmParameter.UpdatePriority(iTagValue, iPriority);
            frmParameter.DestoryPriorityForm();
            this.Close();
        }

        private bool Validation()
        {
            String strError = "";;
            if (cb_DefaultValue.Text.Length == 0)
                strError = "Tag Value cannot empty.\n";
            else
            {
                try
                {
                    int iTest = int.Parse(cb_DefaultValue.Text);
                    if(iTest<0)
                        strError = "Tag Value cannot smaller than zero.\n";
                }catch(Exception ex)
                {
                    strError = "Tag Value is not a valided value.\n";
                    MessageBox.Show(strError);
                    return false;
                }
            }

            List<Priority> lst = frmParameter.GetPriorityList();
            if (lst != null && isNew)
            {
                Priority found = lst.Find(delegate(Priority p) { return int.Parse(cb_DefaultValue.Text) == p.iTagValue; });
                if (found != null)
                    strError = "Repeated tag value is not allowed.\n";
            }
            if(strError.Length!=0)
            {
                MessageBox.Show(strError);
                return false;
            }
            return true;
        }
        
        public void SetConfig(int iTagValue, int iPriority)
        {
            isNew = false;
            cb_DefaultValue.Enabled = false;
            nup_Priority.Value = iPriority;
            cb_DefaultValue.Text = iTagValue.ToString();
        }

        private void cb_DefaultValue_MouseHover(object sender, EventArgs e)
        {
            if (tooltip != null)
                return;
            tooltip = new ToolTip();
            tooltip.Show("A priority of ‘4’ is the default value. \nPriority ‘1’ is the highest (bulletin) value. \nPriority ‘8’ would be the lowest.", cb_DefaultValue);
        }

        private void cb_DefaultValue_MouseLeave(object sender, EventArgs e)
        {

        }
    }
}
