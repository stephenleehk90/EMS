using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Reuters_with_Web_Service.com.reuters.rmb;
using System.Net;
using System.Xml.Linq;
using System.IO;
using System.Data;
using System.Drawing;
using System.Threading;
using System.Xml.Serialization;
/*
 * This class for handle data insert on EMS6.5 Version
*/

namespace Reuters_with_Web_Service
{
    public class WebServiceHandler_MP : WebServiceHandler
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
        String strPhotoAddInfo;

        public WebServiceHandler_MP(String strUserName, String strPassword)
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
            if (setting.strAgencyTableCode.Length == 0)
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
                            return item; });
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
                //XML_Reader reader = new XML_Reader("", false);
                Reader reader;
                if (setting.strXmlType.Equals("Bloomberg"))
                {
                    reader = new BB_XML_Reader("", false);
                    ((BB_XML_Reader)reader).SetStrFilePhyName(lstNewList[j].strFilePhyName.ToString());
                }
                else if (setting.strXmlType.Equals("新華社"))
                {
                    if (lstNewList[j].strFilePhyName.Contains("_simple.xml"))
                        continue;
                    reader = new XHS_XML_Reader("", false);
                }
                else
                    reader = new XML_Reader("", false);
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
                    if (setting.strXmlType.Equals("Bloomberg"))
                    {
                        reader = new BB_XML_Reader(strXMLContent, setting.bIsChannel);
                        ((BB_XML_Reader)reader).SetStrFilePhyName(lstNewList[j].strFilePhyName.ToString());
                    }
                    else if (setting.strXmlType.Equals("新華社"))
                        reader = new XHS_XML_Reader(strXMLContent, setting.bIsChannel);
                    else
                        reader = new XML_Reader(strXMLContent, setting.bIsChannel);
                    reader.SetNewsID(lstNewList[j].id.ToString());
                }
                else
                {
                    if (setting.strXmlType.Equals("Bloomberg"))
                    {
                        reader = new BB_XML_Reader(lstNewList[j].id, setting.bIsChannel);
                        ((BB_XML_Reader)reader).SetStrFilePhyName(lstNewList[j].strFilePhyName.ToString());
                    }
                    else if (setting.strXmlType.Equals("新華社"))
                        reader = new XHS_XML_Reader(lstNewList[j].id, setting.bIsChannel);
                    else
                        reader = new XML_Reader(lstNewList[j].id, setting.bIsChannel);
                }
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
                List<FileSpec> lst_Insert = new List<FileSpec>();
                GetInsertTableList(reader, setting.lst_FileSpec, ref lst_Insert);
                if (setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    int iFirst = 0, iLast = 0;
                    String []words = setting.str_SubstringName.Split(',');
                    iFirst = int.Parse(words[0]);
                    iLast = int.Parse(words[1]);
                    String str_TeleId = "";
                    try
                    {
                        str_TeleId = reader.GetNewsID().Replace(":", "__").Substring(iFirst, reader.GetNewsID().Replace(":", "__").Length - iLast - iFirst);
                    }catch(Exception ex)
                    {
                        SetTextToMain(ex.Message.Trim()+":"+ reader.GetNewsID().Replace(":", "__"));
                        str_TeleId = reader.GetNewsID().Replace(":", "__");
                    }
                    String strHeadline = ""; strHeadline = reader.GetStrHeadLine().Length > 250 ? reader.GetStrHeadLine().Substring(0, 250) : reader.GetStrHeadLine();
                    while (true)
                    {
                        if (strHeadline.Length <= 0)
                            break;
                        if (strHeadline[strHeadline.Length - 1] == '\'')
                        {
                            strHeadline = strHeadline.Remove(strHeadline.Length - 1);
                        }
                        else
                            break;
                    }
                    strValue = String.Format("(N'{0}',N'{1}',{2},N'{3}',N'{4}',N'{5}',N'{6}',N'{7}',N'{8}',N'{9}',N'{10}')",
                        str_TeleId, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                        reader.GetWordCounts(), strHeadline + "\r\n" + reader.GetStrContent().Replace( "   \r\n","_|_").Replace("\r\n", "\n").Replace("_|_", "   \r\n"), strHeadline, 
                        "", "", reader.GetUrgent(), "N", "", "E");
                    bool nRet = true;
                    for (int i = 0; i < lst_Insert.Count; i++)
                    {
                        {
                            InsertRecordToDB(strValue, lst_Insert[i].strAgencyCode, setting.strAgencyType, setting, "");
                            bInsertToDefault = false;
                            String strMessage = String.Format("{0} was insert into {1}", reader.GetNewsID(), lst_Insert[i].strAgencyCode);
                            SetTextToMain(strMessage);
                        }
                    }
                    if (bInsertToDefault)
                    {
                        nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType, setting,"");
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
                    if(Path.GetExtension(info.strSourcePhotoName).Equals(".eps", StringComparison.OrdinalIgnoreCase))
                        continue;
                    if (!DownloadPhoto(ref info, setting, reader.GetNewsDateTime().ToLocalTime()))
                        continue;
                    
                    strPhotoAddInfo = "";
                    if (main.isLoadAddInfo)
                        strPhotoAddInfo = GetPhotoAdditionalInfo(setting.strPhotoSourcePath_Root+info.strPhotoSourcePath);
                    List<MetaKeyValue> lst_photoMeta = LoadPhotoMeta(setting.strPhotoSourcePath_Root + info.strPhotoSourcePath);
                    String strHeadline = "";strHeadline = reader.GetStrHeadLine().Length > 250 ? reader.GetStrHeadLine().Substring(0, 250) : reader.GetStrHeadLine();
                    while (true)
                    {
                        if (strHeadline.Length <= 0)
                            break;
                        if (strHeadline[strHeadline.Length - 1] == '\'' && strHeadline.Length>0)
                        {
                            strHeadline = strHeadline.Remove(strHeadline.Length - 1);
                        }
                        else
                            break;
                    }
                    bool nRet = true;
                    for (int i = 0; i < lst_Insert.Count; i++)
                    {
                        {
                            String strPhotoID = DB.GetNextPhotoID();
                            if (strPhotoID.Length == 0)
                            {
                                SetTextToMain(String.Format("Cannot get the next photo id"));
                                return;
                            }
                            strValue = String.Format("(N'{0}',N'{1}',N'{2}',N'{3}',N'{4}',{5},N'{6}',N'{7}',N'{8}',N'{9}'," +
                               "{10}, {11},N'{12}', {13},{14}, {15},N'{16}', N'{17}',N'{18}', N'{19}',N'{20}',N'{21}')",
                               lst_Insert[i].strAgencyCode, strPhotoID, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                               DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), strHeadline
                               , reader.GetPhotoInfo().strSize, "jpg", "", "N", reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                               DB.GetAgencyId(setting.strAgencyTableCode), "0", info.strPhotoSourcePath, info.strPhotoHeight.Length == 0 ? "0" : info.strPhotoHeight,
                               info.strPhotoWidth.Length == 0 ? "0" : info.strPhotoWidth, "24", info.strPhotoMiddlePath, info.strPhotoSmallPath, info.strPhotoSourcePath,
                               DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"),
                               reader.GetStrCaption() + "\r\n" + strPhotoAddInfo);
                            nRet = InsertRecordToDB(strValue, lst_Insert[i].strAgencyCode, setting.strAgencyType, setting, strPhotoID);
                            if (nRet)
                            {
                                String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetNewsID().Replace("__", ":"), 1);
                                DB.ExecuteSQL(strSql);
                                for (int ii = 0; ii < lst_photoMeta.Count; ii++)
                                {
                                    strSql = String.Format("insert into dbmedia_av_info values(N'{0}',N'{1}',N'{2}',N'{3}')", strPhotoID, "Image", lst_photoMeta[ii].strKey, lst_photoMeta[ii].strDBVal);
                                    DB.ExecuteSQL(strSql);
                                }
                            }
                            bInsertToDefault = false;
                        }
                    }
                    if (bInsertToDefault)
                    {
                        String strPhotoID = DB.GetNextPhotoID();
                        if (strPhotoID.Length == 0)
                        {
                            SetTextToMain(String.Format("Cannot get the next photo id"));
                            return;
                        }
                        strValue = String.Format("(N'{0}',N'{1}',N'{2}',N'{3}',N'{4}',{5},N'{6}',N'{7}',N'{8}',N'{9}'," +
                            "{10}, {11},N'{12}', {13},{14}, {15},N'{16}', N'{17}',N'{18}', N'{19}',N'{20}',N'{21}', 0)",
                        setting.strAgencyTableCode, strPhotoID, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                        DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), strHeadline
                        , reader.GetPhotoInfo().strSize, "jpg", "", "N", reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                        DB.GetAgencyId(setting.strAgencyTableCode), "0", info.strPhotoSourcePath, info.strPhotoHeight.Length == 0 ? "0" : info.strPhotoHeight,
                        info.strPhotoWidth.Length == 0 ? "0" : info.strPhotoWidth, "24", info.strPhotoMiddlePath, info.strPhotoSmallPath, info.strPhotoSourcePath,
                        DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"),
                        reader.GetStrCaption() + "\r\n" + strPhotoAddInfo);
                        nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType, setting, strPhotoID);
                        if (nRet)
                        {
                            String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetNewsID().Replace("__", ":"), 1);
                            DB.ExecuteSQL(strSql);
                            for (int ii = 0; ii < lst_photoMeta.Count; ii++)
                            {
                                strSql = String.Format("insert into dbmedia_av_info values(N'{0}',N'{1}',N'{2}',N'{3}')", strPhotoID, "Image", lst_photoMeta[ii].strKey, lst_photoMeta[ii].strDBVal);
                                DB.ExecuteSQL(strSql);
                            }
                        }
                    }
                    if (nRet)
                    {
                        String strMessage = String.Format("Photo {0} was prcessed and inserted into Database", info.strSourcePhotoName);
                        SetTextToMain(strMessage);
                        iTotalNewsCount++;
                        try
                        {
                            String strPath = setting.strPhotoSourcePath_Format.Replace("%Y", reader.GetNewsDateTime().ToLocalTime().Year.ToString());
                            strPath = strPath.Replace("%M", reader.GetNewsDateTime().ToLocalTime().ToString("MM"));
                            strPath = strPath.Replace("%D", reader.GetNewsDateTime().ToLocalTime().ToString("dd"));

                            strPath = strPath.Trim().LastIndexOf("\\") == strPath.Trim().Length ? strPath.Trim() : strPath.Trim() + "\\";
                            //String strTxtFileName = reader.GetXMLFileName().Substring(0, reader.GetXMLFileName().LastIndexOf(".")) + ".txt";// Picture Caption
                            String strTxtFileName = reader.GetPhotoInfo().strSourcePhotoName.Substring(0, reader.GetPhotoInfo().strSourcePhotoName.LastIndexOf(".")) + ".txt";// Picture Caption
                            File.WriteAllText(strPath + strTxtFileName, reader.GetStrCaption().Trim() + "\r\n" + strPhotoAddInfo);
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

                    // Edit 2023-08
                    if (info.strLimitedPhotoName.Trim() != "")
                    {
                        if (File.Exists(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                            setting.strChannelsName + info.strLimitedPhotoName : setting.strChannelsName + "\\" + info.strLimitedPhotoName))
                            File.Delete(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                                setting.strChannelsName + info.strLimitedPhotoName : setting.strChannelsName + "\\" + info.strLimitedPhotoName);
                    }
                    // Edit 2023-08
                
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

        private bool DownloadPhoto(ref PhotoInfo info, SettingParameter setting, DateTime dt_NewsDate)
        {
            bool nRet = true;
            try
            {
                String strDestPath_Source = setting.strPhotoSourcePath_Format.Replace("%Y", dt_NewsDate.Year.ToString());
                strDestPath_Source = strDestPath_Source.Replace("%M", dt_NewsDate.ToString("MM"));
                strDestPath_Source = strDestPath_Source.Replace("%D", dt_NewsDate.ToString("dd"));
                String strDestPath_Middle = setting.strPhotoMiddlePath_Format.Replace("%Y", dt_NewsDate.Year.ToString());
                strDestPath_Middle = strDestPath_Middle.Replace("%M", dt_NewsDate.ToString("MM"));
                strDestPath_Middle = strDestPath_Middle.Replace("%D", dt_NewsDate.ToString("dd"));
                String strDestPath_Small = setting.strPhotoSmallPath_Format.Replace("%Y", dt_NewsDate.Year.ToString());
                strDestPath_Small = strDestPath_Small.Replace("%M", dt_NewsDate.ToString("MM"));
                strDestPath_Small = strDestPath_Small.Replace("%D", dt_NewsDate.ToString("dd"));

                if (!Directory.Exists(strDestPath_Source))
                    Directory.CreateDirectory(strDestPath_Source);
                if (!Directory.Exists(strDestPath_Middle))
                    Directory.CreateDirectory(strDestPath_Middle);
                if (!Directory.Exists(strDestPath_Small))
                    Directory.CreateDirectory(strDestPath_Small);

                String strDestSourcePath = strDestPath_Source.Trim().LastIndexOf("\\") == strDestPath_Source.Trim().Length-1 ?
                    strDestPath_Source.Trim() + info.strSourcePhotoName : strDestPath_Source.Trim() + "\\" + info.strSourcePhotoName;
                String strDestMiddlePath = strDestPath_Middle.Trim().LastIndexOf("\\") == strDestPath_Middle.Trim().Length-1 ?
                    strDestPath_Middle.Trim() + info.strMiddlePhotoName : strDestPath_Middle.Trim() + "\\" + info.strMiddlePhotoName;
                String strDestSmallPath = strDestPath_Small.Trim().LastIndexOf("\\") == strDestPath_Small.Trim().Length-1 ?
                    strDestPath_Small.Trim() + info.strSmallPhotoName : strDestPath_Small.Trim() + "\\" + info.strSmallPhotoName;
                List<String> lst_DestPhotoName = new List<String>();
                lst_DestPhotoName.Add(strDestSourcePath);
                lst_DestPhotoName.Add(strDestMiddlePath);
                lst_DestPhotoName.Add(strDestSmallPath);
                List<String> lst_PhotoLink = new List<String>();
                lst_PhotoLink.Add(info.strPhotoSourcePath);
                lst_PhotoLink.Add(info.strPhotoMiddlePath);
                lst_PhotoLink.Add(info.strPhotoSmallPath);
                if (setting.bIsChannel)
                {
                    info.strPhotoSourcePath = strDestSourcePath;
                    info.strPhotoMiddlePath = strDestMiddlePath;
                    info.strPhotoSmallPath = strDestSmallPath;
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
                    String strSourceBasePhotoPath = "", strSourceMiddlePhotoPath = "", strSourceSmallPhotoPath = "";
                    strSourceSmallPhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length-1 ?
                        setting.strChannelsName.Trim() + info.strSmallPhotoName : setting.strChannelsName.Trim() + "\\" + info.strSmallPhotoName;
                    strSourceMiddlePhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length-1 ?
                        setting.strChannelsName.Trim() + info.strMiddlePhotoName : setting.strChannelsName.Trim() + "\\" + info.strMiddlePhotoName;
                    strSourceBasePhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length-1 ?
                        setting.strChannelsName.Trim() + info.strSourcePhotoName : setting.strChannelsName.Trim() + "\\" + info.strSourcePhotoName;
                    info.strPhotoSourcePath = strDestSourcePath.Replace(setting.strPhotoSourcePath_Root, "");
                    info.strPhotoMiddlePath = strDestMiddlePath.Replace(setting.strPhotoMiddlePath_Root, "");
                    info.strPhotoSmallPath = strDestSmallPath.Replace(setting.strPhotoSmallPath_Root, "");
                    try
                    {
                        FileInfo fileInfo = new FileInfo(strSourceBasePhotoPath);
                        if (fileInfo.Exists)
                        {
                           
                        }
                        else { return false; }
                        // if Base Picture cannot found, then nothing can be done. ---> return false
                        fileInfo = new FileInfo(strSourceMiddlePhotoPath);
                        if (fileInfo.Exists)
                        {
                            if (strSourceMiddlePhotoPath != strDestMiddlePath)
                                fileInfo.CopyTo(strDestMiddlePath, true);
                        }
                        else
                        {
                            try
                            {
                                LEADLib.LEAD lead = new LEADLib.LEAD();
                                lead.Load(strSourceBasePhotoPath, 0, 0, 1);
                                float bitH = lead.BitmapHeight;
                                float bitW = lead.BitmapWidth;
                                float height = float.Parse(DB.GetConfig("photo_heightM"));
                                float width = float.Parse(DB.GetConfig("photo_widthM"));
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

                                lead.Save(strDestMiddlePath, 10, 24, 10, 0);
                            }
                            catch (Exception ex)
                            {
                                SetTextToMain(String.Format("Failed to process middle picture from picture {0} by reason: {1}", info.strSourcePhotoName, ex.Message.Trim()));
                                return false;
                            }
                        }
                        
                        fileInfo = new FileInfo(strSourceSmallPhotoPath);
                        if (fileInfo.Exists)
                        {
                            if (strSourceSmallPhotoPath != strDestSmallPath)
                                fileInfo.CopyTo(strDestSmallPath, true);
                        }
                        else
                        { 
                            try
                            {
                                LEADLib.LEAD lead = new LEADLib.LEAD();
                                lead.Load(strSourceBasePhotoPath, 0, 0, 1);
                                float bitH = lead.BitmapHeight;
                                float bitW = lead.BitmapWidth;
                                float height = float.Parse(DB.GetConfig("photo_height"));
                                float width = float.Parse(DB.GetConfig("photo_width"));
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
                                lead.Save(strDestSmallPath, 10, 24, 10, 0);
                            }
                            catch (Exception ex) {
                                if (!SpecialHandleOnEPS(info, strSourceMiddlePhotoPath, strDestSmallPath, true))
                                {
                                    SetTextToMain(String.Format("Failed to process small picture from picture {0} reason by: {1}", info.strSourcePhotoName, ex.Message.Trim()));
                                    return false;
                                }
                            }
                        }
                    }
                    catch (Exception ex) { }
                }
            }
            catch (Exception ex) 
            {
                SetTextToMain(String.Format("Error ocurrs in download picture process"));
                return false; 
            }

            return nRet;
        }

        private bool SpecialHandleOnEPS(PhotoInfo info, String strProcessFilePath, String strDestPath, bool isProcSmallPhoto)
        {
            bool nRet = true;
            try
            { 
                LEADLib.LEAD lead = new LEADLib.LEAD();
                lead.Load(strProcessFilePath, 0, 0, 1);
                float bitH = lead.BitmapHeight;
                float bitW = lead.BitmapWidth;
                float height = 0; 
                float width = 0 ; 
                if(isProcSmallPhoto)
                {
                    height= float.Parse(DB.GetConfig("photo_height"));
                    width =float.Parse(DB.GetConfig("photo_width"));
                }else
                {
                    height= float.Parse(DB.GetConfig("photo_heightM"));
                    width =float.Parse(DB.GetConfig("photo_widthM"));
                }
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
                lead.Save(strDestPath, 10, 24, 10, 0);
            }
            catch(Exception ex)
            {
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

        private void GetInsertTableList(Reader reader, List<FileSpec> lst_FileSpec, ref List<FileSpec> lst_Result)
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

        private bool InsertRecordToDB(String strValue, String strTableName, String strTableType, SettingParameter settingParam, String photoId)
        {
            bool nRet = true;
            String strSQL = "";
            if (strTableType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                strSQL = String.Format("Insert into {0} ( tele_name , tele_time , tele_words , tele_content,tele_title, tele_location , tele_keyword,tele_urgent,tele_flag,tele_note , tele_language) values {1}",
                    strTableName, strValue);
            else
            {
                strSQL = String.Format("Insert into wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format," +
                    "photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path," +
                    "photo_src_path,last_modify_time,real_time, photo_caption, media_type) values {0}", strValue);
                nRet = DB.ExecuteSQL(strSQL);
                strSQL = String.Format("Insert into rootPhotoRelation(SrcRoot ,SmlRoot ,MidRoot ,photo_name) values ({0}, {1}, {2}, N'{3}')",
                    settingParam.iSourcePathID, settingParam.iSmallPathID, settingParam.iMiddlePathID, photoId);
                nRet = DB.ExecuteSQL(strSQL);
                strSQL = String.Format("Update wires_photo_src set rootrelation_Id = (select ID from rootPhotoRelation where photo_name = N'{0}')", photoId);
            }
            nRet = DB.ExecuteSQL(strSQL);
            return nRet;
        }

        private void UpdateSettingPath(SettingParameter setting)
        {
            setting.strChannelsName = setting.strChannelsName_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strChannelsName = setting.strChannelsName.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strChannelsName = setting.strChannelsName.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strPhotoSourcePath = setting.strPhotoSourcePath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strPhotoSourcePath = setting.strPhotoSourcePath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strPhotoSourcePath = setting.strPhotoSourcePath.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strPhotoSmallPath = setting.strPhotoSmallPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strPhotoSmallPath = setting.strPhotoSmallPath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strPhotoSmallPath = setting.strPhotoSmallPath.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strPhotoMiddlePath = setting.strPhotoMiddlePath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strPhotoMiddlePath = setting.strPhotoMiddlePath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strPhotoMiddlePath = setting.strPhotoMiddlePath.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strXMLDownloadPath = setting.strXMLDownloadPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strXMLDownloadPath = setting.strXMLDownloadPath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strXMLDownloadPath = setting.strXMLDownloadPath.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strFileCopyPath = setting.strFileCopyPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strFileCopyPath = setting.strFileCopyPath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strFileCopyPath = setting.strFileCopyPath.Replace("%D", DateTime.Now.ToString("dd"));

            try
            {
                if (!setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    if (!Directory.Exists(setting.strPhotoSourcePath))
                        Directory.CreateDirectory(setting.strPhotoSourcePath);
                    if (!Directory.Exists(setting.strPhotoMiddlePath))
                        Directory.CreateDirectory(setting.strPhotoMiddlePath);
                    if (!Directory.Exists(setting.strPhotoSmallPath))
                        Directory.CreateDirectory(setting.strPhotoSmallPath);
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

        private String GetPhotoAdditionalInfo(String strPhotoPath)
        {
            String strAddInfo = "";
            String strTemp = "";
            List<short> lst_CommentCons = new List<short>();
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_OBJECTNAME);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_CATEGORY);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_SUPPLEMENTALCATEGORY);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_DATECREATED);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_BYLINE);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_BYLINETITLE);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_CITY);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_SUBLOCATION);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_PROVINCE_STATE);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_PRIMARYLOCATIONNAME);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_ORIGINALTRANSMISSIONREFERENCE);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_HEADLINE);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_CREDIT);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_SOURCE);
            lst_CommentCons.Add((short)LEADLib.CommentConstants.CMNT_IPTC_AUTHOR);
            try
            {
                LEADLib.LEAD lead = new LEADLib.LEAD();
                lead.Load(strPhotoPath, 0, 0, 1);
                for (int i = 0; i < lst_CommentCons.Count; i++)
                {
                    strTemp = lead.ReadComment(strPhotoPath, 0, lst_CommentCons[i]);
                    if (strTemp == null)
                        continue;
                    if (strTemp.Length>0)
                        strAddInfo += strTemp + "\r\n";
                }
            }
            catch (Exception ex)
            {
                //main.SetTextOnRichTextBox("Getting Addtional Info Error: " + ex.Message.Trim() + " ");
            }
            strAddInfo = strAddInfo.Replace("'", "''");
            return strAddInfo;
        }

        private List<MetaKeyValue> LoadPhotoMeta(String strPhotoPath)
        {
            //strPhotoPath = "d:\\dbgraphic_log\\20140325\\20130408_221010.jpg";
            String strMeta = "", strTemp = "";
            List<MetaKeyValue> lst_MetaData = new List<MetaKeyValue>();
            try
            {
                LEADLib.LEAD lead = new LEADLib.LEAD();
                lead.Load(strPhotoPath, 0, 0, 1);
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZDESC);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("Title", strTemp));
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_IPTC_SUBJECTREFERENCE);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("Subject", strTemp));
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZARTIST);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("Author", strTemp));
                byte[] b = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_USERCOMMENT);
                if (b != null && b.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("Comment", System.Text.Encoding.Default.GetString(b)));
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZDATETIMEORIGINAL);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("Date taken", strTemp));
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZCOPYRIGHT);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("Copyright", strTemp));

                int x = -1, y = -1, bit = -1;
                float YRes = -1, XRes = -1;
                Bitmap bmp = new Bitmap(strPhotoPath);
                x = bmp.Width;
                y = bmp.Height;
                bit = lead.BitmapBits;
                YRes = bmp.VerticalResolution;
                XRes = bmp.HorizontalResolution;
                String str_Compress = "";
                str_Compress = new ImageFormatConverter().ConvertToString(bmp.RawFormat);
                if (x != -1)
                    lst_MetaData.Add(new MetaKeyValue("Width", x.ToString()));
                if (y != -1)
                    lst_MetaData.Add(new MetaKeyValue("Image", y.ToString()));
                if (bit != -1)
                    lst_MetaData.Add(new MetaKeyValue("Bit depth", bit.ToString()));
                if (x != 0 && y != 0)
                    lst_MetaData.Add(new MetaKeyValue("Dimensions", x.ToString() + " x " + y.ToString()));
                if (str_Compress != null && str_Compress.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("Compression", str_Compress));

                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZMAKE);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("camera maker", strTemp));
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZMODEL);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("camera model", strTemp));
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZSOFTWARE);
                if (strTemp != null && strTemp.Length > 0)
                    lst_MetaData.Add(new MetaKeyValue("program name", strTemp));
                int [] i = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_FNUMBER);
                if (i != null)
                {
                    if (i[1] != 0)
                        lst_MetaData.Add(new MetaKeyValue("F-stop", (i[0] / i[1]).ToString() + "." +(i[0] % i[1]).ToString()));
                }
                i = null;
                i = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_EXPOSURETIME);
                if (i != null)
                {
                    lst_MetaData.Add(new MetaKeyValue("exposure time", i[0].ToString()+"/"+i[1].ToString()));
                }
                short s = -1;
                try
                {
                    s = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_ISOSPEEDRATINGS) == null ?
                        -1 : lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_ISOSPEEDRATINGS);
                    if (s != -1)
                        lst_MetaData.Add(new MetaKeyValue("ISO speed", s.ToString()));
                }
                catch (Exception ex)
                {
                }
                i = null;
                i = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_EXPOSUREBIAS);
                if (i != null)
                {
                    lst_MetaData.Add(new MetaKeyValue("exposure bias", i[0].ToString() + "/" + i[1].ToString() + " EV"));
                }
                i = null;
                i = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_FOCALLENGTH);
                if (i != null)
                {
                    lst_MetaData.Add(new MetaKeyValue("focal length", i[0].ToString() + "." + i[1].ToString() + " mm"));
                }
                i = null;
                i = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_MAXAPERTURE);
                if (i != null)
                {
                    lst_MetaData.Add(new MetaKeyValue("max aperture"," F/"+i[0].ToString() + "." + i[1].ToString()));
                }

                try
                {
                    s = -1;
                    s = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_METERINGMODE) == null ?
                        -1 : lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_METERINGMODE);
                    if (s != -1)
                    {
                        switch (s)
                        {
                            case 1: strTemp = "Average";
                                break;
                            case 2: strTemp = "Center Weighted Average";
                                break;
                            case 3: strTemp = "Spot";
                                break;
                            case 4: strTemp = "MultiSpot";
                                break;
                            case 5: strTemp = "Pattern";
                                break;
                            case 6: strTemp = "Partial";
                                break;
                            case 255: strTemp = "Others";
                                break;
                            default:
                                strTemp = "Unidentified";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("metering mode", strTemp));
                    }
                }
                catch (Exception ex) { }
                try
                {
                    int j = -1;
                    j = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SUBJECTDISTANCERANGE) == null ?
                        -1 : lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SUBJECTDISTANCERANGE);
                    if (j != -1)
                    {
                        switch (j)
                        {
                            case 0: strTemp = "Unknown";
                                break;
                            case 1: strTemp = "Macro";
                                break;
                            case 2: strTemp = "Close view";
                                break;
                            case 3: strTemp = "Distant view";
                                break;
                            default:
                                strTemp = "none";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("subject distance", strTemp));
                    }
                }
                catch (Exception ex) { }
                try
                {
                    s = -1;
                    s = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_FLASH) == null ?
                        -1 : lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_FLASH);
                    if (s != -1)
                    {
                        if (s == 0)
                            strTemp = "on";
                        else
                            strTemp = "off";
                        lst_MetaData.Add(new MetaKeyValue("flash mode", strTemp));
                    }
                }
                catch (Exception ex) { }
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_FOCALLENGTHIN35MMFILM);
                if (strTemp != null && strTemp.Length > 0)
                {
                    if (strTemp == "0")
                        strTemp = "Unknown";
                    lst_MetaData.Add(new MetaKeyValue("35mm focal length", strTemp));
                }

               
                try
                {   
                    s = -1;
                    s = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_COLORSPACE)==null?
                    -1 : lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_COLORSPACE);
                    if (s != -1)
                    {
                    
                        switch (Int32.Parse(strTemp))
                        {
                            case 0xFFFF: strTemp = "Uncalibrated";
                                break;
                            case 1: strTemp = "sRGB";
                                break;
                            default:
                                strTemp = "Reserved";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("photometric interpretation", strTemp));
                    }
                }
                catch (Exception ex)
                {}
                strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_CONTRAST);
                if (strTemp != null && strTemp.Length > 0)
                {
                    try
                    {
                        switch (Int32.Parse(strTemp))
                        {
                            case 0: strTemp = "Normal";
                                break;
                            case 1: strTemp = "Soft";
                                break;
                            case 2: strTemp = "Hard";
                                break;
                            default:
                                strTemp = "Reserved";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("contrast", strTemp));
                    }
                    catch (Exception ex)
                    {
                    }
                }
                
                i = null;
                i = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_BRIGHTNESS);
                if (i != null)
                {
                    if (i[0] > 0 && i[1] != 0)
                        strTemp = String.Format("{0}.{1}", i[0] / i[1], i[0] % i[1]);
                    else if (i[1] != 0)
                    {
                        i[0] = i[0] * -1;
                        strTemp = String.Format("-{0}.{1}", i[0] / i[1], i[0] % i[1]);
                    }
                    lst_MetaData.Add(new MetaKeyValue("brightness",strTemp));
                }

                try
                {
                    strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_LIGHTSOURCE);
                    if (strTemp != null && strTemp.Length > 0)
                    {
                        switch (Int32.Parse(strTemp))
                        {
                            case 0: strTemp = "Unidentified";
                                break;
                            case 1: strTemp = "Daylight";
                                break;
                            case 2: strTemp = "Florescence light";
                                break;
                            case 3: strTemp = "Tungsten Lamp";
                                break;
                            case 17: strTemp = "Standard light source A";
                                break;
                            case 18: strTemp = "Standard light source B";
                                break;
                            case 19: strTemp = "Standard light source C";
                                break;
                            case 20: strTemp = "D55";
                                break;
                            case 21: strTemp = "D65";
                                break;
                            case 22: strTemp = "D75";
                                break;
                            default:
                                strTemp = "Others";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("light source:", strTemp));
                    }
                }
                catch (Exception ex)
                {
                }
                
                try
                {   
                    strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_EXPOSUREPROGRAM);
                    if (strTemp != null && strTemp.Length > 0)
                    {
                        switch (Int32.Parse(strTemp))
                        {
                            case 0: strTemp = "Not defined";
                                break;
                            case 1: strTemp = "Manual";
                                break;
                            case 2: strTemp = "Aperture priority";
                                break;
                            case 4: strTemp = "Shutter priority";
                                break;
                            case 5: strTemp = "Creative program";
                                break;
                            case 6: strTemp = "Action program";
                                break;
                            case 7: strTemp = "Portrait mode";
                                break;
                            default:
                                strTemp = "reserved";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("exposure program:", strTemp));
                    }
                }
                catch (Exception ex)
                {
                }
                
                try
                {
                    strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SATURATION);
                    if (strTemp != null && strTemp.Length > 0)
                    {
                        switch (Int32.Parse(strTemp))
                        {
                            case 0: strTemp = "Normal";
                                break;
                            case 1: strTemp = "Low saturation";
                                break;
                            case 2: strTemp = "High saturation";
                                break;
                            default:
                                strTemp = "Reserved";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("saturation", strTemp)); 
                    }
                }
                catch (Exception ex)
                {
                }
               
                
                try
                {   
                    strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SHARPNESS);
                    if (strTemp != null && strTemp.Length > 0)
                    {
                        switch (Int32.Parse(strTemp))
                        {
                            case 0: strTemp = "Normal";
                                break;
                            case 1: strTemp = "Low Soft";
                                break;
                            case 2: strTemp = "High Hard";
                                break;
                            default:
                                strTemp = "Reserved";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("sharpness", strTemp));
                    }
                }
                catch (Exception ex)
                {
                }

                try
                {
                    strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_WHITEBALANCE);
                    if (strTemp != null && strTemp.Length > 0)
                    {
                        switch (Int32.Parse(strTemp))
                        {
                            case 0: strTemp = "Auto white balance";
                                break;
                            case 1: strTemp = "Manual white balance";
                                break;
                            default:
                                strTemp = "Reserved";
                                break;
                        }
                        lst_MetaData.Add(new MetaKeyValue("white balance", strTemp));
                    }
                }
                catch (Exception ex)
                {
                }
                try
                {
                    strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_IPTC_DATECREATED);
                    if (strTemp != null && strTemp.Length > 0)
                        lst_MetaData.Add(new MetaKeyValue("date created", strTemp));
                }
                catch (Exception ex)
                { }
                try
                {
                    strTemp = lead.ReadComment(strPhotoPath, 0, (short)LEADLib.CommentConstants.CMNT_SZHOSTCOMP);
                    if (strTemp != null && strTemp.Length > 0)
                        lst_MetaData.Add(new MetaKeyValue("computer", strTemp));
                }
                catch (Exception ex)
                { }
                FileInfo info = new FileInfo(strPhotoPath);
                lst_MetaData.Add(new MetaKeyValue("size", info.Length.ToString()));
                lst_MetaData.Add(new MetaKeyValue("date create", info.CreationTime.ToString()));
                lst_MetaData.Add(new MetaKeyValue("date modify", info.LastWriteTime.ToString()));
                lst_MetaData.Add(new MetaKeyValue("file path", strPhotoPath));
                
            }
            catch (Exception ex)
            {
            }
            return lst_MetaData;
        }

    }
}

public class MetaKeyValue
{
    public String strKey;
    public String strDBVal;
    public MetaKeyValue(String strKey, String strDBVal)
    {
        this.strKey = strKey;
        this.strDBVal = strDBVal;
        strDBVal = "";
    }
}