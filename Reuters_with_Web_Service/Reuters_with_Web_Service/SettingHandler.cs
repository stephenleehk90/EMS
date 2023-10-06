using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using System.IO;

namespace Reuters_with_Web_Service
{
    class SettingHandler
    {
        int iInterval;// Time Interval between each round
        private List<SettingParameter> lst_SettingParameter;
        private String strEmsVersion;
        private String strProductName;
        private String strDBVersion;
        private List<String> lst_SourcePhotoSizeType;

        public SettingHandler()
        {
            lst_SettingParameter = new List<SettingParameter>();
            lst_SourcePhotoSizeType = new List<String>();
            LoadSetting();
            strEmsVersion = GetKeyValue("EMS_Version");
            strProductName = GetKeyValue("Product_Name");
            strDBVersion = GetKeyValue("DB_Version");
            iInterval = 1;
        }

        public void RemoveSetting(String strKeyName)
        {
            RemoveKeySettingOnConfigFile(strKeyName);
        }
        
        private void RemoveKeySettingOnConfigFile(String strKeyName)
        {
            System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            config.AppSettings.Settings.Remove(strKeyName);
            config.Save(ConfigurationSaveMode.Full);
            ConfigurationManager.RefreshSection("appSettings");
        }

        private void AddKeySettingToConfigFile( String strValue, String strKeyName)
        {
            System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            config.AppSettings.Settings.Add(strKeyName, strValue);
            config.Save(ConfigurationSaveMode.Full);
            ConfigurationManager.RefreshSection("appSettings");
            config.AppSettings.Settings[strKeyName].Value = strValue;
            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");
        }

        public String GetEMSVersion()
        {
            return strEmsVersion;
        }

        public String GetProductName()
        {
            return strProductName;
        }

        public void AddSetting(SettingParameter setting)
        {
            String strFileSpec = "", strPriority = "";
            for (int i = 0; i < setting.lst_FileSpec.Count; i++)
            {
                strFileSpec += setting.lst_FileSpec[i].strFileSpecName + "," + setting.lst_FileSpec[i].strAgencyTable + "," + setting.lst_FileSpec[i].strAgencyCode + ","+(setting.lst_FileSpec[i].isUnique?"U":"N")+"|";
            }
            for (int i = 0; i < setting.lst_Priority.Count; i++)
            {
                strPriority += setting.lst_Priority[i].iTagValue + "," + setting.lst_Priority[i].iPriorityValue + "|";
            }
            String strValue = String.Format("name={0};alisa={1};agencyTable={2};agencyType={3};"+
                "isChannel={4};needCopyFile={5};needDeleteFile={6};fileCopyFile={7};photoSourcePath={8};"+
                "photoMiddlePath={9};photoSmallPath={10};XMLDownloadPath={11};agencyName={12};WiresID={13};"+
                "FileCode={14};FileSpec={15};NameSubString={16};Priority={17};IsAP={18};XmlType={19};BBbIgnoreFile={20};BBIgnoreFileAction={21};BBstrMovePath={22};BBstrIgnoreStr={23};IsGIS={24};IsProcessingTime={25};BufferTime={26}",
                setting.strChannelsName, setting.strAliasCode, setting.strAgencyTableCode, setting.strAgencyType,
                setting.bIsChannel, setting.bNeedCopyFile, setting.bNeedDeleteFile, setting.strFileCopyPath, setting.strPhotoSourcePath, 
                setting.strPhotoMiddlePath, setting.strPhotoSmallPath, setting.strXMLDownloadPath, setting.strAgencyName, setting.strWireID,
                setting.strFileCode, strFileSpec, setting.str_SubstringName, strPriority, setting.bIsAP, setting.strXmlType, setting.BBbIgnoreFile, setting.BBIgnoreFileAction, setting.BBstrMovePath, setting.BBstrIgnoreStr, setting.bIsGIS, setting.bProcessTime, setting.dtBuffer.ToString("HH:mm"));
            if (GetEMSVersion() == "EMS6.5")
            {
                if (setting.strPhotoMiddlePath_Root.Length > 0 ||
                    setting.strPhotoSmallPath_Root.Length > 0 ||
                    setting.strPhotoSourcePath_Root.Length > 0)
                {
                    strValue += ";SourecRoot=" + setting.strPhotoSourcePath_Root +
                        ";MiddleRoot=" + setting.strPhotoMiddlePath_Root +
                        ";SmallRoot=" + setting.strPhotoSmallPath_Root;
                    strValue += ";SourecRelative=" + setting.strPhotoSourcePath_Relative +
                       ";MiddleRelative=" + setting.strPhotoMiddlePath_Relative +
                       ";SmallRelative=" + setting.strPhotoSmallPath_Relative;
                    strValue += ";SourecRootID=" + setting.iSourcePathID.ToString() +
                       ";MiddleRootID=" + setting.iMiddlePathID.ToString() +
                       ";SmallRootID=" + setting.iSmallPathID.ToString();
                }
            }

            AddKeySettingToConfigFile(strValue, setting.strChannelsName);
        }

