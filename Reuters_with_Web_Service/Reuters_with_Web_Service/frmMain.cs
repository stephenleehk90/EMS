using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Reuters_with_Web_Service.com.reuters.rmb;
using System.Threading;
using System.IO;
using System.IO.Packaging;
using System.Configuration;

namespace Reuters_with_Web_Service
{
    public partial class frm_Main : Form
    {
        public delegate void DelegateAddString(String s);
        public delegate void DelegateThreadFinished();

        private DBHandler DB;
        private WebServiceHandler service;
        private DateTime dtPreviousProcessTime;
        Thread myThread;
        private SettingHandler setting;
        List<SettingParameter> lst_SettingParam;
        List<NewsChannel> lst_Channels;
        DateTime dtPrevStartTime;
        public List<SettingParameter> lst_Setting;

        ManualResetEvent m_EventStopThread;
        ManualResetEvent m_EventThreadStopped;
        public DelegateAddString m_DelegateAddString;
        public DelegateThreadFinished m_DelegateThreadFinished;

        bool isAutoLogin;
        public bool isLoadAddInfo;
        private frmSetParameter frm;

        public frm_Main()
        {
            InitializeComponent();
            setting = new SettingHandler();

            m_DelegateAddString = new DelegateAddString(this.SetTextOnRichTextBox);
            m_DelegateThreadFinished = new DelegateThreadFinished(this.ThreadFinished);

            m_EventStopThread = new ManualResetEvent(false);
            m_EventThreadStopped = new ManualResetEvent(false);
            isAutoLogin = false;
            isLoadAddInfo = false;
        }

        public void SetDB(DBHandler DB)
        {
            this.DB = DB;
        }

        public void SetAutoStart()
        {
            isAutoLogin = true;
        }

        public void SetService( WebServiceHandler service)
        {
            this.service = service;
            //lst_Channels = service.GetAllChannels();
        }

        private void frmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void btn_Config_Click(object sender, EventArgs e)
        {
            if (lv_Config.SelectedItems.Count == 0)
            {
                return;
            }
            if (frm != null)
                return;
            frm = new frmSetParameter(this);
            frm.SetDB(DB);
            //frm.SetChannelList(lst_Channels);
            for (int i = 0; i < lst_SettingParam.Count; i++)
            {
                if (lst_SettingParam[i].strChannelsName_Format.Equals(lv_Config.SelectedItems[0].Text, StringComparison.OrdinalIgnoreCase))
                    frm.SetSettingParameter(lst_SettingParam[i]);
            }
            frm.Show();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            dtPrevStartTime = DateTime.MinValue;
            this.KeyPreview = true;
            InitConfigInfoList();
            service.SetDB(DB);
            String strValue = setting.GetKeyValue("Interval");
            try
            {
                if (strValue != null)
                    nup_TimeInterval.Value = Int32.Parse(strValue);
                else
                    nup_TimeInterval.Value = 30;
            }catch(Exception ex)
            {
                nup_TimeInterval.Value = 30;
            }
            btn_Stop.Enabled = false;
            if (isAutoLogin)
            {
                SelectConfigList();
                btn_Start_Click(null, null);
            }

            strValue = setting.GetKeyValue("LoadAddInfo");
            if (strValue.Equals("1"))
                isLoadAddInfo = true;
        }

        public void InitConfigInfoList()
        {
            lv_Config.Items.Clear();
            setting = new SettingHandler();
            lst_SettingParam = setting.GetSettingParameterList();
            for (int i = 0; i < lst_SettingParam.Count; i++)
            {
                ListViewItem item = new ListViewItem(lst_SettingParam[i].strChannelsName);
                lv_Config.Items.Add(item);
            }

            if (lv_Config.Items.Count == 0)
                btn_Config.Enabled = false;
            else
                btn_Config.Enabled = true;

            cb_SelectAll.Checked = false;
            cb_SelectAll_CheckedChanged(null, null);
        }

        private void btn_Exit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btn_Clear_Click(object sender, EventArgs e)
        {
            rtb_Result.Clear();
        }

