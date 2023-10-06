using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Reuters_with_Web_Service.com.reuters.rmb;
using System.Net;

namespace Reuters_with_Web_Service
{
    public partial class frmLogin : Form
    {
        private DBHandler DB;
        private WebServiceHandler service;
        private SettingHandler setting;
        private bool isFirstStart;
        private WebServiceFactory factory;
        private bool bSetAutoStart;
        public frmLogin()
        {
            InitializeComponent();
            setting = new SettingHandler();
            factory = new WebServiceFactory();
            tb_Server.Text = setting.GetKeyValue("DataSource");
            tb_Database.Text = setting.GetKeyValue("InitialCatalog");
            tb_DBUserName.Text = setting.GetKeyValue("DBUserName");
            tb_DBPassword.Text = setting.GetKeyValue("DBPassword");

            String strValue = setting.GetKeyValue("IsWinAuth");
            if (strValue.Equals("1"))
                ckb_IsWindowAuth.Checked = true;

            strValue = setting.GetKeyValue("IsAutoLogin");
            if (strValue.Equals("1"))
                ckb_AutoLogin.Checked = true;
            ckb_IsWindowAuth_CheckedChanged(null, null);
            isFirstStart = true;
            bSetAutoStart = false;
        }

        private void btn_Cancel_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btn_Login_Click(object sender, EventArgs e)
        {
            if (!DoLogin())
            { return; }
            SetLoggingConfig();
            //if (setting.GetEMSVersion().Length == 0)
            //{
            //    MessageBox.Show("Incorrect EMS Identification");
            //    return;
            //}
            frm_Main frm = new frm_Main();
            if (bSetAutoStart)
                frm.SetAutoStart();
            frm.SetDB(DB);
            frm.SetService(service);
            this.Hide();
            frm.Show();
        }

        private void SetLoggingConfig()//used to setup the basic setting on log file on first start up
        {
            try
            {
                double dFileSize = double.Parse(setting.GetKeyValue("LOG_MAX_FILE_SIZE"));
            }
            catch (Exception ex)
            {
                setting.UpdateKeyValue("50", "LOG_MAX_FILE_SIZE");
            }
            
            String strLogFilePath = setting.GetKeyValue(("LOG_FILE_PATH"));
            if (strLogFilePath.Length==0)
            {
                setting.UpdateKeyValue("C:\\founder\\temp\\Reuters_WS", "LOG_FILE_PATH");
            }
            try
            {
                int iCurrentLogNo = Int32.Parse(setting.GetKeyValue("CURRENT_LOG_NO"));
            }catch(Exception ex)
            {
                setting.UpdateKeyValue("1", "CURRENT_LOG_NO");
            }
            try
            {
                int iNoOfLogFile = Int32.Parse(setting.GetKeyValue("LOG_NO_ROTATE"));
            }
            catch (Exception ex)
            {
                setting.UpdateKeyValue("5", "LOG_NO_ROTATE");
            }
        }

        private bool DoLogin()
        {
            setting.UpdateKeyValue(tb_Server.Text, "DataSource");
            setting.UpdateKeyValue(tb_Database.Text, "InitialCatalog");
            setting.UpdateKeyValue(tb_DBUserName.Text, "DBUserName");
            if (ckb_AutoLogin.Checked)
                setting.UpdateKeyValue(tb_DBPassword.Text, "DBPassword");
            else
                setting.UpdateKeyValue("", "DBPassword");
            setting.UpdateKeyValue(ckb_IsWindowAuth.Checked ? "1" : "0", "IsWinAuth");
            setting.UpdateKeyValue(ckb_AutoLogin.Checked ? "1" : "0", "IsAutoLogin");

            DB = new DBHandler();
            DB.PrePareConnStr(tb_DBUserName.Text, tb_DBPassword.Text, ckb_IsWindowAuth.Checked);
            bool bDBValidate = DB.Connect();
            bool bServiceValidate = false;
            if (bDBValidate)
            {
                tb_ReutersUserName.Text = DB.GetConfig("Reuters_UserName");
                tb_ReutersPassword.Text = DB.GetConfig("Reuters_Password");
                service = factory.Create(tb_ReutersUserName.Text, tb_ReutersPassword.Text, setting.GetEMSVersion());
                if (service == null)
                {
                    MessageBox.Show("Please defind the correct EMS Version in configuration file", "Login Failed");
                    return false;
                }
                if (setting.GetEMSVersion().Equals("EMS4.0", StringComparison.OrdinalIgnoreCase))
                {
                    service.SetDB(DB);
                    bServiceValidate = true;
                    //bServiceValidate = service.Connect();
                }
                else
                {
                    //bServiceValidate = service.Connect();
                    bServiceValidate = true;
                }
                if (bServiceValidate == true)
                    return true;
            }
            if (!bDBValidate || !bServiceValidate)
            {
                String strError = "Incorrect login information detail:\n";
                strError += DB.GetDBError() + "\n";
                if (service!=null)
                    strError += service.GetError();
                MessageBox.Show(strError, "Login Failed");
                return false;
            }
            return false;
        }

        private void ckb_IsWindowAuth_CheckedChanged(object sender, EventArgs e)
        {
            if (ckb_IsWindowAuth.Checked == true)
            {
                groupBox2.Enabled = false;
                tb_DBUserName.Text = "";
                tb_DBPassword.Text = "";
            }
            else
            {
                groupBox2.Enabled = true;
                tb_DBUserName.Text = setting.GetKeyValue("DBUserName");
                tb_DBPassword.Text = setting.GetKeyValue("DBPassword");
            }
        }

        private void frmLogin_Load(object sender, EventArgs e)
        {
            this.KeyPreview = true;
        }

        private void frmLogin_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Escape)
                Application.Exit();
        }

        private void ckb_AutoLogin_CheckedChanged(object sender, EventArgs e)
        {
            isFirstStart = false;
        }

        private void frmLogin_Activated(object sender, EventArgs e)
        {
            
        }

        private void tb_DBPassword_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
                btn_Login_Click(null, null);
        }

        private void tb_DBUserName_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
                tb_DBPassword.Focus();
        }

        private void frmLogin_Validated(object sender, EventArgs e)
        {
            
        }

        private void frmLogin_Shown(object sender, EventArgs e)
        {
            if (ckb_AutoLogin.Checked && isFirstStart)
            {
                bSetAutoStart = true;
                btn_Login_Click(null, null);
                isFirstStart = false;
                bSetAutoStart = false;
            }
        }
    }
}
