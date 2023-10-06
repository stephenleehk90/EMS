using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;
/*
 * For different EMS Version, there would be different on layout when setting parameters.
 */
namespace Reuters_with_Web_Service
{
    public partial class frmSetParameter : Form
    {
        private DBHandler DB;
        private SettingParameter setting;
        private SettingHandler settingHandler;
        List<NewsChannel> lst_Channels;
        private bool bIsNew;
        private String strPrevChannelOrPathName;
        private List<ComboItem> lst_AgencyList;
        private frm_Main main;
        private List<FileSpec> lst_FileSpec;
        private frm_FileSpecial frmFileSpecial;
        private List<Priority> lst_Priority;
        private frm_Priority frmPriority;
        private List<int> lst_RootId;

        public frmSetParameter(frm_Main main)
        {
            InitializeComponent();
            button1.Visible = button2.Visible = false;
            setting = new SettingParameter();
            settingHandler = new SettingHandler();
            bIsNew = true;
            lst_FileSpec = new List<FileSpec>();
            lst_Priority = new List<Priority>();
            lst_RootId = new List<int>();
            this.main = main;
            if (settingHandler.GetEMSVersion().Length != 0)
                ResizeLayout(settingHandler.GetEMSVersion());
        }

        private void ResizeLayout(String strEMSVersion)
        {
            switch (strEMSVersion)
            {
                case "EMS6.0"://EMS6.0 would be using default layout
                    defaultLayoutStyle();
                    break;
                case "EMS4.0":
                    EMS4LayoutStyle();
                    break;
                case "EMSFBS":
                    FBSLayoutStyle();
                    break;
                case "EMS6.5":
                    EMS65LayoutStyle();
                    break;
                default:
                    break;
            }
            rb_moveToDir.Checked = true;
        }

        private void defaultLayoutStyle()
        {
            //Pat 20160721 add bloomberg-----------------------------------------------
            cb_XMLType.Visible = true;
// Edit Stephen 2022=08-01
//            cb_XMLType.Items.Remove("AP");
            // Edit Stephen 2022=08-01
            cb_XMLType.Items.Remove("GIS");
            //-------------------------------------------------------------------------
        }

        private void FBSLayoutStyle()
        {
            ClientSize = new System.Drawing.Size(452, 623);
            this.MaximumSize = new System.Drawing.Size(460, 650);
            this.MaximumSize = new Size((int)(this.Size.Width), (int)(this.Size.Height*1.05));
            this.MinimumSize = new Size((int)(this.Size.Width), (int)(this.Size.Height * 1.05));
            this.Size = new Size((int)(this.Size.Width), (int)(this.Size.Height * 1.05));
            gb_DataFromReuters.Size = new Size(gb_DataFromReuters.Size.Width, (int)(gb_DataFromReuters.Size.Height*0.5));
            gb_DataFromFile.Size = new Size(gb_DataFromFile.Size.Width, (int)(gb_DataFromFile.Size.Height * 0.5));
            gb_NamingSetting.Visible = false;
            btn_OK.Location = new System.Drawing.Point(75, 583);
            btn_OK.Location = new Point(btn_OK.Location.X, (int)(btn_OK.Location.Y * 1.05));
            btn_OK.BringToFront();
            btn_Cancel.Location = new System.Drawing.Point(257, 584);
            btn_Cancel.Location = new Point(btn_Cancel.Location.X, (int)(btn_Cancel.Location.Y * 1.05));
            btn_Cancel.BringToFront();
            label14.Location = new Point(label14.Location.X, (int)(label14.Location.Y*1.3));
            label15.Location = new Point(label15.Location.X, (int)(label15.Location.Y * 1.3));
            gb_DataFromReuters.Location = new Point(gb_DataFromReuters.Location.X, (int)(gb_DataFromReuters.Location.Y * 1.3));
            gb_DataFromFile.Location = new Point(gb_DataFromFile.Location.X, (int)(gb_DataFromFile.Location.Y * 1.3));
            label3.Visible = false;
            label2.Visible = false;
            label9.Text = "Wires ID :";
            label16.Visible = true;
            tb_FileCode.Visible = true;
            tb_WiresCode.Visible = true;
            cb_DBTable.Visible = false;
            lst_FileSpecMapping.Columns[1].Text = "Class Name";
            gb_Priority.Visible = true;
            cb_XMLType.Visible = true;
            cb_XMLType.Items.Remove("Bloomberg");
            cb_XMLType.Items.Remove("新華社");
            ckb_ProcessTime.Visible = false;
            dtp_Buffer.Visible = false;
            this.Update();
        }

        private void EMS4LayoutStyle()
        {
            this.MaximumSize = new Size((int)(this.Size.Width), (int)(this.Size.Height * 0.95));
            this.MinimumSize = new Size((int)(this.Size.Width), (int)(this.Size.Height));
            this.Size = new Size((int)(this.Size.Width), (int)(this.Size.Height * 0.99));
            gb_DataFromReuters.Size = new Size(gb_DataFromReuters.Size.Width, (int)(gb_DataFromReuters.Size.Height * 0.75));
            gb_DataFromFile.Size = new Size(gb_DataFromFile.Size.Width, (int)(gb_DataFromFile.Size.Height * 0.75));
            btn_OK.Location = new Point(btn_OK.Location.X, (int)(btn_OK.Location.Y * 0.91));
            //btn_OK.BringToFront();
            btn_Cancel.Location = new Point(btn_Cancel.Location.X, (int)(btn_Cancel.Location.Y * 0.91));
            //btn_Cancel.BringToFront();
            label11.Visible = false;
            label12.Visible = false;
            tb_FMiddlePicPath.Visible = false;
            tb_FSmallPicPath.Visible = false;
            ckb_ProcessTime.Visible = false;
            dtp_Buffer.Visible = false;
            //Pat 20160721 add bloomberg-----------------------------------------------
            //cb_XMLType.Visible = true;
            //-------------------------------------------------------------------------
        }