        private bool CheckConfig()
        {
            bool nRet = true;
            if (lv_Config.CheckedItems.Count == 0)
            {
                MessageBox.Show("Please select at least one Channel or Path to process", "Error");
                nRet = false;
            }

            return nRet;
        }

        private void btn_Start_Click(object sender, EventArgs e)
        {
            if (frm != null)
            {
                MessageBox.Show("Please finish all setting first.");
                return;
            }
            if (!CheckConfig())
                return;
            lst_Setting = new List<SettingParameter>();
            for (int i = 0; i < lv_Config.CheckedItems.Count; i++)
            {
                for (int j = 0; j < lst_SettingParam.Count; j++)
                {
                    //*************************************************************************
                    // Edit Stephen 2018-05-17
                    //*************************************************************************
                    string strCompareChannel = lst_SettingParam[j].strChannelsName;
                    if (setting.GetEMSVersion().Equals("EMS6.5") || setting.GetEMSVersion().Equals("EMS6.0") || setting.GetEMSVersion().Equals("EMSFBS"))
                    {
                        strCompareChannel = lst_SettingParam[j].strChannelsName_Format;
                    }
                    //if (lv_Config.CheckedItems[i].Text.Equals(lst_SettingParam[j].strChannelsName))
                    if (lv_Config.CheckedItems[i].Text.Equals(strCompareChannel))
                    //*************************************************************************
                    {
                        if (!setting.GetEMSVersion().Equals("EMS4.0"))
                        {
                            if (CheckSettingPath(j))
                                lst_Setting.Add(lst_SettingParam[j]);
                            else
                            {
                                String strError = String.Format("The path config on \"{0}\" contains error, \"{1}\" will be skipped \n",
                                    lst_SettingParam[j].strChannelsName, lst_SettingParam[j].strChannelsName);
                                SetTextOnRichTextBox(strError);
                            }
                        }else
                            lst_Setting.Add(lst_SettingParam[j]);
                    }
                }
            }
            if (dtPrevStartTime != DateTime.MinValue)
            {
                TimeSpan tSpan = DateTime.Now - dtPrevStartTime;
                service.SetMaxAge(String.Format("{0}h", tSpan.Hours == 0 ? 1 : tSpan.Hours));
            }
            else
                dtPrevStartTime = DateTime.Now;

            service.SetInterval(Int32.Parse(nup_TimeInterval.Value.ToString()) * 1000);
            SaveCheckedList();
            myThread = new Thread(new ThreadStart(StartProcessData));
            //myThread = new Thread(new ThreadStart(ThreadTest));
            myThread.IsBackground = true;
            myThread.Start();
            timer1.Start();
            //service.GetAllChannels();
            btn_Start.Enabled = false;
            btn_Stop.Enabled = true;
            btn_Config.Enabled = false;
            btn_AddConfig.Enabled = false;
            btn_Remove.Enabled = false;
            btn_ClearAll.Enabled = false;
            cb_SelectAll.Enabled = false;
            nup_TimeInterval.Enabled = false;
            lv_Config.Enabled = false;
        }

        private void StartProcessData()
        {
            //while (true)
            //{
            //    this.Invoke((MethodInvoker)delegate()
            //    {
                    //List<SettingParameter> lst_Setting = new List<SettingParameter>();
                    //for (int i = 0; i < lv_Config.CheckedItems.Count; i++)
                    //{
                    //    for (int j = 0; j < lst_SettingParam.Count; j++)
                    //    {
                    //        if (lv_Config.CheckedItems[i].Text.Equals(lst_SettingParam[j].strChannelsName))
                    //        {
                    //            if (CheckSettingPath(j))
                    //                lst_Setting.Add(lst_SettingParam[j]);
                    //            else
                    //            {
                    //                String strError = String.Format("The path config on \"{0}\" contains error, \"{1}\" will be skipped \n", 
                    //                    lst_SettingParam[j].strChannelsName, lst_SettingParam[j].strChannelsName);
                    //                SetTextOnRichTextBox(strError);
                    //            }
                    //        }
                    //    }
                    //}
                    //if (dtPrevStartTime != DateTime.MinValue)
                    //{
                    //    TimeSpan tSpan = DateTime.Now - dtPrevStartTime;
                    //    service.SetMaxAge(String.Format("{0}h", tSpan.Hours == 0 ? 1 : tSpan.Hours));
                    //}
                    //else
                    //    dtPrevStartTime = DateTime.Now;
                    service.SetFrmMain(this);
                    service.Start(lst_Setting);
                    //SetTextOnRichTextBox("Start...");
                    //service.Start(lst_Setting);
                    //SetTextOnRichTextBox("Finish.\n");
                //});
                //Thread.Sleep(Int32.Parse(nup_TimeInterval.Value.ToString())*1000);
            //}
        }

