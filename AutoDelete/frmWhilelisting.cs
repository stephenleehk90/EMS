using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace MyNewService
{
    public partial class frmWhitelisting : Form
    {
        AutoDeleteHiddenForm frm;
        List<EmptyPath> lst_EmptyPath;
        public frmWhitelisting()
        {
            InitializeComponent();
        }

        public void SetMainForm(AutoDeleteHiddenForm frm)
        {
            this.frm = frm;
            lst_EmptyPath = new List<EmptyPath>();
            foreach (EmptyPath p in frm.lst_Whitelisting)
            {
                lst_EmptyPath.Add(p);
            }
        }

        private void tb_Path_TextChanged(object sender, EventArgs e)
        {
            if (tb_Path.Text.Length > 0)
            {
                btn_Confirm.Enabled = true;
                btn_Del.Enabled = true;
                btn_OK.Enabled = false;
            }
            else
            {
                btn_Confirm.Enabled = false;
                btn_Del.Enabled = false;
                btn_OK.Enabled = true;
            }
        }

        private void frmWhitelisting_FormClosing(object sender, FormClosingEventArgs e)
        {
            String strFormat = "{0},{1}", strSeperator = "_|_", strValue = "";
            for (int i = 0; i < lstv_Path.Items.Count; i++)
            {
                strValue += String.Format(strFormat, lstv_Path.Items[i].Checked, lstv_Path.Items[i].Text) + strSeperator;
            }
            if (strValue.Length > 0)
                strValue.Remove(strValue.Length - strSeperator.Length - 1);
            frm.KillWhitelistingForm(strValue);
        }

        private void btn_OK_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btn_Confirm_Click(object sender, EventArgs e)
        {
            if (btn_Confirm.Text == "修改")
            {
                lstv_Path.SelectedItems[0].Text = tb_Path.Text;
            }
            else
            {
                if (tb_Path.Text == "")
                    return;
                EmptyPath p = lst_EmptyPath.Find(delegate(EmptyPath ep) { return ep.strPath == tb_Path.Text; });
                if (p != null)
                    return;
                ListViewItem item = new ListViewItem(tb_Path.Text);
                item.Checked = true;
                lstv_Path.Items.Add(item);
            }
            tb_Path.Text = "";
        }

        private void btn_Del_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lstv_Path.SelectedItems.Count; i++)
            {
                ListViewItem item = lstv_Path.SelectedItems[i];
                EmptyPath p = lst_EmptyPath.Find(delegate(EmptyPath ep) { return ep.strPath == item.Text; });
                if(p != null)
                    lst_EmptyPath.Remove(p);
                lstv_Path.Items.Remove(item);
                i--;
            }
        }

        private void lstv_Path_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (lstv_Path.SelectedItems.Count == 1 || lstv_Path.SelectedItems.Count == 0)
            {   
                btn_Confirm.Enabled = true;
                if (lstv_Path.SelectedItems.Count == 1)
                {
                    tb_Path.Text = lstv_Path.SelectedItems[0].Text;
                    btn_Confirm.Text = "修改";
                }
                else
                    btn_Confirm.Text = "新增";
            }
            else
                btn_Confirm.Enabled = false;
            if (lstv_Path.SelectedItems.Count == 0)
            {
                tb_Path.Text = "";
                btn_Confirm.Enabled = true;
            }
        }

        private void frmWhitelisting_Load(object sender, EventArgs e)
        {
            foreach (EmptyPath path in frm.lst_Whitelisting)
            {
                ListViewItem item = new ListViewItem(path.strPath);
                item.Checked = path.bNeedProc;
                lstv_Path.Items.Add(item);
            }
        }
    }
}