        private void EMS65LayoutStyle()
        {
           
            tb_FSourcePicPath.Location = new Point(tb_FSourcePicPath.Location.X+100, tb_FSourcePicPath.Location.Y);
            tb_FSourcePicPath.Width = tb_FSourcePicPath.Width - 100;
            tb_FMiddlePicPath.Location = new Point(tb_FMiddlePicPath.Location.X + 100, tb_FMiddlePicPath.Location.Y);
            tb_FMiddlePicPath.Width = tb_FMiddlePicPath.Width - 100;
            tb_FSmallPicPath.Location = new Point(tb_FSmallPicPath.Location.X + 100, tb_FSmallPicPath.Location.Y);
            tb_FSmallPicPath.Width = tb_FSmallPicPath.Width - 100;
            tb_RSourcePicPath.Location = new Point(tb_RSourcePicPath.Location.X + 100, tb_RSourcePicPath.Location.Y);
            tb_RSourcePicPath.Width = tb_RSourcePicPath.Width - 100;
            tb_RMiddlePicPath.Location = new Point(tb_RMiddlePicPath.Location.X + 100, tb_RMiddlePicPath.Location.Y);
            tb_RMiddlePicPath.Width = tb_RMiddlePicPath.Width - 100;
            tb_RSmallPicPath.Location = new Point(tb_RSmallPicPath.Location.X + 100, tb_RSmallPicPath.Location.Y);
            tb_RSmallPicPath.Width = tb_RSmallPicPath.Width - 100;
            cb_MiddleRelativePath.Visible = true;
            cb_MiddleRelativePath.BringToFront();
            cb_SmallRelativePath.Visible = true;
            cb_SmallRelativePath.BringToFront();
            cb_SourceRelativePath.Visible = true;
            cb_SourceRelativePath.BringToFront();
            //Pat 20160721 add bloomberg-----------------------------------------------
            cb_XMLType.Visible = true;
            cb_XMLType.Items.Remove("AP");
            cb_XMLType.Items.Remove("GIS");
            ckb_ProcessTime.Visible = false;
            dtp_Buffer.Visible = false;
            //-------------------------------------------------------------------------
        }
        private void SetupSettingParameter()
        {
            cb_ChannelOrPath.Text = setting.strChannelsName_Format;
            cb_DBTable.Text = setting.strAgencyName;
            tb_FileCopyPath.Text = setting.strFileCopyPath_Format;
            if (settingHandler.GetEMSVersion().Equals("EMS6.5", StringComparison.OrdinalIgnoreCase) && cb_DBTable.Text.IndexOf("Text") < 0)
            {
                tb_FMiddlePicPath.Text = setting.strPhotoMiddlePath_Relative;
                tb_FSmallPicPath.Text = setting.strPhotoSmallPath_Relative;
                tb_FSourcePicPath.Text = setting.strPhotoSourcePath_Relative;
            }
            else
            {
                tb_FMiddlePicPath.Text = setting.strPhotoMiddlePath_Format;
                tb_FSmallPicPath.Text = setting.strPhotoSmallPath_Format;
                tb_FSourcePicPath.Text = setting.strPhotoSourcePath_Format;
                tb_RMiddlePicPath.Text = setting.strPhotoMiddlePath_Format;
                tb_RSmallPicPath.Text = setting.strPhotoSmallPath_Format;
                tb_RSourcePicPath.Text = setting.strPhotoSourcePath_Format;
            }
            ckb_DeleteFile.Checked = setting.bNeedDeleteFile;
            ckb_DownloadXML.Checked = setting.bNeedCopyFile;
            tb_XMLDownLoadPath.Text = setting.strXMLDownloadPath_Format;
            ckb_ProcessTime.Checked = setting.bProcessTime;

            dtp_Buffer.Value = new DateTime(2000,1,1, setting.dtBuffer.Hour, setting.dtBuffer.Minute, 0);
            lst_FileSpec = setting.lst_FileSpec;
            lst_Priority = setting.lst_Priority == null ? lst_Priority : setting.lst_Priority;
            try
            {
                string[] words = setting.str_SubstringName.Split(',');
                nup_First.Value = int.Parse(words[0]);
                nup_Last.Value = int.Parse(words[1]);
            }
            catch (Exception ex)
            {
            }
            if (settingHandler.GetEMSVersion().Equals("EMSFBS"))
            {
                tb_WiresCode.Text = setting.strWireID;
                tb_FileCode.Text = setting.strFileCode;
                if (lst_Priority != null)
                    UpdatePriorityMapping();
            //Pat 20160721 add bloomberg-----------------------------------------------
                //cb_XMLType.SelectedIndex = setting.bIsAP == true ? 1 : 0;                
            }
            if (!setting.strXmlType.Equals(""))
            {
                cb_XMLType.SelectedIndex = cb_XMLType.Items.IndexOf(setting.strXmlType);
            }

            if (setting.BBbIgnoreFile)
                cb_ignoreFile.Checked = true;

            if (setting.BBIgnoreFileAction == 0)
                rb_IFDel.Checked = true;

            if (setting.BBstrMovePath != null)
            {
                tb_movePath.Text = setting.BBstrMovePath;
            }

            if (setting.BBstrIgnoreStr != null)
            {
                tb_ignoreStr.Text = setting.BBstrIgnoreStr;
            }
            //-------------------------------------------------------------------------
            UpdateFileSpecMapping();
        }

