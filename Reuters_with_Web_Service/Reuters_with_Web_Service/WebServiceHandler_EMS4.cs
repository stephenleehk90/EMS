using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Reuters_with_Web_Service.com.reuters.rmb;
using System.Net;
using System.Xml.Linq;
using System.Data;
using System.Threading;
/*
 * This class for handle data insert on EMS4.0 Version
*/
namespace Reuters_with_Web_Service
{
    public class WebServiceHandler_EMS4 : WebServiceHandler
    {
        RmdService service;
        String strAuth;
        String strUserName, strPassword;
        String strError;
        String strMaxAge;
        List<NewsChannel> lstNewsChannels;
        List<NewsItem> lstNewsItem;
        DBHandler DB;
        List<SettingParameter> lst_Setting;
        List<String> lst_ListOfDataInDB;
        frm_Main main;
        int iInterval;
        List<FaultXMLList> lst_FaultNews;

        public WebServiceHandler_EMS4(String strUserName, String strPassword)
        {
            this.strUserName = strUserName;
            this.strPassword = strPassword;
            strMaxAge = "1h";
            lst_FaultNews = new List<FaultXMLList>();
        }

        public override void SetDB(DBHandler DB)
        {
            this.DB = DB;
        }

        public override bool Connect()
        {
            try
            {
                DataTable dt = new DataTable();
                dt = DB.GetRecordToDataTable("select value from master..sys_newsroom where entry = N'Reuters_UserName'");
                this.strUserName = dt.Rows[0].ItemArray[0].ToString().Trim();
                dt = DB.GetRecordToDataTable("select value from master..sys_newsroom where entry =N'Reuters_Password'");
                this.strPassword = dt.Rows[0].ItemArray[0].ToString().Trim();
                strAuth = GetReutersLoginAuth();
                if (strAuth.Length == 0)
                    return false;
                service = new RmdService();
                service.Url = "http://rmb.reuters.com/rmd/soap/xml/?token=" + strAuth;
            }catch(Exception ex){
                return false;
            }
            return true;
        }

        public override void SetFrmMain(frm_Main main)
        {
            this.main = main;
        }

        private bool CheckServiceLoginTimeout()
        {// if return false, that mean the login section was timeout.
            String response;
            try
            {
                using (WebClient wc = new WebClient())
                {
                    response = wc.DownloadString(service.Url);
                }
                if (response.Length > 0)
                {
                    return true;
                }
            }
            catch (Exception ex)
            { 
                return false;
            }
            return true;
        }
        
        private String GetReutersLoginAuth()
        {
            String response;
            //"https://commerce.reuters.com/rmd/rest/xml/login?username=takungpao_rccd&password=Rtrt4Kun6PA0";
            try
            {
                using (WebClient wc = new WebClient())
                {
                    response = wc.DownloadString("https://commerce.reuters.com/rmd/rest/xml/login?username=" + strUserName + "&password=" + strPassword);
                }
                if (response.Length > 0)
                {
                    XDocument doc = XDocument.Parse(response);
                    response = doc.Element("authToken").Value;
                }
            }
            catch (Exception ex)
            {
                strError = "Connect to reuters with error:"+ex.Message.Trim()+"\n";
                strError += "Hints: Please check the Reuters username and password are correct in AdminPro";
                response = ""; 
            }

            return response;
        }

        public override RmdService GetService()
        {
            return service;
        }

        public override List<NewsChannel> GetAllChannels()
        {
            lstNewsChannels = new List<NewsChannel>();
            return lstNewsChannels;
            availableChannels channels = new availableChannels();
            channels = service.getChannels(new String[] { "" }, new String[] { "TXT", "PIC", "NGS", "" });
            List<channelInformation> channelInfo = channels.channelInformation.ToList();
            for (int i = 0; i < channelInfo.Count; i++)
            {
                NewsChannel channel = new NewsChannel();
                channel.strAlias = channelInfo[i].alias;
                channel.strDescrpition = channelInfo[i].description;
                channel.strField = channelInfo[i].availableContentProfiles[0].ToString();
                channel.dtLastUpdateTime = channelInfo[i].lastUpdate.ToLocalTime();
                channel.strCateId = channelInfo[i].category.id;
                lstNewsChannels.Add(channel);
            }
            return lstNewsChannels;
        }