        private bool CheckSettingPath(int index)
        {
            SettingParameter settingParam = lst_SettingParam[index];

            settingParam.strPhotoSourcePath = settingParam.strPhotoSourcePath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            settingParam.strPhotoSourcePath = settingParam.strPhotoSourcePath.Replace("%M", DateTime.Now.ToString("MM"));
            settingParam.strPhotoSourcePath = settingParam.strPhotoSourcePath.Replace("%D", DateTime.Now.ToString("dd"));

            settingParam.strPhotoSmallPath = settingParam.strPhotoSmallPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            settingParam.strPhotoSmallPath = settingParam.strPhotoSmallPath.Replace("%M", DateTime.Now.ToString("MM"));
            settingParam.strPhotoSmallPath = settingParam.strPhotoSmallPath.Replace("%D", DateTime.Now.ToString("dd"));

            settingParam.strPhotoMiddlePath = settingParam.strPhotoMiddlePath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            settingParam.strPhotoMiddlePath = settingParam.strPhotoMiddlePath.Replace("%M", DateTime.Now.ToString("MM"));
            settingParam.strPhotoMiddlePath = settingParam.strPhotoMiddlePath.Replace("%D", DateTime.Now.ToString("dd"));

            settingParam.strXMLDownloadPath = settingParam.strXMLDownloadPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            settingParam.strXMLDownloadPath = settingParam.strXMLDownloadPath.Replace("%M", DateTime.Now.ToString("MM"));
            settingParam.strXMLDownloadPath = settingParam.strXMLDownloadPath.Replace("%D", DateTime.Now.ToString("dd"));

            settingParam.strFileCopyPath = settingParam.strFileCopyPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            settingParam.strFileCopyPath = settingParam.strFileCopyPath.Replace("%M", DateTime.Now.ToString("MM"));
            settingParam.strFileCopyPath = settingParam.strFileCopyPath.Replace("%D", DateTime.Now.ToString("dd"));
            
            try
            {
                if (!settingParam.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    if (!Directory.Exists(settingParam.strPhotoSourcePath))
                        Directory.CreateDirectory(settingParam.strPhotoSourcePath);
                    if (!Directory.Exists(settingParam.strPhotoMiddlePath))
                        Directory.CreateDirectory(settingParam.strPhotoMiddlePath);
                    if (!Directory.Exists(settingParam.strPhotoSmallPath))
                        Directory.CreateDirectory(settingParam.strPhotoSmallPath);
                }
                if (settingParam.bNeedCopyFile)
                {
                    if (!Directory.Exists(settingParam.strXMLDownloadPath))
                        Directory.CreateDirectory(settingParam.strXMLDownloadPath);
                }

                if(settingParam.strFileCopyPath.Length!=0)
                    if (!Directory.Exists(settingParam.strFileCopyPath))
                        Directory.CreateDirectory(settingParam.strFileCopyPath);
            }
            catch (Exception ex)
            {
                return false;
            }

            return true;
        }

        private void ThreadTest()//For testing only
        {
            while(true){
                this.Invoke((MethodInvoker)delegate()
                {
                    int count = 0;
                    while (count<1000)
                    {
                        rtb_Result.Text = rtb_Result.Text.Insert(rtb_Result.Text.Length, "Testing Thread\n"); count++;
                        this.Refresh();
                    }
                    rtb_Result.Refresh();
                });
                Thread.Sleep(100);
            }
        }