        public void SetChannelList(List<NewsChannel> lst_Channels)
        {
            this.lst_Channels = lst_Channels;
            for (int i = 0; i < lst_Channels.Count; i++)
                cb_ChannelOrPath.Items.Add(new ComboItem(lst_Channels[i].strDescrpition, lst_Channels[i].strAlias));
        }

        public void SetSettingParameter(SettingParameter setting)
        {
            this.setting = setting;
            try
            {
                int len = setting.strChannelsName.Length;
                bIsNew = false;
            }
            catch (Exception ex) { bIsNew = true;}
            strPrevChannelOrPathName = setting.strChannelsName;
            if (!bIsNew)
                SetupSettingParameter();
        }

        public void SetDB(DBHandler DB)
        {
            this.DB = DB;
        }

        private void frmSetParameter_Load(object sender, EventArgs e)
        {
            this.KeyPreview = true;
            ckb_DownloadXML_CheckedChanged(null, null);
            SettingHandler a = new SettingHandler();
            DataTable dt = new DataTable();
            try
            {
                lst_AgencyList = new List<ComboItem>();
                dt = DB.GetRecordToDataTable("select * from news_agency");
                for (int i = 0; i < dt.Rows.Count;i++ )
                {
                    lst_AgencyList.Add(new ComboItem(dt.Rows[i].ItemArray[1].ToString().Trim() + "(Text)", dt.Rows[i].ItemArray[0].ToString()));
                }
                dt = new DataTable();
                dt = DB.GetRecordToDataTable("select * from news_agency_photo");
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    lst_AgencyList.Add(new ComboItem(dt.Rows[i].ItemArray[1].ToString().Trim() + "(Picture)", dt.Rows[i].ItemArray[0].ToString()));
                }
            }catch(Exception ex){
                String strError = "Error Catched: " + DB.GetDBError();
                MessageBox.Show(strError, "Error");
            }
            if(settingHandler.GetEMSVersion()=="EMS6.5")
                GetRootPathList();
            cb_ChannelOrPath_Leave(null, null);
            UpdateFileSpecMapping();
        }

        private void LoadAgency()
        {
            cb_DBTable.Items.Clear();
            for (int i = 0; i < lst_AgencyList.Count; i++)
                cb_DBTable.Items.Add(lst_AgencyList[i]);
        }

        private bool Validation()
        {
            bool nRet = true;
            String strErrorMessage = "";
            if (cb_XMLType.Visible && cb_XMLType.SelectedIndex < 0)
                strErrorMessage += "Please select one XML Formal\n";
            if (cb_ChannelOrPath.Text.Length == 0)
                strErrorMessage += "Please choose a channel or type a valided path.\n";
            else
            {
                if (bIsNew)
                {
                    String strSettingList = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["SettingList"]);
                    List<String> lst_Str = new List<String>();
                    string[] words = strSettingList.Split(';');
                    foreach (string word in words)
                    {
                        if (word.Length == 0)
                            continue;
                        lst_Str.Add(word);
                    }
                    String strFind = lst_Str.Find(
                        delegate(String str)
                        {
                            return str == cb_ChannelOrPath.Text;
                        }
                        );
                    if (strFind != null)
                        strErrorMessage += "Repeated setting are not allowed. Please choose another channel or path.\n";
                }
            }
            if (cb_DBTable.Visible == true)
            {
                if (settingHandler.GetEMSVersion().Equals("EMS4.0", StringComparison.OrdinalIgnoreCase) && cb_DBTable.Text.IndexOf("Text") < 0)
                {
                    if (tb_FSourcePicPath.Text.Length == 0)
                        strErrorMessage += "Source image copy path must be input.\n";
                    bool isCorrect = true;
                    Regex r = new Regex(@"^(([a-zA-Z]\:)|(\\))(\\{1}|((\\{1})[^\\]([^/:*?<>""|]*))+)$");
                    if (!r.IsMatch(tb_FSourcePicPath.Text))
                        isCorrect = false;
                    if (!isCorrect)
                        strErrorMessage += "Path was not in corrected format.\n";
                }
                else
                {
                    if (cb_DBTable.SelectedIndex < 0)
                        strErrorMessage += "Please select a agency.\n";
                    if (ckb_DownloadXML.Checked && setting.bIsChannel)
                    {
                        if (tb_XMLDownLoadPath.Text.Length == 0)
                            strErrorMessage += "Please insert the destination link on file copy.\n";
                        else
                        {
                            try
                            {
                                Regex r = new Regex(@"^(([a-zA-Z]\:)|(\\))(\\{1}|((\\{1})[^\\]([^/:*?<>""|]*))+)$");
                                if (!r.IsMatch(tb_XMLDownLoadPath.Text))
                                    strErrorMessage += "XML Download path was not in corrected format.\n";
                            }
                            catch (Exception ex)
                            { strErrorMessage += "Please check the destination link on file copy path is accessable.\n"; }
                        }
                    }
                    if (cb_DBTable.Text.IndexOf("Text") < 0)
                    {
                        if (setting.bIsChannel)
                        {
                            if (tb_RMiddlePicPath.Text.Length == 0 ||
                                tb_RSmallPicPath.Text.Length == 0 ||
                                tb_RSourcePicPath.Text.Length == 0)
                                strErrorMessage += "All picture path must be input.\n";

                            bool isCorrect = true;
                            Regex r = new Regex(@"^(([a-zA-Z]\:)|(\\))(\\{1}|((\\{1})[^\\]([^/:*?<>""|]*))+)$");
                            if (!r.IsMatch(tb_RMiddlePicPath.Text))
                                isCorrect = false;
                            if (!r.IsMatch(tb_RSmallPicPath.Text))
                                isCorrect = false;
                            if (!r.IsMatch(tb_RSourcePicPath.Text))
                                isCorrect = false;
                            if (!isCorrect)
                                strErrorMessage += "Path was not in corrected format.\n";
                        }
                        else
                        {
                            if (settingHandler.GetEMSVersion().Equals("EMS6.5", StringComparison.OrdinalIgnoreCase) && cb_DBTable.Text.IndexOf("Text") < 0)
                            {
                                if (tb_FMiddlePicPath.Text.Length == 0 ||
                                    tb_FSmallPicPath.Text.Length == 0 ||
                                    tb_FSourcePicPath.Text.Length == 0 ||
                                    cb_MiddleRelativePath.Text.Length == 0 ||
                                    cb_SmallRelativePath.Text.Length == 0 ||
                                    cb_SourceRelativePath.Text.Length == 0)
                                    strErrorMessage += "All picture path must be input.\n";
                            }
                            else
                            {
                                if (tb_FMiddlePicPath.Text.Length == 0 ||
                                    tb_FSmallPicPath.Text.Length == 0 ||
                                    tb_FSourcePicPath.Text.Length == 0)
                                    strErrorMessage += "All picture path must be input.\n";
                                bool isCorrect = true;
                                Regex r = new Regex(@"^(([a-zA-Z]\:)|(\\))(\\{1}|((\\{1})[^\\]([^/:*?<>""|]*))+)$");
                                if (!r.IsMatch(tb_FMiddlePicPath.Text))
                                    isCorrect = false;
                                if (!r.IsMatch(tb_FSmallPicPath.Text))
                                    isCorrect = false;
                                if (!r.IsMatch(tb_FSourcePicPath.Text))
                                    isCorrect = false;
                                if (!isCorrect)
                                    strErrorMessage += "Path was not in corrected format.\n";
                            }
                        }
                    }
                }
            }