        public List<NewsItem> GetChannelNewsList(NewsChannel channel)
        {
            results res = new results();
            lstNewsItem = new List<NewsItem>();
            try
            {
                res = service.getItems(null, strMaxAge, new String[] { channel.strAlias }, null, 300, true, new String[] { channel.strCateId }, "default", 100, true, false, true, null);
                for (int i = 0; i < res.result.Count(); i++)
                {
                    NewsItem item = new NewsItem();
                    item.id = res.result[i].id;
                    item.dtNewsTime = res.result[i].dateCreated.ToLocalTime();
                    item.iUrgentPriority = res.result[i].priority;
                    lstNewsItem.Add(item);
                }
            }
            catch (Exception ex)
            {
                SetTextToMain(String.Format("Getting news items error with the following message:\n{0}", ex.Message.Trim()));
            }
            return lstNewsItem;
        }

        private void SetTextToMain(String strMessage)
        {
            try
            {
                main.Invoke(main.m_DelegateAddString, new Object[] { strMessage });
            }
            catch (Exception ex) { }
        }

        public override void SetMaxAge(String strMaxAge)
        {
            this.strMaxAge = strMaxAge;
        }

        public override String GetError()
        {
            return strError;
        }

        public List<String> GetDBDataIntoList(String strNewsType)
        {
            //process picture later
            DataTable table = new DataTable();
            String strSQL= "";
            if (strNewsType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                //strSQL = String.Format("select replace(tele_name,'__',':') from {0}", strAgencyCode);
                strSQL = String.Format("select reuter_news_id from wires_reuters_record where isPhoto = 0");
            else
                strSQL = String.Format("select reuter_news_id from wires_reuters_record where isPhoto = 1");
            table = DB.GetRecordToDataTable(strSQL);
            lst_ListOfDataInDB = new List<String>();
            try
            {
                for (int i = 0; i < table.Rows.Count; i++)
                    lst_ListOfDataInDB.Add(table.Rows[i].ItemArray[0].ToString().Trim());
            }catch(Exception ex)
            {
            }
            return lst_ListOfDataInDB;
        }

        public override void Start(SettingParameter setting)
        {
            UpdateSettingPath(setting);
            List<NewsItem> lstNewList = new List<NewsItem>();
            int iTotalNewsCount = 0;
            if(setting.strAgencyTableCode.Length==0)
            {
                SetTextToMain("Agency Table must be choose. All record refer to this setting will be skipped.");
                return;
            }
            if (setting.bIsChannel)
            {
                int count = 0;
                while (!CheckServiceLoginTimeout())
                {
                    Connect();
                    count++;
                    if (count > 10)
                    {
                        strError = "Connect to Reuters failed. Program will be retry later.";
                        return;
                    }
                }
                for (int j = 0; j < lstNewsChannels.Count; j++)
                {
                    if (lstNewsChannels[j].strAlias.Equals(setting.strAliasCode))
                    {
                        lstNewList = GetChannelNewsList(lstNewsChannels[j]);
                        break;
                    }
                }
            }
            else
            {
                try
                {
                    DirectoryInfo dir = new DirectoryInfo(setting.strChannelsName);
                    List<FileInfo> lst_Files = dir.GetFiles().ToList().FindAll(delegate(FileInfo file)
                    {
                        return file.Extension.Equals(".xml", StringComparison.OrdinalIgnoreCase);
                    }
                    );
                    lstNewList = lst_Files.ConvertAll<NewsItem>(delegate(FileInfo file)
                        {
                            NewsItem item = new NewsItem();
                            item.id = file.FullName;
                            return item;
                        });
                }
                catch (Exception ex)
                {
                    SetTextToMain(ex.Message.Trim() + ":" + setting.strChannelsName);
                }
            }
            List<String> lst_DBRecord = GetDBDataIntoList(setting.strAgencyType);

            for (int j = 0; j < lstNewList.Count; j++)
            {
                if (setting.bIsChannel)
                {
                    String strFind = lst_DBRecord.Find(
                        delegate(String str)
                        {
                            return str == lstNewList[j].id;
                        }
                        );
                    if (strFind != null)
                        continue;
                }
                String strXMLContent = "";
                XML_Reader reader = new XML_Reader("", false);
                if (setting.bIsChannel)
                {
                    try
                    {
                        strXMLContent = service.getItem(lstNewList[j].id.ToString(), setting.strAliasCode, null, null);
                    }
                    catch (Exception ex)
                    {
                        main.SetTextOnRichTextBox(String.Format("Process on {0} catch the following error:\n {1} ", lstNewList[j].id.ToString(), ex.Message.Trim()));
                        continue;
                    }
                    reader = new XML_Reader(strXMLContent, setting.bIsChannel);
                    reader.SetNewsID(lstNewList[j].id.ToString());
                }
                else
                    reader = new XML_Reader(lstNewList[j].id, setting.bIsChannel);
                reader.Read();
                if (!setting.bIsChannel)
                {
                    String strFind = lst_DBRecord.Find(
                        delegate(String str)
                        {
                            return str == reader.GetNewsID().Replace("__", ":");
                        }
                        );
                    if (strFind != null)
                    {
                        SetTextToMain(".");
                        continue;
                    }
                }
                String strValue = "", strFileSpecTableCode = "";
                bool bInsertToDefault = true;
                String strContent = reader.GetStrContent();
                strContent = strContent.Replace("''", "'");
                //for (int i = 0; i < strContent.Length; i++)
                //{
                //    if (strContent[i] == '\r')
                //    {
                //        strContent = strContent.Insert(i, "   ");
                //        i = i + 4;
                //    }
                //    if (strContent[i] == '\n')
                //    {
                //        strContent = strContent.Insert(i, "   \r");
                //        i = i + 4;
                //    }
                //}
                List<FileSpec> lst_Insert = new List<FileSpec>();
                GetInsertTableList(reader, setting.lst_FileSpec, ref lst_Insert);
                if (setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    int iFirst = 0, iLast = 0;
                    String[] words = setting.str_SubstringName.Split(',');
                    iFirst = int.Parse(words[0]);
                    iLast = int.Parse(words[1]);
                    String str_TeleId = "";
                    try
                    {
                        str_TeleId = reader.GetNewsID().Replace(":", "__").Substring(iFirst, reader.GetNewsID().Replace(":", "__").Length - iLast - iFirst);
                    }
                    catch (Exception ex)
                    {
                        SetTextToMain(ex.Message.Trim() + ":" + reader.GetNewsID().Replace(":", "__"));
                        str_TeleId = reader.GetNewsID().Replace(":", "__");
                    }
                    bool nRet = true;
                    for (int i = 0; i < lst_Insert.Count; i++)
                    {
                        {
                            ADOOjectWires wires = new ADOOjectWires();
                            wires.str_TeleName = str_TeleId;
                            wires.dt_TeleTime = reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss");
                            wires.iwords = reader.GetWordCounts();
                            wires.str_Title = reader.GetStrHeadLine().Length > 250 ? reader.GetStrHeadLine().Substring(0, 250) : reader.GetStrHeadLine(); wires.str_Title = wires.str_Title.Replace("''", "'");
                            wires.str_Urgent = reader.GetUrgent();
                            wires.str_Note = ""; wires.str_Location = "";
                            wires.str_Language = "E";
                            wires.str_Flag = "N";
                            wires.str_Keyword = "";
                            wires.str_Content = wires.str_Title+"   \r\n"+strContent;
                            InsertRecordToDB(strValue, lst_Insert[i].strAgencyCode, setting.strAgencyType, null, wires);
                            bInsertToDefault = false;
                            String strMessage = String.Format("{0} was insert into {1}", str_TeleId, lst_Insert[i].strAgencyCode);
                            SetTextToMain(strMessage);
                        }
                    }
                    if (bInsertToDefault)
                    {
                        ADOOjectWires wires = new ADOOjectWires();
                        wires.str_TeleName = str_TeleId;
                        wires.dt_TeleTime = reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss");
                        wires.iwords = reader.GetWordCounts();
                        wires.str_Title = reader.GetStrHeadLine().Length > 250 ? reader.GetStrHeadLine().Substring(0, 250) : reader.GetStrHeadLine(); wires.str_Title = wires.str_Title.Replace("''", "'");
                        wires.str_Urgent = reader.GetUrgent();
                        wires.str_Note = ""; wires.str_Location = "";
                        wires.str_Language = "E";
                        wires.str_Flag = "N";
                        wires.str_Keyword = "";
                        wires.str_Content = wires.str_Title + "   \r\n" + strContent;
                        nRet =InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType, null, wires);
                        String strMessage = String.Format("{0} was insert into {1}", reader.GetNewsID(), setting.strAgencyTableCode);
                        SetTextToMain(strMessage);
                    }
                    if (nRet)
                    {
                        String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", str_TeleId, reader.GetNewsID(), 0);
                        DB.ExecuteSQL(strSql);
                        iTotalNewsCount++;
                        if (setting.bIsChannel && setting.bNeedCopyFile)
                            nRet = DownloadXMLFile(setting.strXMLDownloadPath.Trim().LastIndexOf("\\") == setting.strXMLDownloadPath.Trim().Length ?
                                setting.strXMLDownloadPath + reader.GetXMLFileName() : setting.strXMLDownloadPath + "\\" + reader.GetXMLFileName(), strXMLContent);
                        if (!setting.bIsChannel && setting.strFileCopyPath.Length > 0)
                            CopyXML(setting, reader.GetXMLFileName(), reader.GetNewsDateTime().ToLocalTime());
                        if (!setting.bIsChannel && setting.bNeedDeleteFile)
                            nRet = DeleteFiles(setting, reader.GetXMLFileName(), null);
                    }
                    else
                        SetTextToMain(String.Format("{0} cannot insert to {1}", reader.GetNewsID(), setting.strAgencyTableCode));
                }
                else
                {
                    PhotoInfo info = reader.GetPhotoInfo();
                    byte[] photo = null;
                    String strDescSourcePath = "";
                    if(Path.GetExtension(info.strSourcePhotoName).ToLower().Equals(".eps", StringComparison.OrdinalIgnoreCase))
                        continue;
                    if (!DownloadPhoto(ref info, setting, ref photo, ref strDescSourcePath, reader.GetNewsDateTime().ToLocalTime()))
                        continue;

                    bool nRet = true;
                    for (int i = 0; i < lst_Insert.Count; i++)
                    {
                        {
                            String strPhotoID = DB.GetNextPhotoID().Trim();
                            if (strPhotoID.Length == 0)
                            {
                                SetTextToMain(String.Format("Cannot get the next photo id"));
                                return;
                            }
                            ADOPhotoObject photoADO = new ADOPhotoObject();
                            photoADO.iSize = int.Parse(info.strSize);
                            photoADO.str_BeginTime = "00:00";
                            photoADO.str_Caption = reader.GetStrCaption();
                            photoADO.str_Format = "jpg";
                            photoADO.str_Location = "";
                            photoADO.str_PhotoFlag = "N";
                            photoADO.str_PhotoName = strPhotoID + ".jpg";
                            photoADO.str_PhotoTime = reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss");
                            photoADO.str_TeleCode = lst_Insert[i].strAgencyCode;
                            photoADO.str_Title = reader.GetStrHeadLine().Length > 250 ? reader.GetStrHeadLine().Substring(0, 250) : reader.GetStrHeadLine();
                            photoADO.photo_Bitmap = photo;
                            nRet = InsertRecordToDB(strValue, "", setting.strAgencyType, photoADO, null);
                            String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetNewsID().Replace("__", ":"), 1);
                            DB.ExecuteSQL(strSql);
                            strSql = String.Format("insert into photo_log values ('{0}','{1}')", strPhotoID + ".jpg", strDescSourcePath);
                            DB.ExecuteSQL(strSql);
                            bInsertToDefault = false;
                        }
                    }
                    if (bInsertToDefault)
                    {
                        String strPhotoID = DB.GetNextPhotoID().Trim();
                        if (strPhotoID.Length == 0)
                        {
                            SetTextToMain(String.Format("Cannot get the next photo id"));
                            return;
                        }
                        ADOPhotoObject photoADO = new ADOPhotoObject();
                        photoADO.iSize = int.Parse(info.strSize);
                        photoADO.str_BeginTime = "00:00";
                        photoADO.str_Caption = reader.GetStrCaption();
                        photoADO.str_Format = "jpg";
                        photoADO.str_Location = "";
                        photoADO.str_PhotoFlag = "N";
                        photoADO.str_PhotoName = strPhotoID + ".jpg";
                        photoADO.str_PhotoTime = reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss");
                        photoADO.str_TeleCode = setting.strAgencyTableCode;
                        photoADO.photo_Bitmap = photo;
                        photoADO.str_Title = reader.GetStrHeadLine().Length > 250 ? reader.GetStrHeadLine().Substring(0, 250) : reader.GetStrHeadLine();
                        nRet = InsertRecordToDB(strValue, "", setting.strAgencyType, photoADO,null);
                        String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetNewsID().Replace("__", ":"), 1);
                        DB.ExecuteSQL(strSql);
                        strSql = String.Format("insert into photo_log values ('{0}','{1}')", strPhotoID + ".jpg", strDescSourcePath);
                        DB.ExecuteSQL(strSql);
                    }
                    if (nRet)
                    {
                        String strMessage = String.Format("Photo {0} was prcessed and inserted into Database", info.strSourcePhotoName);
                        SetTextToMain(strMessage);
                        iTotalNewsCount++;
                        try
                        {
                            String strPath = setting.strPhotoSourcePath.Trim().LastIndexOf("\\") == setting.strPhotoSourcePath.Trim().Length ?
                                setting.strPhotoSourcePath.Trim(): setting.strPhotoSourcePath.Trim() + "\\";
                            String strTxtFileName = reader.GetXMLFileName().Substring(0, reader.GetXMLFileName().LastIndexOf(".")) + ".txt";// Picture Caption
                            File.WriteAllText(strPath+strTxtFileName, reader.GetStrCaption().Trim());
                        }
                        catch (Exception ex)
                        {
                         
                            SetTextToMain(String.Format("Failed to create picture caption on {0}", info.strSourcePhotoName, setting.strAgencyTableCode));
                        }
                        if (setting.bIsChannel && setting.bNeedCopyFile)
                        {
                            nRet = DownloadXMLFile(setting.strXMLDownloadPath.Trim().LastIndexOf("\\") == setting.strXMLDownloadPath.Trim().Length ?
                                setting.strXMLDownloadPath + reader.GetXMLFileName() : setting.strXMLDownloadPath + "\\" + reader.GetXMLFileName(), strXMLContent);
                        }
                        if (!setting.bIsChannel && setting.strFileCopyPath.Length > 0)
                            CopyXML(setting, reader.GetXMLFileName(), reader.GetNewsDateTime().ToLocalTime());
                        if (!setting.bIsChannel && setting.bNeedDeleteFile)
                            nRet = DeleteFiles(setting, reader.GetXMLFileName(), info);
                    }
                    else
                    {
                        String strMessage = String.Format("Photo {0} cannot insert into Database", info.strSourcePhotoName);
                        SetTextToMain(strMessage);
                    }
                }
            }
            SetTextToMain(String.Format("Total news processed on {0} : {1}", setting.strChannelsName, iTotalNewsCount.ToString()));
            strError = "";
        }

        private bool CopyXML(SettingParameter setting, String strXMLFileName, DateTime dt_NewsTime)//From a local path to another local path
        {
            String strSourcePath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                        setting.strChannelsName + strXMLFileName : setting.strChannelsName + "\\" + strXMLFileName;
            String strCopyPath = "";
            try
            {

                strCopyPath = setting.strFileCopyPath_Format.Replace("%Y", dt_NewsTime.Year.ToString());
                strCopyPath = strCopyPath.Replace("%M", dt_NewsTime.ToString("MM"));
                strCopyPath = strCopyPath.Replace("%D", dt_NewsTime.ToString("dd"));
                if (!Directory.Exists(strCopyPath))
                    Directory.CreateDirectory(strCopyPath);
                String strDestPath = strCopyPath.Trim().LastIndexOf("\\") == strCopyPath.Length ?
                            strCopyPath + strXMLFileName : strCopyPath + "\\" + strXMLFileName;
                File.Copy(strSourcePath, strDestPath, true);
            }
            catch (Exception ex)
            {
                SetTextToMain(String.Format("Transfer {0} to {1} Failed. {2} : [{3}][{4}][{5}]",
                    strSourcePath, strCopyPath, ex.Message.Trim(), setting.strChannelsName.Trim(), setting.strFileCopyPath.Trim(), strXMLFileName));
                return false;
            }
            return true;
        }

        private bool DeleteFiles(SettingParameter setting, String strXMLFileName,PhotoInfo info)
        {
            try
            {
                if (setting.bNeedDeleteFile && setting.strAgencyType.Equals("text", StringComparison.OrdinalIgnoreCase))
                {
                    if (File.Exists(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                        setting.strChannelsName + strXMLFileName : setting.strChannelsName + "\\" + strXMLFileName))
                        File.Delete(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                            setting.strChannelsName + strXMLFileName : setting.strChannelsName + "\\" + strXMLFileName);

                }
                else
                {
                    if (File.Exists(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                        setting.strChannelsName + strXMLFileName : setting.strChannelsName + "\\" + strXMLFileName))
                        File.Delete(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                            setting.strChannelsName + strXMLFileName : setting.strChannelsName + "\\" + strXMLFileName);
                    if (File.Exists(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                        setting.strChannelsName + info.strSourcePhotoName : setting.strChannelsName + "\\" + info.strSourcePhotoName))
                        File.Delete(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                            setting.strChannelsName + info.strSourcePhotoName : setting.strChannelsName + "\\" + info.strSourcePhotoName);
                    if (File.Exists(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                        setting.strChannelsName + info.strMiddlePhotoName : setting.strChannelsName + "\\" + info.strMiddlePhotoName))
                        File.Delete(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                            setting.strChannelsName + info.strMiddlePhotoName : setting.strChannelsName + "\\" + info.strMiddlePhotoName);
                    if (File.Exists(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                        setting.strChannelsName + info.strSmallPhotoName : setting.strChannelsName + "\\" + info.strSmallPhotoName))
                        File.Delete(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                            setting.strChannelsName + info.strSmallPhotoName : setting.strChannelsName + "\\" + info.strSmallPhotoName);
                }
            }
            catch (Exception ex) 
            {
                return false;
            }
            return true;
        }

        private bool DownloadXMLFile(String strXMLFileName, String strXMLContent)
        {
            try
            {
                File.WriteAllText(strXMLFileName, strXMLContent);
            }
            catch (Exception ex)
            {
                return false;
            }
            return true;
        }

        public override void SetInterval(int iInterval)
        {
            this.iInterval = iInterval;
        }

        public override void Start(List<SettingParameter> lst_Setting)
        {
            while (true)
            {
                SetTextToMain("Start...");
                for (int i = 0; i < lst_Setting.Count; i++)
                {
                    String strMessage = String.Format("Now start process {0}", lst_Setting[i].strChannelsName);
                    SetTextToMain(strMessage);
                    Start(lst_Setting[i]);
                }
                strError = "";
                SetTextToMain("Finish.");
                Thread.Sleep(iInterval);
            }
        }

        private bool DownloadPhoto(ref PhotoInfo info, SettingParameter setting, ref byte[] photo, ref String strDestSource, DateTime dt_NewsDate)
        {
            bool nRet = true;
            try
            {
                String strDestPath_Source = setting.strPhotoSourcePath_Format.Replace("%Y", dt_NewsDate.Year.ToString());
                strDestPath_Source = strDestPath_Source.Replace("%M", dt_NewsDate.ToString("MM"));
                strDestPath_Source = strDestPath_Source.Replace("%D", dt_NewsDate.ToString("dd"));
                if (!Directory.Exists(strDestPath_Source))
                    Directory.CreateDirectory(strDestPath_Source);
                String strDestSourcePath = strDestPath_Source.Trim().LastIndexOf("\\") == strDestPath_Source.Trim().Length ?
                    strDestPath_Source.Trim() + info.strSourcePhotoName : strDestPath_Source.Trim() + "\\" + info.strSourcePhotoName;
               
                List<String> lst_DestPhotoName = new List<String>();
                lst_DestPhotoName.Add(strDestSourcePath);
                List<String> lst_PhotoLink = new List<String>();
                lst_PhotoLink.Add(info.strPhotoSourcePath);
                if (setting.bIsChannel)
                {
                    info.strPhotoSourcePath = strDestSourcePath;
                    for (int i = 0; i < lst_PhotoLink.Count; i++)
                    {
                       using (WebClient wc = new WebClient())
                       {
                           try
                           {
                               wc.DownloadFile(lst_PhotoLink[i] + "?token=" + strAuth, lst_DestPhotoName[i]);
                           }
                           catch (Exception ex) {
                               SetTextToMain(String.Format("Failed to download picture with url {0}", lst_PhotoLink[i]));
                           }
                       }
                    }
                }
                else
                {
                    String strSourceBasePhotoPath = "";
                    strSourceBasePhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                        setting.strChannelsName.Trim() + info.strSourcePhotoName : setting.strChannelsName.Trim() + "\\" + info.strSourcePhotoName;
                    info.strPhotoSourcePath = strDestSourcePath;
                    try
                    {
                        FileInfo fileInfo = new FileInfo(strSourceBasePhotoPath);
                        if (fileInfo.Exists)
                        {

                            LEADLib.LEAD lead = new LEADLib.LEAD();
                            lead.Load(strSourceBasePhotoPath, 0, 0, 1);
                            float bitH = lead.BitmapHeight;
                            float bitW = lead.BitmapWidth;
                            float height = 150;
                            float width = 150;
                            if (bitH / height > bitW / width)
                            {
                                height = bitH / (bitH / height);
                                width = bitW / (bitH / height);
                            }
                            else
                            {
                                height = bitH / (bitW / width);
                                width = bitW / (bitW / width);
                            }
                            lead.Size(width, height, 2);
                            String str_TempPath = strDestSourcePath.Substring(0, strDestSourcePath.LastIndexOf('.')) + "_temp.jpg";
                            lead.Save(str_TempPath, 10, 24, 10, 0);
                            FileStream stream = new FileStream(str_TempPath, FileMode.Open, FileAccess.Read);
                            BinaryReader reader = new BinaryReader(stream);

                            byte[] photo_temp = reader.ReadBytes((int)stream.Length);

                            reader.Close();
                            stream.Close();
                            photo = photo_temp;
                            try
                            {
                                File.Delete(str_TempPath);
                            }
                            catch (Exception ex) { }

                            fileInfo.CopyTo(strDestSourcePath, true);
                            strDestSource = strDestSourcePath;


                        }
                        else { return false; }
                        // if Base Picture cannot found, then nothing can be done. ---> return false
                        
                    }
                    catch (Exception ex) 
                    {
                        SetTextToMain("Process picture failed: "+ex.Message.Trim());
                    }
                }
            }
            catch (Exception ex) 
            {
                SetTextToMain(String.Format("Error ocurrs in download picture process:"+ex.Message.Trim()));
                return false; 
            }

            return nRet;
        }
      
        private String GetFileSpecTableMap(XML_Reader reader, FileSpec spec)
        {
            String strTableCode = "";
            List<String> lst_Subject = reader.GetSubjectList();
            for (int i = 0; i < lst_Subject.Count; i++)
            {
                if (lst_Subject[i].Equals(spec.strFileSpecName))
                {
                    strTableCode = spec.strAgencyCode;
                    break;
                }
            }
            return strTableCode;
        }

        private void GetInsertTableList(XML_Reader reader, List<FileSpec> lst_FileSpec, ref List<FileSpec> lst_Result)
        {
            List<FileSpec> lst_Temp = new List<FileSpec>();
            List<String> lst_Subject = reader.GetSubjectList();
            // search the value on File Spec list where the settled File Spec are same as subject list value
            var matches = from spec in lst_FileSpec
                          join subject in lst_Subject on spec.strFileSpecName equals subject
                          select spec;
            lst_Temp = matches.ToList();
            FileSpec temp_spec = lst_Temp.Find(delegate(FileSpec spec) { return spec.isUnique == true; });
            if (temp_spec != null)//if the map list contains "Unique" pick on the previous step, that take "Non-Unique" out of the insert list.
            {
                for (int i = 0; i < lst_Temp.Count; i++)
                {
                    if (lst_Temp[i].isUnique == false)
                    {
                        lst_Temp.RemoveAt(i);
                        i = -1;
                    }
                }
                // if "Unique" set, only one table will be insert. If more than one "Unique" the first one on the list will be use, other File Spec will be ignore
                if (lst_Temp.Count > 1)
                    lst_Temp.RemoveRange(1, lst_Temp.Count - 1);
            }
            lst_Result = lst_Temp;
        }

        private bool InsertRecordToDB(String strValue, String strTableName, String strTableType, ADOPhotoObject photo, ADOOjectWires wires)
        {
            bool nRet = true;
            String strSQL = "";
            if (strTableType.Equals("Text", StringComparison.OrdinalIgnoreCase))
            {
                //strSQL = String.Format("Insert into {0} ( tele_name , tele_time , tele_words , tele_content,tele_title, tele_location , tele_keyword,tele_urgent,tele_flag,tele_note , tele_language) values {1}",
                //    strTableName, strValue);
                //nRet = DB.ExecuteSQL(strSQL);
                nRet = DB.PrepareOnInsertWires(wires, strTableName);
            }
            else
            {
                nRet = DB.SpecicalHandleEMS4InsertPhoto(photo);
            }
            
            return nRet;
        }

        private void UpdateSettingPath(SettingParameter setting)
        {

            setting.strXMLDownloadPath = setting.strXMLDownloadPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strXMLDownloadPath = setting.strXMLDownloadPath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strXMLDownloadPath = setting.strXMLDownloadPath.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strFileCopyPath = setting.strFileCopyPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strFileCopyPath = setting.strFileCopyPath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strFileCopyPath = setting.strFileCopyPath.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strPhotoSourcePath = setting.strPhotoSourcePath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strPhotoSourcePath = setting.strPhotoSourcePath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strPhotoSourcePath = setting.strPhotoSourcePath.Replace("%D", DateTime.Now.ToString("dd"));
            try
            {
                if (!setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    if (!Directory.Exists(setting.strPhotoSourcePath))
                        Directory.CreateDirectory(setting.strPhotoSourcePath);
                }

                if (setting.bNeedCopyFile)
                {
                    if (!Directory.Exists(setting.strXMLDownloadPath))
                        Directory.CreateDirectory(setting.strXMLDownloadPath);
                }

                if (setting.strFileCopyPath.Length != 0)
                    if (!Directory.Exists(setting.strFileCopyPath))
                        Directory.CreateDirectory(setting.strFileCopyPath);
            }
            catch (Exception ex)
            {
                main.SetTextOnRichTextBox("Path Setting Error: "+ex.Message.Trim()+" ");
            }
            return;
        }
    }
}


public class ADOPhotoObject
{
    public String str_TeleCode;
    public String str_PhotoName;
    public String str_PhotoTime;
    public String str_Title;
    public int iSize;
    public String str_Format;
    public String str_Caption;
    public String str_Location;
    public String str_PhotoFlag;
    public String str_BeginTime;
    public byte[] photo_Bitmap;
}

public class ADOOjectWires
{
    public String str_TeleName;
    public String dt_TeleTime ;
    public int iwords;
    public String str_Content;
    public String str_Keyword;
    public String str_Urgent;
    public String str_Location;
    public String str_Flag;
    public String str_Note;
    public String str_Language;
    public String str_Title;
}