        public void UpdateSetting(SettingParameter setting)
        {
            String strFileSpec = "", strPriority = "";
            for (int i = 0; i < setting.lst_FileSpec.Count; i++)
            {
                strFileSpec += setting.lst_FileSpec[i].strFileSpecName + "," + setting.lst_FileSpec[i].strAgencyTable + "," + setting.lst_FileSpec[i].strAgencyCode + "," + (setting.lst_FileSpec[i].isUnique ? "U" : "N") + "|";
            }
            for (int i = 0; i < setting.lst_Priority.Count; i++)
            {
                strPriority += setting.lst_Priority[i].iTagValue + "," + setting.lst_Priority[i].iPriorityValue + "|";
            }
            String strValue = String.Format("name={0};alisa={1};agencyTable={2};agencyType={3};isChannel={4};needCopyFile={5};needDeleteFile={6};fileCopyFile={7};photoSourcePath={8};photoMiddlePath={9};photoSmallPath={10};XMLDownloadPath={11};agencyName={12};WiresID={13};FileCode={14};FileSpec={15};NameSubString={16};Priority={17};IsAP={18};XmlType={19};BBbIgnoreFile={20};BBIgnoreFileAction={21};BBstrMovePath={22};BBstrIgnoreStr={23};IsGIS={24};IsProcessingTime={25};BufferTime={26}", 
               setting.strChannelsName, setting.strAliasCode, setting.strAgencyTableCode, setting.strAgencyType, setting.bIsChannel, setting.bNeedCopyFile, setting.bNeedDeleteFile,
               setting.strFileCopyPath, setting.strPhotoSourcePath, setting.strPhotoMiddlePath, setting.strPhotoSmallPath, setting.strXMLDownloadPath, setting.strAgencyName, setting.strWireID, setting.strFileCode, strFileSpec, setting.str_SubstringName, strPriority, setting.bIsAP, setting.strXmlType, setting.BBbIgnoreFile, setting.BBIgnoreFileAction, setting.BBstrMovePath, setting.BBstrIgnoreStr, setting.bIsGIS, setting.bProcessTime, setting.dtBuffer.ToString("HH:mm"));
            if (GetEMSVersion() == "EMS6.5")
            {
                if (setting.strPhotoMiddlePath_Root.Length > 0 ||
                setting.strPhotoSmallPath_Root.Length > 0 ||
                setting.strPhotoSourcePath_Root.Length > 0)
                {
                    strValue += ";SourecRoot=" + setting.strPhotoSourcePath_Root +
                        ";MiddleRoot=" + setting.strPhotoMiddlePath_Root +
                        ";SmallRoot=" + setting.strPhotoSmallPath_Root;
                    strValue += ";SourecRelative=" + setting.strPhotoSourcePath_Relative +
                       ";MiddleRelative=" + setting.strPhotoMiddlePath_Relative +
                       ";SmallRelative=" + setting.strPhotoSmallPath_Relative;
                    strValue += ";SourecRootID=" + setting.iSourcePathID.ToString() +
                       ";MiddleRootID=" + setting.iMiddlePathID.ToString() +
                       ";SmallRootID=" + setting.iSmallPathID.ToString();
                }
            }
            UpdateKeyValueOnConfigFile(strValue, setting.strChannelsName);
        }

        private void DeleteKeyOnConfigFile(String strKeyName)
        {
            System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            config.AppSettings.Settings.Remove(strKeyName);
            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");
        }