            try
            {
                if (tb_FileCopyPath.Text.Length != 0)
                {
                    Regex r = new Regex(@"^(([a-zA-Z]\:)|(\\))(\\{1}|((\\{1})[^\\]([^/:*?<>""|]*))+)$");
                    if (!r.IsMatch(tb_FileCopyPath.Text))
                        strErrorMessage += "File copy path was not in corrected format.\n";
                }
            }
            catch (Exception ex)
            { strErrorMessage += "Please check the destination link on file copy path is accessable.\n"; }


            if (cb_XMLType.Text != "GIS")
            {
                if (tb_FileCode.Visible == true)
                {
                    if (tb_FileCode.Text.Length == 0)
                        strErrorMessage += "Please enter File Code.\n";
                    Regex r = new Regex(@"^[0-9a-zA-Z]*$");
                    if (!r.IsMatch(tb_FileCode.Text))
                        strErrorMessage += "Other symbol can not accept on File Code.\n";
                }
            }

            if (tb_WiresCode.Visible == true)
            {
                if (tb_WiresCode.Text.Length == 0)
                    strErrorMessage += "Please enter Wires ID.\n";
                Regex r = new Regex(@"^[0-9a-zA-Z]*$");
                if (!r.IsMatch(tb_WiresCode.Text))
                    strErrorMessage += "Other symbol can not accept on Wires ID.\n";
            }

            try
            {
                String strAgencyType = "";
                strAgencyType = ((ComboItem)cb_DBTable.SelectedItem).Name;
                strAgencyType = strAgencyType.Substring(strAgencyType.IndexOf('('));
                for (int i = 0; i < lst_FileSpec.Count; i++)
                {
                    if (lst_FileSpec[i].strAgencyTable.IndexOf(strAgencyType) == -1)
                    {
                        strErrorMessage += "File Special not match with default agency type.\n";
                    }
                }
            }
            catch (Exception ex) { }
            //Pat 20160812 ignore file------------------------------------------------------------------------------
            