        private void btn_Stop_Click(object sender, EventArgs e)
        {
            ThreadFinished();
            int i = rtb_Result.Text.LastIndexOf(".");
            if (rtb_Result.Text.LastIndexOf(".") == rtb_Result.Text.Length-1)
                SetTextOnRichTextBox("\n");
            SetTextOnRichTextBox("Stop.");
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("Are you sure to exit?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                btn_Stop_Click(null, null);
            }
            else
            {
                e.Cancel = true;
                return;
            }

            // Edit Stephen 2022-10-06 
            if (DB!=null)
                DB.CloseConnection();
            // Edit Stephen 2022-10-06 
        }

        private void lv_Config_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (setting.GetEMSVersion())
            {
                case "EMS6.0":
                    TKPConfigListDetailStyle();
                    break;
                case "EMS4.0":
                    TKPConfigListDetailStyle();
                    break;
                case "EMSFBS":
                    FBSConfigListDetailStyle();
                    break;
                case "EMS6.5":
                    MPConfigListDetailStyle();
                    break;
            }
        }

        private void btn_AddConfig_Click(object sender, EventArgs e)
        {
            if (frm != null)
                return;
            frm = new frmSetParameter(this);
            frm.SetDB(DB);
            frm.SetSettingParameter(new SettingParameter());
            //frm.SetChannelList(lst_Channels);
            frm.Show();
        }