        private void UpdateKeyValueOnConfigFile(String strValue, String strKeyName)
        {
            System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            config.AppSettings.Settings[strKeyName].Value = strValue;
            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");
        }

        public String GetKeyValue(String strKey)
        {
            String strKeyValue = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings[strKey]);
            if (strKeyValue == null)
            {
                strKeyValue = "";
                AddKeySettingToConfigFile("", strKey);
            }
            return strKeyValue;
        }

        public List<SettingParameter> GetSettingParameterList()
        {
            return lst_SettingParameter;
        }

        public void UpdateKeyValue(String strValue, String strKey)
        {
            UpdateKeyValueOnConfigFile(strValue, strKey);
        }

        public void UpdateSettingList(String strValue)
        {
            UpdateKeyValueOnConfigFile(strValue,"SettingList" );
        }

        public void LoadSetting()
        {
            List<String> lst_SettingName = new List<String>();
            try
            {
                strEmsVersion = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["EMS_Version"]);
                try
                {
                    int i = strEmsVersion.Length;
                }
                catch (Exception ex)
                {
                    strEmsVersion = "";
                }
                strProductName = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["Product_Name"]);
                try
                {
                    int i = strProductName.Length;
                }
                catch (Exception ex)
                {
                    strProductName = "";
                }
                String strSettingList = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["SettingList"]);
                try
                {
                    string[] words = strSettingList.Split(';');
                    foreach (string word in words)
                    {
                        if (word.Length == 0)
                            continue;
                        lst_SettingName.Add(word);
                    }
                }catch(Exception ex){
                    strSettingList = "";
                    AddKeySettingToConfigFile(strSettingList, "SettingList");
                }

                for (int i = 0; i < lst_SettingName.Count; i++)
                {
                    SettingParameter setting = new SettingParameter();
                    setting.strChannelsName = lst_SettingName[i];
                    setting.strChannelsName_Format = lst_SettingName[i];
                    String settingParam = "";
                    setting.bIsAP = false;
                    setting.strXmlType = "";//Pat 20160721 add bloomberg
                    setting.bIsGIS = false;
                    try
                    {
                        settingParam = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings[setting.strChannelsName]);
                        string[] words = settingParam.Split(';');
                        List<String> lst_ParameterSet = new List<String>();
                        foreach (string word in words)
                        {
                            lst_ParameterSet.Add(word);
                        }
                        for (int j = 0; j < lst_ParameterSet.Count; j++)
                        {
                            if (lst_ParameterSet[j].IndexOf("alisa=") > -1)
                            {
                                setting.strAliasCode = lst_ParameterSet[j].Replace("alisa=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("agencyTable=") > -1)
                            {
                                setting.strAgencyTableCode = lst_ParameterSet[j].Replace("agencyTable=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("agencyType=") > -1)
                            {
                                setting.strAgencyType = lst_ParameterSet[j].Replace("agencyType=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("isChannel=") > -1)
                            {
                                String str = lst_ParameterSet[j].ToString();
                                setting.bIsChannel = lst_ParameterSet[j].ToString().Replace("isChannel=", "").Equals("true", StringComparison.OrdinalIgnoreCase) ? true : false;
                            }
                            if (lst_ParameterSet[j].IndexOf("needCopyFile=") > -1)
                            {
                                setting.bNeedCopyFile = lst_ParameterSet[j].ToString().Replace("needCopyFile=", "").Equals("true", StringComparison.OrdinalIgnoreCase) ? true : false;
                            }
                            if (lst_ParameterSet[j].IndexOf("needDeleteFile=") > -1)
                            {
                                setting.bNeedDeleteFile = lst_ParameterSet[j].ToString().Replace("needDeleteFile=", "").Equals("true", StringComparison.OrdinalIgnoreCase) ? true : false;
                            }

                            if (lst_ParameterSet[j].IndexOf("IsProcessingTime=") > -1)
                            {
                                setting.bProcessTime = lst_ParameterSet[j].ToString().Replace("IsProcessingTime=", "").Equals("true", StringComparison.OrdinalIgnoreCase) ? true : false;
                            }
                            if (lst_ParameterSet[j].IndexOf("BufferTime=") > -1)
                            {
                                int iHour = 0;
                                int iMin = 0;
                                string strBufTime = lst_ParameterSet[j].ToString().Replace("BufferTime=", "").Trim();
                                if (strBufTime != "")
                                {
                                    iHour = Convert.ToInt32(strBufTime.Substring(0, strBufTime.IndexOf(":")));
                                    iMin = Convert.ToInt32(strBufTime.Substring(strBufTime.IndexOf(":")+1));
                                }
                                setting.dtBuffer = new DateTime(2000,1,1,iHour,iMin,0);
                            }

                            if (lst_ParameterSet[j].IndexOf("fileCopyFile=") > -1)
                            {
                                setting.strFileCopyPath = lst_ParameterSet[j].Replace("fileCopyFile=", "");
                                setting.strFileCopyPath_Format = lst_ParameterSet[j].Replace("fileCopyFile=", "");
                            }

                            if (lst_ParameterSet[j].IndexOf("photoSourcePath=") > -1)
                            {
                                setting.strPhotoSourcePath = lst_ParameterSet[j].Replace("photoSourcePath=", "");
                                setting.strPhotoSourcePath_Format = lst_ParameterSet[j].Replace("photoSourcePath=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("photoMiddlePath=") > -1)
                            {
                                setting.strPhotoMiddlePath = lst_ParameterSet[j].Replace("photoMiddlePath=", "");
                                setting.strPhotoMiddlePath_Format = lst_ParameterSet[j].Replace("photoMiddlePath=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("photoSmallPath=") > -1)
                            {
                                setting.strPhotoSmallPath = lst_ParameterSet[j].Replace("photoSmallPath=", "");
                                setting.strPhotoSmallPath_Format = lst_ParameterSet[j].Replace("photoSmallPath=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("XMLDownloadPath=") > -1)
                            {
                                setting.strXMLDownloadPath = lst_ParameterSet[j].Replace("XMLDownloadPath=", "");
                                setting.strXMLDownloadPath_Format = lst_ParameterSet[j].Replace("XMLDownloadPath=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("agencyName=") > -1)
                            {
                                setting.strAgencyName = lst_ParameterSet[j].Replace("agencyName=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("WiresID=") > -1)
                            {
                                setting.strWireID = lst_ParameterSet[j].Replace("WiresID=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("FileCode=") > -1)
                            {
                                setting.strFileCode = lst_ParameterSet[j].Replace("FileCode=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("FileSpec=") > -1)
                            {
                                String strFileSpec = lst_ParameterSet[j].Replace("FileSpec=", "");
                                List<String> lst_Spec = new List<String>();
                                List<FileSpec> lst_FileSpec = new List<FileSpec>();
                                try
                                {
                                    string[] fileSpec = strFileSpec.Split('|');
                                    foreach (string word in fileSpec)
                                    {
                                        if (word.Length == 0)
                                            continue;
                                        lst_Spec.Add(word);
                                    }
                                    foreach (string word in lst_Spec)
                                    {
                                        FileSpec temp_Spec = new FileSpec();
                                        fileSpec = word.Split(',');
                                        temp_Spec.strFileSpecName = fileSpec[0];
                                        temp_Spec.strAgencyCode = fileSpec[2];
                                        temp_Spec.strAgencyTable = fileSpec[1];
                                        if (fileSpec.Length > 3)
                                            temp_Spec.isUnique = fileSpec[3].Equals("U") ? true : false;
                                        else
                                            temp_Spec.isUnique = false;
                                        lst_FileSpec.Add(temp_Spec);
                                    }
                                    setting.lst_FileSpec = lst_FileSpec;
                                }
                                catch (Exception ex)
                                {
                                    strSettingList = "";
                                    AddKeySettingToConfigFile(strSettingList, "SettingList");
                                }
                            }
                            if (lst_ParameterSet[j].IndexOf("NameSubString=") > -1)
                               setting.str_SubstringName = lst_ParameterSet[j].Replace("NameSubString=", "");
                            if (lst_ParameterSet[j].IndexOf("Priority=") > -1)
                            {
                                try
                                {
                                    String strPriority = lst_ParameterSet[j].Replace("Priority=", "");
                                    List<String> lst_Pri = new List<String>();
                                    List<Priority> lst_Priority = new List<Priority>();
                                    string[] pri = strPriority.Split('|');
                                    foreach (string word in pri)
                                    {
                                        if (word.Length == 0)
                                            continue;
                                        lst_Pri.Add(word);
                                    }
                                    foreach (string word in lst_Pri)
                                    {
                                        Priority priority = new Priority();
                                        pri = word.Split(',');
                                        priority.iTagValue = int.Parse(pri[0]);
                                        priority.iPriorityValue = int.Parse(pri[1]);
                                        lst_Priority.Add(priority);
                                    }
                                    setting.lst_Priority = lst_Priority;
                                }catch(Exception ex)
                                {
                                    if (lst_ParameterSet[j].IndexOf("agencyName=") > -1)
                                    {
                                        setting.strAgencyName = lst_ParameterSet[j].Replace("agencyName=", "");
                                    }
                                }
                            }
                            if (lst_ParameterSet[j].IndexOf("IsAP=") > -1)
                            {
                                String str = lst_ParameterSet[j].ToString();
                                setting.bIsAP = lst_ParameterSet[j].ToString().Replace("IsAP=", "").Equals("true", StringComparison.OrdinalIgnoreCase) ? true : false;
                            }
                            //Pat 20160721 add bloomberg-----------------------------------------------
                            if (lst_ParameterSet[j].IndexOf("XmlType=") > -1)
                            {
                                String str = lst_ParameterSet[j].ToString();
                                setting.strXmlType = lst_ParameterSet[j].ToString().Replace("XmlType=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("BBbIgnoreFile=") > -1)
                            {
                                String str = lst_ParameterSet[j].ToString();
                                setting.BBbIgnoreFile = lst_ParameterSet[j].ToString().Replace("BBbIgnoreFile=", "").Equals("True", StringComparison.OrdinalIgnoreCase) ? true : false; 
                            }
                            if (lst_ParameterSet[j].IndexOf("BBIgnoreFileAction=") > -1)
                            {
                                int temp = -1;
                                try
                                {
                                    temp = int.Parse(lst_ParameterSet[j].ToString().Replace("BBIgnoreFileAction=", ""));
                                }
                                catch (Exception ex)
                                {
                                    temp = 1;
                                }
                                setting.BBIgnoreFileAction = temp;
                            }
                            if (lst_ParameterSet[j].IndexOf("BBstrMovePath=") > -1)
                            {
                                setting.BBstrMovePath = lst_ParameterSet[j].ToString().Replace("BBstrMovePath=", "");
                            }
                            if (lst_ParameterSet[j].IndexOf("BBstrIgnoreStr=") > -1)
                            {
                                setting.BBstrIgnoreStr = lst_ParameterSet[j].ToString().Replace("BBstrIgnoreStr=", "");
                            }
                            //-------------------------------------------------------------------------
                            if (strEmsVersion == "EMS6.5")
                            {
                                if (lst_ParameterSet[j].IndexOf("SourecRoot=") > -1)
                                    setting.strPhotoSourcePath_Root = lst_ParameterSet[j].Replace("SourecRoot=", "");
                                if (lst_ParameterSet[j].IndexOf("MiddleRoot=") > -1)
                                    setting.strPhotoMiddlePath_Root = lst_ParameterSet[j].Replace("MiddleRoot=", "");
                                if (lst_ParameterSet[j].IndexOf("SmallRoot=") > -1)
                                    setting.strPhotoSmallPath_Root = lst_ParameterSet[j].Replace("SmallRoot=", "");
                                if (lst_ParameterSet[j].IndexOf("SourecRelative=") > -1)
                                    setting.strPhotoSourcePath_Relative = lst_ParameterSet[j].Replace("SourecRelative=", "");
                                if (lst_ParameterSet[j].IndexOf("MiddleRelative=") > -1)
                                    setting.strPhotoMiddlePath_Relative = lst_ParameterSet[j].Replace("MiddleRelative=", "");
                                if (lst_ParameterSet[j].IndexOf("SmallRelative=") > -1)
                                    setting.strPhotoSmallPath_Relative = lst_ParameterSet[j].Replace("SmallRelative=", "");
                                try
                                {
                                    if (lst_ParameterSet[j].IndexOf("SourecRootID=") > -1)
                                        setting.iSourcePathID = Int32.Parse(lst_ParameterSet[j].Replace("SourecRootID=", ""));
                                }
                                catch (Exception ex)
                                {setting.iSourcePathID = -1;}
                                try
                                {
                                    if (lst_ParameterSet[j].IndexOf("MiddleRootID=") > -1)
                                        setting.iMiddlePathID = Int32.Parse(lst_ParameterSet[j].Replace("MiddleRootID=", ""));
                                }
                                catch (Exception ex)
                                { setting.iMiddlePathID = -1; }
                                try
                                {
                                    if (lst_ParameterSet[j].IndexOf("SmallRootID=") > -1)
                                        setting.iSmallPathID = Int32.Parse(lst_ParameterSet[j].Replace("SmallRootID=", ""));
                                }
                                catch (Exception ex)
                                { setting.iSmallPathID = -1; }
                            }
                            if (lst_ParameterSet[j].IndexOf("IsGIS=") > -1)
                            {
                                String str = lst_ParameterSet[j].ToString();
                                setting.bIsGIS = lst_ParameterSet[j].ToString().Replace("IsGIS=", "").Equals("true", StringComparison.OrdinalIgnoreCase) ? true : false;
                            }
                        }
                        lst_SettingParameter.Add(setting);
                    }catch(Exception ex)
                    {
                        continue;
                    }
                }

                String strSourcePhotoSizeTypeList = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["SourcePhotoSizeTypeList"]);
                try
                {
                    string[] words = strSourcePhotoSizeTypeList.Split(';');
                    foreach (string word in words)
                    {
                        if (word.Length == 0)
                            continue;
                        lst_SourcePhotoSizeType.Add(word);
                    }
                }
                catch (Exception ex)
                {
                    strSourcePhotoSizeTypeList = "baseImage;limitedImage;viewImage;thumbnail";
                    string[] words = strSourcePhotoSizeTypeList.Split(';');
                    foreach (string word in words)
                    {
                        if (word.Length == 0)
                            continue;
                        lst_SourcePhotoSizeType.Add(word);
                    }
                    AddKeySettingToConfigFile(strSourcePhotoSizeTypeList, "SourcePhotoSizeTypeList");
                }
            }
            catch (Exception ex)
            {

            }

        }

        public List<String> GetSourcePhotoSizeType()
        {
            return lst_SourcePhotoSizeType;
        }
    }
}

public class SettingParameter
{
    public String strChannelsName; // Can be a path. If is a path, "strAliasCode" will be empty.
    public String strChannelsName_Format;
    public String strAliasCode;
    public String strAgencyTableCode;
    public String strAgencyType;
    public String strAgencyName;
    public bool bIsChannel;
    public bool bNeedCopyFile;
    public bool bNeedDeleteFile;
    public bool bProcessTime;
    public DateTime dtBuffer;
    public String strFileCopyPath;
    public String strXMLDownloadPath;
    public String strPhotoSourcePath;
    public String strPhotoMiddlePath;
    public String strPhotoSmallPath;
    public String strFileCopyPath_Format;
    public String strXMLDownloadPath_Format;
    public String strPhotoMiddlePath_Format;
    public String strPhotoSourcePath_Format;
    public String strPhotoSmallPath_Format;
    public String strWireID;// 20130910 For Metro use
    public String strFileCode;// 20130910 For Metro use
    public bool bIsAP;// 20160126 For Metro use (declare the setting to process AP XML)
    public String strXmlType;//Pat 20160720 more xml type
    public List<FileSpec> lst_FileSpec;
    public String str_SubstringName;
    public List<Priority> lst_Priority;
    public int iSourcePathID;//for EMS6.5 use
    public int iMiddlePathID;//for EMS6.5 use
    public int iSmallPathID;//for EMS6.5 use
    public String strPhotoSmallPath_Relative;//for EMS6.5 use
    public String strPhotoMiddlePath_Relative;//for EMS6.5 use
    public String strPhotoSourcePath_Relative;//for EMS6.5 use
    public String strPhotoSmallPath_Root;//for EMS6.5 use
    public String strPhotoMiddlePath_Root;//for EMS6.5 use
    public String strPhotoSourcePath_Root;//for EMS6.5 use
    //Pat 20160812 for bloomberg----------------
    public bool BBbIgnoreFile;
    public int BBIgnoreFileAction;
    public String BBstrMovePath;
    public String BBstrIgnoreStr;
    //------------------------------------------
    public bool bIsGIS;// 20180530 For Metro use (declare the setting to process GIS XML)
}