            if (cb_ignoreFile.Checked && rb_moveToDir.Checked)
            {
                Regex r = new Regex(@"^(([a-zA-Z]\:)|(\\))(\\{1}|((\\{1})[^\\]([^/:*?<>""|]*))+)$");
                if (!r.IsMatch(tb_movePath.Text))
                    strErrorMessage += "Ignore file move path was not in corrected format.\n";
            }
            if (tb_ignoreStr.Text.Contains(";"))
            {
                strErrorMessage += "Ignore file name cannot contain ','.\n";
            }
            //------------------------------------------------------------------------------------------------------
            if (strErrorMessage.Length > 0)
            {
                MessageBox.Show(strErrorMessage, "Error");
                nRet = false;
            }
            return nRet;
        }

        private void btn_OK_Click(object sender, EventArgs e)
        {
            if (!Validation())
                return;
            setting.strChannelsName = cb_ChannelOrPath.Text;
            setting.strChannelsName_Format = RefindPath(cb_ChannelOrPath.Text);
            setting.bIsChannel = false;

            setting.dtBuffer = dtp_Buffer.Value;
            setting.bProcessTime = ckb_ProcessTime.Checked;

            if (cb_ChannelOrPath.FindString(setting.strChannelsName) > -1)
            {
                setting.strAliasCode = ((ComboItem)cb_ChannelOrPath.Items[cb_ChannelOrPath.SelectedIndex]).Value.ToString();
                setting.bIsChannel = true;
                setting.bNeedCopyFile = ckb_DownloadXML.Checked;
                setting.bNeedDeleteFile = false;
                if (settingHandler.GetEMSVersion().Equals("EMS6.5", StringComparison.OrdinalIgnoreCase) && cb_DBTable.Text.IndexOf("Text") < 0)
                {
                    setting.strPhotoSourcePath = RefindPath(cb_SourceRelativePath.Text + tb_RSourcePicPath.Text);
                    setting.strPhotoSmallPath = RefindPath(cb_SmallRelativePath.Text + tb_RSmallPicPath.Text);
                    setting.strPhotoMiddlePath = RefindPath(cb_MiddleRelativePath.Text + tb_RMiddlePicPath.Text);
                    setting.strXMLDownloadPath = RefindPath(tb_XMLDownLoadPath.Text);
                    setting.strPhotoSourcePath_Format = RefindPath(cb_SourceRelativePath.Text + tb_RSourcePicPath.Text);
                    setting.strXMLDownloadPath_Format = RefindPath(tb_XMLDownLoadPath.Text);
                    setting.strPhotoSmallPath_Format = RefindPath(cb_SmallRelativePath.Text + tb_RSmallPicPath.Text);
                    setting.strPhotoMiddlePath_Format = RefindPath(cb_MiddleRelativePath.Text + tb_RMiddlePicPath.Text);
                }
                else
                {
                    setting.strPhotoSourcePath = RefindPath(tb_RSourcePicPath.Text);
                    setting.strPhotoSmallPath = RefindPath(tb_RSmallPicPath.Text);
                    setting.strPhotoMiddlePath = RefindPath(tb_RMiddlePicPath.Text);
                    setting.strXMLDownloadPath = RefindPath(tb_XMLDownLoadPath.Text);
                    setting.strPhotoSourcePath_Format = RefindPath(tb_RSourcePicPath.Text);
                    setting.strXMLDownloadPath_Format = RefindPath(tb_XMLDownLoadPath.Text);
                    setting.strPhotoSmallPath_Format = RefindPath(tb_RSmallPicPath.Text);
                    setting.strPhotoMiddlePath_Format = RefindPath(tb_RMiddlePicPath.Text);

                }
            }
            else
            {
                try
                {
                    /*DirectoryInfo dirInfo = new DirectoryInfo(cb_ChannelOrPath.Text);
                    if (!dirInfo.Exists)
                    {
                        MessageBox.Show("Inputed path does not found");
                        return;
                    }*/
                }
                catch (Exception ex)
                { return; }
                setting.strAliasCode = "";
                setting.bNeedCopyFile = false;
                setting.bNeedDeleteFile = ckb_DeleteFile.Checked;
                setting.strPhotoSourcePath = RefindPath(tb_FSourcePicPath.Text);
                setting.strPhotoSmallPath = RefindPath(tb_FSmallPicPath.Text);
                setting.strPhotoMiddlePath = RefindPath(tb_FMiddlePicPath.Text);
                setting.strFileCopyPath = RefindPath(tb_FileCopyPath.Text);
                setting.strFileCopyPath_Format = RefindPath(tb_FileCopyPath.Text);
                if (settingHandler.GetEMSVersion().Equals("EMS6.5", StringComparison.OrdinalIgnoreCase) && cb_DBTable.Text.IndexOf("Text") < 0)
                {
                    setting.strPhotoSourcePath = RefindPath(cb_SourceRelativePath.Text + tb_FSourcePicPath.Text);
                    setting.strPhotoSmallPath = RefindPath(cb_SmallRelativePath.Text + tb_FSmallPicPath.Text);
                    setting.strPhotoMiddlePath = RefindPath(cb_MiddleRelativePath.Text + tb_FMiddlePicPath.Text);
                    setting.strPhotoMiddlePath_Format = RefindPath(cb_MiddleRelativePath.Text + tb_FMiddlePicPath.Text);
                    setting.strPhotoSmallPath_Format = RefindPath(cb_SmallRelativePath.Text + tb_FSmallPicPath.Text);
                    setting.strPhotoSourcePath_Format = RefindPath(cb_SourceRelativePath.Text + tb_FSourcePicPath.Text);
                    setting.iSourcePathID = lst_RootId[cb_SourceRelativePath.SelectedIndex];
                    setting.iMiddlePathID = lst_RootId[cb_MiddleRelativePath.SelectedIndex];
                    setting.iSmallPathID = lst_RootId[cb_SmallRelativePath.SelectedIndex];
                    setting.strPhotoSmallPath_Relative = tb_FSmallPicPath.Text;
                    setting.strPhotoMiddlePath_Relative = tb_FMiddlePicPath.Text;
                    setting.strPhotoSourcePath_Relative = tb_FSourcePicPath.Text;
                    setting.strPhotoSourcePath_Root = cb_SourceRelativePath.Text;
                    setting.strPhotoSmallPath_Root = cb_SmallRelativePath.Text;
                    setting.strPhotoMiddlePath_Root = cb_MiddleRelativePath.Text;
                }
                else
                {
                    setting.strPhotoMiddlePath_Format = RefindPath(tb_FMiddlePicPath.Text);
                    setting.strPhotoSmallPath_Format = RefindPath(tb_FSmallPicPath.Text);
                    setting.strPhotoSourcePath_Format = RefindPath(tb_FSourcePicPath.Text);
                }
            }

            if (cb_DBTable.Visible == true)
            {
                setting.strAgencyName = ((ComboItem)cb_DBTable.SelectedItem).Name;
                setting.strAgencyTableCode = ((ComboItem)cb_DBTable.SelectedItem).Value;
                setting.strAgencyType = ((ComboItem)cb_DBTable.SelectedItem).Name.Substring(((ComboItem)cb_DBTable.SelectedItem).Name.IndexOf("(") + 1).Trim();
                if (setting.strAgencyType.LastIndexOf(")") == setting.strAgencyType.Length - 1)
                    setting.strAgencyType = setting.strAgencyType.Remove(setting.strAgencyType.Length - 1);
                // Edit Stephen 2022-08-13
                setting.bIsAP = cb_XMLType.Text == "AP";
                // Edit Stephen 2022-08-13
            
            }
            else
            {
                //For Metro
                setting.strWireID = tb_WiresCode.Text;
                setting.strFileCode = tb_FileCode.Text;
                setting.strAgencyType = "Text";
                setting.bIsAP = cb_XMLType.Text == "AP";
                setting.bIsGIS = cb_XMLType.Text == "GIS";
                //end
            }
            //Pat 20160721 add bloomberg-----------------------------------------------
            if (!cb_XMLType.Text.Equals(""))
            {
                setting.strXmlType = cb_XMLType.Text;
            }
            else
                setting.strXmlType = "";
            setting.BBbIgnoreFile = cb_ignoreFile.Checked;
            if (rb_moveToDir.Checked)
            {
                setting.BBIgnoreFileAction = 1;
                setting.BBstrMovePath = tb_movePath.Text;
            }
            else
                setting.BBIgnoreFileAction = 0;

            setting.BBstrIgnoreStr = tb_ignoreStr.Text;
            //-------------------------------------------------------------------------
            setting.lst_FileSpec = this.lst_FileSpec;
            String strSettingList = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["SettingList"]);

            setting.str_SubstringName = nup_First.Value.ToString()+","+nup_Last.Value.ToString();
            setting.lst_Priority = lst_Priority;
            if (bIsNew)
            {
                settingHandler.AddSetting(setting);
                strSettingList += setting.strChannelsName + ";";
                settingHandler.UpdateSettingList(strSettingList);
                settingHandler.UpdateSetting(setting);
            }
            else
            {
                if (!setting.strChannelsName.Equals(strPrevChannelOrPathName, StringComparison.OrdinalIgnoreCase) && !bIsNew)
                {
                    strSettingList = strSettingList.Replace(strPrevChannelOrPathName+";", setting.strChannelsName+";");
                    settingHandler.RemoveSetting(strPrevChannelOrPathName);
                    settingHandler.AddSetting(setting);
                    settingHandler.UpdateSettingList(strSettingList);
                }
                else
                {
                    settingHandler.UpdateSetting(setting);
                }
            }
            main.InitConfigInfoList();
            main.DestorySetParamForm();
            this.Close();
        }

        private String RefindPath(String strPath)
        {
            strPath = strPath.Replace("%y", "%Y");
            strPath = strPath.Replace("%m", "%M");
            strPath = strPath.Replace("%d", "%D");
            return strPath;
        }

        private void btn_Cancel_Click(object sender, EventArgs e)
        {
            main.DestorySetParamForm();
            this.Close();
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked == true)
            {
                gb_DataFromFile.Visible = false;
                gb_DataFromReuters.Visible = true;
            }
            else
            {
                gb_DataFromFile.Visible = true;
                gb_DataFromReuters.Visible = false;
            }
        }

        private void FilterAgency(String strFilter)
        {
            bool bIsFilterText = true;
            if (strFilter.Equals("TXT", StringComparison.OrdinalIgnoreCase))
                bIsFilterText = false;
            for (int i = 0; i < cb_DBTable.Items.Count; i++)
            {
                if (bIsFilterText)
                {
                    if (cb_DBTable.Items[i].ToString().IndexOf("(Text)") > -1)
                    {
                        cb_DBTable.Items.RemoveAt(i);
                        i = -1;
                    }
                }
                else
                {
                    if (cb_DBTable.Items[i].ToString().IndexOf("(Text)") == -1)
                    {
                        cb_DBTable.Items.RemoveAt(i);
                        i = -1;
                    }
                }
            }
        }

        private void cb_ChannelOrPath_Leave(object sender, EventArgs e)
        {
            LoadAgency();
            bool bFound = false;
            for(int i=0;i<cb_ChannelOrPath.Items.Count;i++)
            {
                if (((ComboItem)cb_ChannelOrPath.Items[i]).Name.Equals(cb_ChannelOrPath.Text, StringComparison.OrdinalIgnoreCase))
                {
                    for (int j = 0; j < lst_Channels.Count; j++)
                    {
                        if (lst_Channels[j].strDescrpition.Equals(cb_ChannelOrPath.Text, StringComparison.OrdinalIgnoreCase))
                        {
                            if (lst_Channels[j].strCateId.Equals("TXT", StringComparison.OrdinalIgnoreCase))
                                FilterAgency("TXT");
                            else
                                FilterAgency("PIC");
                            setting.bIsChannel = true;
                            bFound = true;
                            break;
                        }
                    }
                    if (bFound)
                        break;
                }else
                    setting.bIsChannel = false;
            }
            if (setting.bIsChannel)
            {
                gb_DataFromReuters.Visible = true;
                gb_DataFromFile.Visible = false;
            }
            else
            {
                gb_DataFromReuters.Visible = false;
                gb_DataFromFile.Visible = true;
            }
            try
            {
                if (setting.strAgencyName.Length != 0)
                {
                    for (int i = 0; i < cb_DBTable.Items.Count; i++)
                        if (((ComboItem)cb_DBTable.Items[i]).Name.Equals(setting.strAgencyName, StringComparison.OrdinalIgnoreCase))
                        {
                            cb_DBTable.SelectedIndex = i;
                            break;
                        }
                }
            }catch(Exception ex){}
            cb_DBTable.Focus();
        }

        private void frmSetParameter_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Escape)
                this.Close();
        }

        private void ckb_DownloadXML_CheckedChanged(object sender, EventArgs e)
        {
            if (ckb_DownloadXML.Checked)
                tb_XMLDownLoadPath.Enabled = true;
            else
                tb_XMLDownLoadPath.Enabled = false;
        }

        private void SetFileSpecMapping()
        {

        }

        private void btn_Add_Click(object sender, EventArgs e)
        {
            if (frmFileSpecial != null)
                return;
            frmFileSpecial = new frm_FileSpecial();
            frmFileSpecial.SetFrmParameter(this);
            frmFileSpecial.SetEMSVersion(settingHandler.GetEMSVersion());
            frmFileSpecial.SetAgencyList(lst_AgencyList);
            frmFileSpecial.Show();
        }

        private void btn_Config_Click(object sender, EventArgs e)
        {
            if (frmFileSpecial != null)
                return;
            if(lst_FileSpecMapping.SelectedItems.Count==0)
                return;
            frmFileSpecial = new frm_FileSpecial();
            frmFileSpecial.SetFrmParameter(this);
            frmFileSpecial.SetEMSVersion(settingHandler.GetEMSVersion());
            frmFileSpecial.SetAgencyList(lst_AgencyList);
            frmFileSpecial.SetConfig(lst_FileSpec[lst_FileSpecMapping.SelectedItems[0].Index].strFileSpecName,
                lst_FileSpec[lst_FileSpecMapping.SelectedItems[0].Index].strAgencyTable, lst_FileSpec[lst_FileSpecMapping.SelectedItems[0].Index].isUnique);
            frmFileSpecial.Show();
        }

        public List<FileSpec> GetFileSpecList()
        {
            return lst_FileSpec;
        }

        public List<Priority> GetPriorityList()
        {
            return lst_Priority;
        }

        private void btn_Remove_Click(object sender, EventArgs e)
        {
            if (lst_FileSpecMapping.SelectedItems.Count == 0)
                return;
            if (MessageBox.Show("Confirm remove the selected setting parameter?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                lst_FileSpec.RemoveAt(lst_FileSpecMapping.SelectedItems[0].Index);
                UpdateFileSpecMapping();
            }
        }

        public void AddFileSpec(String strFileSpecName, String strAgencyTable, String strAgencyCode, bool isUnqiue)
        {
            FileSpec fileSpec = new FileSpec();
            fileSpec.strAgencyCode = strAgencyCode;
            fileSpec.strAgencyTable = strAgencyTable;
            fileSpec.strFileSpecName = strFileSpecName;
            fileSpec.isUnique = isUnqiue;
            lst_FileSpec.Add(fileSpec);
            UpdateFileSpecMapping();
        }

        public void UpdateFileSpec(String strFileSpecName, String strAgencyTable, String strAgencyCode, bool isUnqiue)
        {
            for (int i = 0; i < lst_FileSpec.Count; i++)
            {
                if (lst_FileSpec[i].strFileSpecName.Equals(strFileSpecName, StringComparison.OrdinalIgnoreCase))
                {
                    lst_FileSpec[i].strAgencyTable = strAgencyTable;
                    lst_FileSpec[i].strAgencyCode = strAgencyCode;
                    lst_FileSpec[i].isUnique = isUnqiue;
                    UpdateFileSpecMapping();
                    break;
                }
            }
        }

        public void AddPriority(int iTagValue, int iPriorityValue)
        {
            Priority priority = new Priority();
            priority.iTagValue = iTagValue;
            priority.iPriorityValue = iPriorityValue;
            lst_Priority.Add(priority);
            UpdatePriorityMapping();
        }

        public void UpdatePriority(int iTagValue, int iPriorityValue)
        {
            for (int i = 0; i < lst_Priority.Count; i++)
            {
                if (lst_Priority[i].iTagValue == iTagValue)
                {
                    lst_Priority[i].iPriorityValue = iPriorityValue;
                    UpdatePriorityMapping();
                    break;
                }
            }
        }

        private void UpdatePriorityMapping()
        {
            lst_PriorityMapping.Items.Clear();
            for (int i = 0; i < lst_Priority.Count; i++)
            {
                ListViewItem item = new ListViewItem(lst_Priority[i].iTagValue.ToString());
                item.SubItems.Add(lst_Priority[i].iPriorityValue.ToString());
                lst_PriorityMapping.Items.Add(item);
            }
        }

        private void UpdateFileSpecMapping()
        {
            lst_FileSpecMapping.Items.Clear();
            for(int i=0;i<lst_FileSpec.Count;i++)
            {
                ListViewItem item = new ListViewItem(lst_FileSpec[i].strFileSpecName);
                item.SubItems.Add(lst_FileSpec[i].strAgencyTable);
                lst_FileSpecMapping.Items.Add(item);
            }
        }

        private void cb_DBTable_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cb_DBTable.Text.IndexOf("text") > -1 || cb_DBTable.Text.IndexOf("Text") > -1)
                gb_NamingSetting.Enabled = true;
            else
                gb_NamingSetting.Enabled = false;
        }

        private void lst_FileSpecMapping_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            btn_Config_Click(null,null);
        }

        public void DestoryFileSpecForm()
        {
            frmFileSpecial = null;
        }

        public void DestoryPriorityForm()
        {
            frmPriority = null;
        }

        private void frmSetParameter_FormClosed(object sender, FormClosedEventArgs e)
        {
            main.DestorySetParamForm();
        }

        private void frmSetParameter_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (frmFileSpecial != null)
            {
                frmFileSpecial.Close();
                frmFileSpecial = null;
            }
            if (frmPriority != null)
            {
                frmPriority.Close();
                frmPriority = null;
            }
        }

        private void btn_PAdd_Click(object sender, EventArgs e)
        {
            if (frmPriority != null)
                return;
            frmPriority = new frm_Priority();
            frmPriority.SetFrmParameter(this);
            frmPriority.Show();
        }

        private void btn_PConfig_Click(object sender, EventArgs e)
        {
            if (frmPriority != null)
                return;
            if(lst_PriorityMapping.SelectedItems.Count==0)
                return;
            frmPriority = new frm_Priority();
            frmPriority.SetConfig(lst_Priority[lst_PriorityMapping.SelectedIndices[0]].iTagValue, 
                lst_Priority[lst_PriorityMapping.SelectedIndices[0]].iPriorityValue);
            frmPriority.SetFrmParameter(this);
            frmPriority.Show();
        }

        private void lst_PriorityMapping_DoubleClick(object sender, EventArgs e)
        {
            btn_PConfig_Click(null,null);
        }

        private void btn_PRemove_Click(object sender, EventArgs e)
        {
            if (lst_PriorityMapping.SelectedItems.Count == 0)
                return;
            if (MessageBox.Show("Confirm remove the selected setting parameter?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                lst_Priority.RemoveAt(lst_PriorityMapping.SelectedItems[0].Index);
                UpdatePriorityMapping();
            }
        }

        private void GetRootPathList()
        {
            List<ComboBox> lst_Combo = new List<ComboBox>();
            lst_Combo.Add(cb_SourceRelativePath);
            lst_Combo.Add(cb_MiddleRelativePath);
            lst_Combo.Add(cb_SmallRelativePath);
            List<String> lst_RootPath = new List<String>();
            lst_RootPath = DB.GetRootPathList();
            for (int c = 0; c < lst_Combo.Count; c++)
            {
                for (int i = 0; i < lst_RootPath.Count; i++)
                {
                    try
                    {
                        List<String> lst_Temp = new List<string>();
                        lst_Temp =  lst_RootPath[i].Split(';').ToList();
                        lst_RootId.Add(Int32.Parse(lst_Temp[0]));
                        lst_Combo[c].Items.Add(lst_Temp[1]);
                    }
                    catch (Exception ex)
                    { }
                }
            }

            for (int i = 0; i < cb_SourceRelativePath.Items.Count; i++)
            {
                if(cb_SourceRelativePath.Items[i].ToString().Equals(setting.strPhotoSourcePath_Root, StringComparison.OrdinalIgnoreCase))
                {
                    cb_SourceRelativePath.SelectedIndex = i;
                    break;
                }
            }
            for (int i = 0; i < cb_MiddleRelativePath.Items.Count; i++)
            {
                if(cb_MiddleRelativePath.Items[i].ToString().Equals(setting.strPhotoMiddlePath_Root, StringComparison.OrdinalIgnoreCase))
                {
                    cb_MiddleRelativePath.SelectedIndex = i;
                    break;
                }
            }
            for (int i = 0; i < cb_SmallRelativePath.Items.Count; i++)
            {
                if(cb_SmallRelativePath.Items[i].ToString().Equals(setting.strPhotoSmallPath_Root, StringComparison.OrdinalIgnoreCase))
                {
                    cb_SmallRelativePath.SelectedIndex = i;
                    break;
                }
            }
        }
        //Pat 20160812 ignore file-----------------------------------------------------------------------
        private void cb_XMLType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cb_XMLType.Text.Equals("Bloomberg"))
            {
                gb_BBIgnoreFile.Visible = true;
            }
            else
            {
                gb_BBIgnoreFile.Visible = false;
            }
        }

        private void cb_ignoreFile_CheckedChanged(object sender, EventArgs e)
        {
            if (cb_ignoreFile.Checked)
            {
                rb_IFDel.Enabled = true;
                rb_moveToDir.Enabled = true;
            }
            else
            {
                rb_IFDel.Enabled = false;
                rb_moveToDir.Enabled = false;
            }
        }

        private void rb_moveToDir_CheckedChanged(object sender, EventArgs e)
        {
            if (rb_moveToDir.Checked)
                tb_movePath.Enabled = true;
            else
                tb_movePath.Enabled = false;
        }
        //------------------------------------------------------------------------------------------------
    }
}

public class FileSpec
{
    public String strFileSpecName;
    public String strAgencyTable; // on FBS version, this value will be same as strAgencyCode but meanning will be different
    public String strAgencyCode; // on FNS version, this value will be "Class" name
    public bool isUnique;
}

public class Priority
{
    public int iTagValue;
    public int iPriorityValue;
}

public class ComboItem
{
    public string Name;
    public string Value;
    public ComboItem(string name, string value)
    {
        Name = name; Value = value;
    }
    public override string ToString()
    {
        // Generates the text shown in the combo box
        return Name;
    }
}