        private void cb_SelectAll_CheckedChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < lv_Config.Items.Count; i++)
                lv_Config.Items[i].Checked = cb_SelectAll.Checked;
        }

        private void btn_Remove_Click(object sender, EventArgs e)
        {
            if (frm != null)
            {
                MessageBox.Show("Please finish all setting first.");
                return;
            }
            if (lv_Config.SelectedItems.Count == 0)
            {
                MessageBox.Show("Please select record to remove.");
                return;
            }
            if (MessageBox.Show("Confirm remove the selected setting parameter?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                for (int i = 0; i < lv_Config.Items.Count; i++)
                {
                    if (lv_Config.Items[i].Checked || lv_Config.Items[i].Selected)
                    {
                        DeleteSetting(lv_Config.Items[i].Text);
                        lv_Config.Items.RemoveAt(i);
                        i = 0;
                    }
                }
                InitConfigInfoList();
            }
        }

        private void DeleteSetting(String strName)
        {
            setting.RemoveSetting(strName);
            String strSettingList = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["SettingList"]);
            if (strSettingList.IndexOf(strName)!=0)
                strSettingList = strSettingList.Replace(";"+strName + ";", ";");
            else
                strSettingList = strSettingList.Replace(strName + ";", ";");
            setting.UpdateSettingList(strSettingList);
        }

        private void btn_ClearAll_Click(object sender, EventArgs e)
        {
            if (frm != null)
            {
                MessageBox.Show("Please finish all setting first.");
                return;
            }
            if (MessageBox.Show("Are you sure to clear all the setting?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                for (int i = 0; i < lv_Config.Items.Count; i++)
                {
                    DeleteSetting(lv_Config.Items[i].Text);
                    //lv_Config.Items.RemoveAt(i);
                    //i = 0;
                }
                InitConfigInfoList();
            }
        }

        private void lv_Config_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            
        }

        public void SaveCheckedList()
        {
            if (lv_Config.CheckedIndices.Count <= 0)
                return;
            String lst_CheckList = setting.GetKeyValue("CheckList");
            lst_CheckList = "";
            for (int i = 0; i < lv_Config.CheckedIndices.Count; i++)
                lst_CheckList += lv_Config.CheckedIndices[i] + ",";
            setting.UpdateKeyValue(lst_CheckList, "CheckList");
        }

        public void SelectConfigList()
        {
            String lst_CheckList = setting.GetKeyValue("CheckList");
            string[] words = lst_CheckList.Split(',');
            try
            {

                for (int i = 0; i < words.Length; i++)
                {
                    lv_Config.Items[int.Parse(words[i])].Checked = true;
                }
            }
            catch (Exception ex)
            {
            }
        }

        public void SetTextOnRichTextBox(String strMessage)
        {
            try
            {
                rtb_Result.SelectionStart = rtb_Result.Text.Length;
                rtb_Result.ScrollToCaret();
                if (strMessage.Equals(".") || strMessage.Equals("\n"))
                    rtb_Result.Text = rtb_Result.Text.Insert(rtb_Result.Text.Length, strMessage);
                else
                    rtb_Result.Text = rtb_Result.Text.Insert(rtb_Result.Text.Length, "[" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + "]" + strMessage + "\n");

            }catch(Exception ex)
            {
                //rtb_Result.Clear();
            }
            rtb_Result.SelectionStart = rtb_Result.Text.Length;
            rtb_Result.ScrollToCaret();
            //this.Refresh();
            WriteToFile(strMessage);
        }

        private void WriteToFile(String message)
        {
            if (message.Equals(".") || message.Equals("\n"))
                return;
            try
            {
                String strFileName = CheckLogFile();
                Encoding encode = System.Text.UnicodeEncoding.Unicode;
                using (StreamWriter sw = new StreamWriter(strFileName, true, encode))
                {
                    sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm   ") + message);
                    sw.Flush();
                    sw.Close();
                }
            }
            catch (Exception ex) { }
        }

        public String CheckLogFile()
        {

            double dFileSize = double.Parse(setting.GetKeyValue("LOG_MAX_FILE_SIZE"));
            String strLogFilePath = setting.GetKeyValue(("LOG_FILE_PATH"));
            if (strLogFilePath.LastIndexOf("\\") != strLogFilePath.Length)
                strLogFilePath = strLogFilePath + "\\";
            int iCurrentLogNo = Int32.Parse(setting.GetKeyValue("CURRENT_LOG_NO"));
            int iNoOfLogFile = Int32.Parse(setting.GetKeyValue("LOG_NO_ROTATE"));
            //serviceTimer.Stop();
            if (!Directory.Exists(strLogFilePath))
                Directory.CreateDirectory(strLogFilePath);
            String strLogFileName = strLogFilePath + "Reuters_" + iCurrentLogNo + "_log.txt";
            FileInfo fileInfo = new FileInfo(strLogFileName);
            try
            {
                if (fileInfo.Length > dFileSize * 1024 * 1024)
                {
                    iCurrentLogNo++;
                    if (iCurrentLogNo > iNoOfLogFile)
                    {
                        iCurrentLogNo = 1;
                        ZipLogFile(strLogFilePath, iCurrentLogNo, iNoOfLogFile);
                    }
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    //WriteToFile(iCurrentLogNo.ToString());
                    config.AppSettings.Settings["CURRENT_LOG_NO"].Value = iCurrentLogNo.ToString();
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                    try
                    {
                        iCurrentLogNo = Int32.Parse(config.AppSettings.Settings["CURRENT_LOG_NO"].Value.ToString());
                    }
                    catch (Exception ex)
                    {
                        iCurrentLogNo = 1;
                    }
                    strLogFileName = strLogFilePath + "Reuters_" + iCurrentLogNo + "_log.txt";
                    fileInfo = new FileInfo(strLogFileName);
                    if (fileInfo.Length > dFileSize * 1024 * 1024)
                    {

                    }
                }
            }
            catch (Exception ex)
            {
                FileStream file = File.Create(strLogFileName);
                file.Close();
            }
            //serviceTimer.Start();
            return strLogFileName;
        }

        private void ZipLogFile(String strLogFilePath, int iCurrentLogNo, int iNoOfLogFile)
        {
            try
            {
                String str_ZipFilePath = strLogFilePath + "Reuters_" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".zip";
                for (int i = iCurrentLogNo; i <= iNoOfLogFile; i++)
                {
                    String sourceFile = strLogFilePath + "Reuters_" + i + "_log.txt";
                    AddFileToZip(str_ZipFilePath, sourceFile);
                    File.Delete(sourceFile);
                }
            }
            catch (Exception ex) { }
        }

        private void AddFileToZip(string zipFilename, string fileToAdd)
        {
            using (Package zip = System.IO.Packaging.Package.Open(zipFilename, FileMode.OpenOrCreate))
            {
                string destFilename = ".\\" + Path.GetFileName(fileToAdd);
                Uri uri = PackUriHelper.CreatePartUri(new Uri(destFilename, UriKind.Relative));
                if (zip.PartExists(uri))
                {
                    zip.DeletePart(uri);
                }
                PackagePart part = zip.CreatePart(uri, "", CompressionOption.Normal);
                using (FileStream fileStream = new FileStream(fileToAdd, FileMode.Open, FileAccess.Read))
                {
                    using (Stream dest = part.GetStream())
                    {
                        CopyStream(fileStream, dest);
                    }
                }
            }
        }
        private const long BUFFER_SIZE = 4096;
        private void CopyStream(System.IO.FileStream inputStream, System.IO.Stream outputStream)
        {
            try
            {
                long bufferSize = inputStream.Length < BUFFER_SIZE ? inputStream.Length : BUFFER_SIZE;
                byte[] buffer = new byte[bufferSize];
                int bytesRead = 0;
                long bytesWritten = 0;
                while ((bytesRead = inputStream.Read(buffer, 0, buffer.Length)) != 0)
                {
                    outputStream.Write(buffer, 0, bytesRead);
                    bytesWritten += bufferSize;
                }
            }
            catch (Exception ex) { }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //this.Refresh();
        }

        private void frm_Main_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Escape)
                btn_Exit_Click(null,null);
        }

        private void nup_TimeInterval_ValueChanged(object sender, EventArgs e)
        {
            setting.UpdateKeyValue( nup_TimeInterval.Value.ToString(), "Interval");
        }

        private void aboutAToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frm_About frm_About = new frm_About();
            frm_About.Show();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frm_About frm_About = new frm_About();
            frm_About.Show();
        }

        private void ThreadFinished()
        {
            try
            {
                myThread.Abort();
            }
            catch (Exception ex) { };
            btn_Start.Enabled = true;
            btn_Stop.Enabled = false;
            btn_Config.Enabled = true;
            btn_AddConfig.Enabled = true;
            btn_Remove.Enabled = true;
            btn_ClearAll.Enabled = true;
            cb_SelectAll.Enabled = true;
            nup_TimeInterval.Enabled = true;
            lv_Config.Enabled = true;
            timer1.Stop();
        }

        private void rtb_Result_TextChanged(object sender, EventArgs e)
        {
            if (rtb_Result.TextLength >= rtb_Result.MaxLength/2 ||rtb_Result.Lines.Length>4000)
                rtb_Result.Text = "";
        }

        private void TKPConfigListDetailStyle()
        {
            lv_Detail.Items.Clear();
            if (lv_Config.SelectedItems.Count == 0)
            {
                return;
            }
            SettingParameter setting = new SettingParameter();
            for (int i = 0; i < lst_SettingParam.Count; i++)
            {
                if (lst_SettingParam[i].strChannelsName_Format.Equals(lv_Config.SelectedItems[0].Text, StringComparison.OrdinalIgnoreCase))
                {
                    CheckSettingPath(i);
                    setting = lst_SettingParam[i];
                }
            }
            ListViewItem itemChannelName = new ListViewItem("Name/Path");
            itemChannelName.SubItems.Add(setting.strChannelsName_Format);
            lv_Detail.Items.Add(itemChannelName);
            ListViewItem itemAgencyName = new ListViewItem("Agency Name");
            if (setting.strAgencyName.Length>0)
                itemAgencyName.SubItems.Add(setting.strAgencyName.Substring(0, setting.strAgencyName.IndexOf("(")));
            lv_Detail.Items.Add(itemAgencyName);
            ListViewItem itemAgencyType = new ListViewItem("Data Type");
            itemAgencyType.SubItems.Add(setting.strAgencyType);
            lv_Detail.Items.Add(itemAgencyType);
            if (setting.bIsChannel)
            {
                ListViewItem itemXMLDownloadPath = new ListViewItem("XML Download Path");
                if (setting.bNeedCopyFile)
                    itemXMLDownloadPath.SubItems.Add(setting.strXMLDownloadPath);
                else
                    itemXMLDownloadPath.SubItems.Add("N/A");
                lv_Detail.Items.Add(itemXMLDownloadPath);
            }
            else
            {
                ListViewItem itemFileCopyPath = new ListViewItem("File Copy Path");
                if (setting.strFileCopyPath.Length>0)
                    itemFileCopyPath.SubItems.Add(setting.strFileCopyPath);
                else
                    itemFileCopyPath.SubItems.Add("N/A");
                lv_Detail.Items.Add(itemFileCopyPath);

                ListViewItem itemDeleteFile = new ListViewItem("Delete Local File");
                if (setting.bNeedDeleteFile)
                    itemDeleteFile.SubItems.Add("Yes");
                else
                    itemDeleteFile.SubItems.Add("No");
                lv_Detail.Items.Add(itemDeleteFile);
            }

            if (!setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
            {
                lv_Detail.Items.Add("Photo Copy Path:");
                ListViewItem itemSourcePhotoCopyPath = new ListViewItem("Source Picture");
                itemSourcePhotoCopyPath.SubItems.Add(setting.strPhotoSourcePath);
                lv_Detail.Items.Add(itemSourcePhotoCopyPath);
                if (this.setting.GetEMSVersion().Equals("EMS4.0"))
                    return;
                ListViewItem itemMiddlePhotoCopyPath = new ListViewItem("Middle Picture");
                itemMiddlePhotoCopyPath.SubItems.Add(setting.strPhotoMiddlePath);
                lv_Detail.Items.Add(itemMiddlePhotoCopyPath);
                ListViewItem itemSmallPhotoCopyPath = new ListViewItem("Small Picture");
                itemSmallPhotoCopyPath.SubItems.Add(setting.strPhotoSmallPath);
                lv_Detail.Items.Add(itemSmallPhotoCopyPath);
            }
        }
        private void FBSConfigListDetailStyle()
        {
            lv_Detail.Items.Clear();
            if (lv_Config.SelectedItems.Count == 0)
            {
                return;
            }
            SettingParameter setting = new SettingParameter();
            for (int i = 0; i < lst_SettingParam.Count; i++)
            {
                if (lst_SettingParam[i].strChannelsName_Format.Equals(lv_Config.SelectedItems[0].Text, StringComparison.OrdinalIgnoreCase))
                {
                    CheckSettingPath(i);
                    setting = lst_SettingParam[i];
                }
            }
            ListViewItem itemChannelName = new ListViewItem("Name/Path");
            itemChannelName.SubItems.Add(setting.strChannelsName_Format);
            lv_Detail.Items.Add(itemChannelName);
            ListViewItem itemWiresID = new ListViewItem("Wires ID");
            itemWiresID.SubItems.Add(setting.strWireID);
            lv_Detail.Items.Add(itemWiresID);
            ListViewItem itemFileCode = new ListViewItem("File Code");
            itemFileCode.SubItems.Add(setting.strFileCode);
            lv_Detail.Items.Add(itemFileCode);
            if (setting.bIsChannel)
            {
                ListViewItem itemXMLDownloadPath = new ListViewItem("XML Download Path");
                if (setting.bNeedCopyFile)
                    itemXMLDownloadPath.SubItems.Add(setting.strXMLDownloadPath);
                else
                    itemXMLDownloadPath.SubItems.Add("N/A");
                lv_Detail.Items.Add(itemXMLDownloadPath);
            }
            else
            {
                ListViewItem itemFileCopyPath = new ListViewItem("File Copy Path");
                if (setting.bNeedCopyFile)
                    itemFileCopyPath.SubItems.Add(setting.strFileCopyPath);
                else
                    itemFileCopyPath.SubItems.Add("N/A");
                lv_Detail.Items.Add(itemFileCopyPath);

                ListViewItem itemDeleteFile = new ListViewItem("Delete Local File");
                if (setting.bNeedDeleteFile)
                    itemDeleteFile.SubItems.Add("Yes");
                else
                    itemDeleteFile.SubItems.Add("No");
                lv_Detail.Items.Add(itemDeleteFile);
            }
        }

        private void MPConfigListDetailStyle()
        {
            lv_Detail.Items.Clear();
            if (lv_Config.SelectedItems.Count == 0)
            {
                return;
            }
            SettingParameter setting = new SettingParameter();
            for (int i = 0; i < lst_SettingParam.Count; i++)
            {
                if (lst_SettingParam[i].strChannelsName_Format.Equals(lv_Config.SelectedItems[0].Text, StringComparison.OrdinalIgnoreCase))
                {
                    CheckSettingPath(i);
                    setting = lst_SettingParam[i];
                }
            }
            ListViewItem itemChannelName = new ListViewItem("Name/Path");
            itemChannelName.SubItems.Add(setting.strChannelsName_Format);
            lv_Detail.Items.Add(itemChannelName);
            ListViewItem itemAgencyName = new ListViewItem("Agency Name");
            if (setting.strAgencyName.Length > 0)
                itemAgencyName.SubItems.Add(setting.strAgencyName.Substring(0, setting.strAgencyName.IndexOf("(")));
            lv_Detail.Items.Add(itemAgencyName);
            ListViewItem itemAgencyType = new ListViewItem("Data Type");
            itemAgencyType.SubItems.Add(setting.strAgencyType);
            lv_Detail.Items.Add(itemAgencyType);
            if (setting.bIsChannel)
            {
                ListViewItem itemXMLDownloadPath = new ListViewItem("XML Download Path");
                if (setting.bNeedCopyFile)
                    itemXMLDownloadPath.SubItems.Add(setting.strXMLDownloadPath);
                else
                    itemXMLDownloadPath.SubItems.Add("N/A");
                lv_Detail.Items.Add(itemXMLDownloadPath);
            }
            else
            {
                ListViewItem itemFileCopyPath = new ListViewItem("File Copy Path");
                if (setting.strFileCopyPath.Length > 0)
                    itemFileCopyPath.SubItems.Add(setting.strFileCopyPath);
                else
                    itemFileCopyPath.SubItems.Add("N/A");
                lv_Detail.Items.Add(itemFileCopyPath);

                ListViewItem itemDeleteFile = new ListViewItem("Delete Local File");
                if (setting.bNeedDeleteFile)
                    itemDeleteFile.SubItems.Add("Yes");
                else
                    itemDeleteFile.SubItems.Add("No");
                lv_Detail.Items.Add(itemDeleteFile);
            }

            if (!setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
            {
                lv_Detail.Items.Add("Photo Copy Path:");
                ListViewItem itemSourcePhotoCopyPath = new ListViewItem("Source Picture");
                itemSourcePhotoCopyPath.SubItems.Add(setting.strPhotoSourcePath);
                lv_Detail.Items.Add(itemSourcePhotoCopyPath);
                if (this.setting.GetEMSVersion().Equals("EMS4.0"))
                    return;
                ListViewItem itemMiddlePhotoCopyPath = new ListViewItem("Middle Picture");
                itemMiddlePhotoCopyPath.SubItems.Add(setting.strPhotoMiddlePath);
                lv_Detail.Items.Add(itemMiddlePhotoCopyPath);
                ListViewItem itemSmallPhotoCopyPath = new ListViewItem("Small Picture");
                itemSmallPhotoCopyPath.SubItems.Add(setting.strPhotoSmallPath);
                lv_Detail.Items.Add(itemSmallPhotoCopyPath);
            }
        }

        public void DestorySetParamForm()
        {
            frm = null;
        }
    }
}
