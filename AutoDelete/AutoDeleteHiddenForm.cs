using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using System.IO.Packaging;
using System.Configuration;

namespace MyNewService
{
    public partial class AutoDeleteHiddenForm : Form
    {
        public delegate void DelegateAddString(String s);
        public delegate void DelegateThreadFinished();

        ManualResetEvent m_EventStopThread;
        ManualResetEvent m_EventThreadStopped;
        public DelegateAddString m_DelegateAddString;
        public DelegateThreadFinished m_DelegateThreadFinished;

        Thread myThread;
        private NewAutoDeleteClass autoDelClass;

        int count = 0;
        public bool bIsSimpleDisplay;
        public bool bIsDelRO;
        frmSetDelEmptyFolderPath frm;
        frmWhitelisting frmWhite;
        public List<EmptyPath> lst_ClearPath;
        public List<EmptyPath> lst_Whitelisting;
        
        int iThread;

        public AutoDeleteHiddenForm()
        {
            InitializeComponent();
            m_DelegateAddString = new DelegateAddString(this.SetTextOnRichTextBox);
            m_DelegateThreadFinished = new DelegateThreadFinished(this.ThreadFinished);

            m_EventStopThread = new ManualResetEvent(false);
            m_EventThreadStopped = new ManualResetEvent(false);

            autoDelClass = new NewAutoDeleteClass();

            mynotifyicon = new NotifyIcon();
            mynotifyicon.BalloonTipText = "Auto Delete is Running...";
            mynotifyicon.BalloonTipTitle = "Auto Delete";
            mynotifyicon.BalloonTipIcon = ToolTipIcon.Info;
            lst_ClearPath = new List<EmptyPath>();
            lst_Whitelisting = new List<EmptyPath>();
        }

        public void ThreadFinished()
        {
            try
            {
                myThread.Abort();
            }
            catch (Exception ex) { myThread.Interrupt(); EndProgram(); };
            //EndProgram();
        }

        public void SetTextOnRichTextBox(String strMessage)
        {
            try
            {
                if (rtb_Result.TextLength >= rtb_Result.MaxLength / 2 || rtb_Result.Lines.Length > 4000)
                    rtb_Result.Text = "";
                rtb_Result.SelectionStart = rtb_Result.Text.Length;
                rtb_Result.ScrollToCaret();
                if (strMessage.Equals(".") || strMessage.Equals("\n"))
                    rtb_Result.Text = rtb_Result.Text.Insert(rtb_Result.Text.Length, strMessage);
                else
                    rtb_Result.Text = rtb_Result.Text.Insert(rtb_Result.Text.Length, "[" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + "]" + strMessage + "\n");
                count++;
                lb_Count.Text = count.ToString();
            }catch(Exception ex)
            {
                //rtb_Result.Clear();
            }
            rtb_Result.SelectionStart = rtb_Result.Text.Length;
            rtb_Result.ScrollToCaret();
        }

        private void AutoDeleteHiddenForm_Load(object sender, EventArgs e)
        {
            btn_Stop.Enabled = false;
            String strValue = "";
            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["ROUND_INTERVAL"]); 
                int iTest = strValue.Length;
                nud_Interval.Value = Int32.Parse(strValue);
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("5", "ROUND_INTERVAL");
                nud_Interval.Value = 5;
            }
            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["AUTO_START_UP"]);
                int iTest = strValue.Length;
                if (strValue == "1")
                    ckb_AutoStart.Checked = true;
                else
                    ckb_AutoStart.Checked = false;
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("0", "AUTO_START_UP");
                ckb_AutoStart.Checked = false; 
            }
            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["AUTO_CLOSE"]);
                int iTest = strValue.Length;
                if (strValue == "1")
                    ckb_AutoClose.Checked = true;
                else
                    ckb_AutoClose.Checked = false;
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("0", "AUTO_CLOSE");
                ckb_AutoClose.Checked = false;
            }

            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["MARK_FULL_LOG"]);
                int iTest = strValue.Length;
                if (strValue == "1")
                    ckb_FullLog.Checked = true;
                else
                    ckb_FullLog.Checked = false;
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("0", "MARK_FULL_LOG");
                ckb_FullLog.Checked = false;
            }

            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["DEL_RO"]);
                int iTest = strValue.Length;
                if (strValue == "1")
                    ckb_Del_RO.Checked = true;
                else
                    ckb_Del_RO.Checked = false;
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("0", "DEL_RO");
                ckb_Del_RO.Checked = false;
            }



            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["ClearEmptyFolderPath"]);
                lst_ClearPath.Clear();
                string[] arr_Value = strValue.Split(new string[]{"_|_"}, StringSplitOptions.RemoveEmptyEntries);
                foreach (String str in arr_Value)
                {
                    EmptyPath item = new EmptyPath();
                    string[] arr_temp = str.Split(',');
                    item.bNeedProc = bool.Parse(arr_temp[0]);
                    item.strPath = arr_temp[1];
                    lst_ClearPath.Add(item);
                }
            }catch(Exception ex)
            {
                AddKeySettingToConfigFile("", "ClearEmptyFolderPath");
                ckb_FullLog.Checked = false;
            }


            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["Whitelisting"]);
                lst_Whitelisting.Clear();
                string[] arr_Value = strValue.Split(new string[] { "_|_" }, StringSplitOptions.RemoveEmptyEntries);
                foreach (String str in arr_Value)
                {
                    EmptyPath item = new EmptyPath();
                    string[] arr_temp = str.Split(',');
                    item.bNeedProc = bool.Parse(arr_temp[0]);
                    item.strPath = arr_temp[1];
                    lst_Whitelisting.Add(item);
                }
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("", "Whitelisting");
                ckb_FullLog.Checked = false;
            }

            iThread = Convert.ToInt32(System.Configuration.ConfigurationSettings.AppSettings["NO_OF_THREAD"]);

            if (ckb_AutoStart.Checked == true)
                btn_Start_Click(null,null);
        }

        private void StartProcessData()
        {
            autoDelClass.SetHiddenForm(this);
            autoDelClass.SetInterval((int)nud_Interval.Value*1000*60);
            autoDelClass.SetMarkFullLog(ckb_FullLog.Checked);
            autoDelClass.SetAutoClose(ckb_AutoClose.Checked);
            autoDelClass.OnStart();
        }

        public void EndProgram()
        {
            this.Close();
            Application.Exit();
        }


        private void AutoDeleteHiddenForm_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == this.WindowState)
            {
                mynotifyicon.Visible = true;
                mynotifyicon.ShowBalloonTip(500);
                //this.Hide(); 
                //this.ShowInTaskbar = false;
            }
            else if (FormWindowState.Normal == this.WindowState)
            {
                this.ShowInTaskbar = true;
                mynotifyicon.Visible = false;
            }
        }

        private void mynotifyicon_DoubleClick(object sender, EventArgs e)
        {
            if (this.ShowInTaskbar == false)
            {
                this.WindowState = FormWindowState.Maximized;
                this.ShowInTaskbar = true;
                this.Show();
            }
        }

        private void btn_Start_Click(object sender, EventArgs e)
        {
            myThread = null;
            myThread = new Thread(new ThreadStart(StartProcessData));
            myThread.IsBackground = true;
            myThread.Start(); 
            this.WindowState = FormWindowState.Minimized;
            btn_Start.Enabled = false;
            btn_Stop.Enabled = true;
            ckb_FullLog.Enabled = false;
            ckb_Del_RO.Enabled = false;
            btn_EmptyFolder.Enabled = false;
        }

        private void btn_Stop_Click(object sender, EventArgs e)
        {
            if (iThread > 1)
            {
                autoDelClass.SetStopThread(true);
                if (!myThread.IsAlive)
                    UpdateUI();
            }
            else
            {
                UpdateUI();
            }
        }

        public void UpdateBtnStatusAfterStopDeleteThread()
        {
           MethodInvoker mi = new MethodInvoker(this.UpdateUI);
           this.BeginInvoke(mi, null);
        }

        private void UpdateUI()
        {
            myThread.Abort();
            btn_Start.Enabled = true;
            btn_Stop.Enabled = false;
            ckb_FullLog.Enabled = true;
            ckb_Del_RO.Enabled = true;
            btn_EmptyFolder.Enabled = true;
            SetTextOnRichTextBox("停止");
        }

        private void ckb_FullLog_CheckedChanged(object sender, EventArgs e)
        {
            String strValue = "0";
            if (ckb_FullLog.Checked == true)
                strValue = "1";
            UpdateKeyValueOnConfigFile(strValue, "MARK_FULL_LOG");
        }

        private void ckb_AutoStart_CheckedChanged(object sender, EventArgs e)
        {
            String strValue = "0";
            if (ckb_AutoStart.Checked == true)
                strValue = "1";
            UpdateKeyValueOnConfigFile(strValue, "AUTO_START_UP");
        }

        private void AddKeySettingToConfigFile(String strValue, String strKeyName)
        {
            System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            config.AppSettings.Settings.Add(strKeyName, strValue);
            config.Save(ConfigurationSaveMode.Full);
            ConfigurationManager.RefreshSection("appSettings");
            config.AppSettings.Settings[strKeyName].Value = strValue;
            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");
        }

        public void UpdateKeyValueOnConfigFile(String strValue, String strKeyName)
        {
            System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            config.AppSettings.Settings[strKeyName].Value = strValue;
            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");
        }

        private void nud_Interval_ValueChanged(object sender, EventArgs e)
        {
            String strValue = "";
            strValue = nud_Interval.Value.ToString();
            UpdateKeyValueOnConfigFile(strValue, "ROUND_INTERVAL");
        }

        private void ckb_AutoClose_CheckedChanged(object sender, EventArgs e)
        {
            String strValue = "0";
            if (ckb_AutoClose.Checked == true)
                strValue = "1";
            UpdateKeyValueOnConfigFile(strValue, "AUTO_CLOSE");
        }

        private void ckb_SimpleDisplay_CheckedChanged(object sender, EventArgs e)
        {
            bIsSimpleDisplay = ckb_SimpleDisplay.Checked;
        }

        private void btn_EmptyFolder_Click(object sender, EventArgs e)
        {
            if (frm == null)
            {
                frm = new frmSetDelEmptyFolderPath();
                frm.SetMainForm(this);
            }
            frm.ShowDialog();
        }

        public void KillSetDelEmptyFolderPathForm(String strValue)
        {
            frm = null;
            UpdateKeyValueOnConfigFile(strValue, "ClearEmptyFolderPath");
            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["ClearEmptyFolderPath"]);
                lst_ClearPath.Clear();
                string[] arr_Value = strValue.Split(new string[] { "_|_" }, StringSplitOptions.RemoveEmptyEntries);
                foreach (String str in arr_Value)
                {
                    EmptyPath item = new EmptyPath();
                    string[] arr_temp = str.Split(',');
                    item.bNeedProc = bool.Parse(arr_temp[0]);
                    item.strPath = arr_temp[1];
                    lst_ClearPath.Add(item);
                }
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("", "ClearEmptyFolderPath");
                ckb_FullLog.Checked = false;
            }
        }

        private void ckb_Del_RO_CheckedChanged(object sender, EventArgs e)
        {
            String strValue = "0";
            if (ckb_Del_RO.Checked == true)
                strValue = "1";
            UpdateKeyValueOnConfigFile(strValue, "DEL_RO");
            bIsDelRO = ckb_Del_RO.Checked;
        }

        private void btn_Whitelisting_Click(object sender, EventArgs e)
        {
            if (frmWhite == null)
            {
                frmWhite = new frmWhitelisting();
                frmWhite.SetMainForm(this);
            }
            frmWhite.ShowDialog();
        }

        public void KillWhitelistingForm(String strValue)
        {
            frmWhite = null;
            UpdateKeyValueOnConfigFile(strValue, "Whitelisting");
            try
            {
                strValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["Whitelisting"]);
                lst_Whitelisting.Clear();
                string[] arr_Value = strValue.Split(new string[] { "_|_" }, StringSplitOptions.RemoveEmptyEntries);
                foreach (String str in arr_Value)
                {
                    EmptyPath item = new EmptyPath();
                    string[] arr_temp = str.Split(',');
                    item.bNeedProc = bool.Parse(arr_temp[0]);
                    item.strPath = arr_temp[1];
                    lst_Whitelisting.Add(item);
                }
            }
            catch (Exception ex)
            {
                AddKeySettingToConfigFile("", "Whitelisting");
                ckb_FullLog.Checked = false;
            }
        }
    }
}

public class EmptyPath
{
    public bool bNeedProc { get; set; }
    public String strPath { get; set; }
}