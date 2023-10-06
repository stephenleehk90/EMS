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
 * This class for handle data insert on EMS6.0 Version
*/

namespace Reuters_with_Web_Service
{
    public class WebServiceHandler_TKP : WebServiceHandler
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
        SettingHandler settingHandler;
        LEADLib.LEAD m_lead;

        public WebServiceHandler_TKP(String strUserName, String strPassword)
        {
            this.strUserName = strUserName;
            this.strPassword = strPassword;
            strMaxAge = "1h";
            lst_FaultNews = new List<FaultXMLList>();
            settingHandler = new SettingHandler();
            LEADLib.LEAD m_lead = new LEADLib.LEAD();
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

        // Edit Stephen 2022-10-05
        public List<String> GetDBDataIntoList(String strNewsType, bool bIsAP, string strTableName, List<FileSpec> lst_FileSpec)
        // Edit Stephen 2022-10-05
        {
            //process picture later
            DataTable table = new DataTable();
            String strSQL= "";

            // Edit Stephen 2022-10-05
            if (bIsAP)
            {
                strSQL = "select reuter_news_id from wires_reuters_record a ";
                strSQL = strSQL + " inner join " + strTableName + "  b on a.file_name = b.tele_name and a.isPhoto = 0 ";
                strSQL = strSQL + " and b.tele_time >= DateAdd(month, -1, GetDate())";
                strSQL = strSQL + " union select reuter_news_id from wires_reuters_record a ";
                strSQL = strSQL + " inner join wires_photo_src b on a.file_name = b.photo_name and a.isPhoto = 1 ";
                strSQL = strSQL + " and b.photo_time >= DateAdd(month, -1, GetDate())";

                for (int i = 0; i < lst_FileSpec.Count; i++)
                {
                    strSQL = strSQL + " union select reuter_news_id from wires_reuters_record a ";
                    strSQL = strSQL + " inner join " + lst_FileSpec[i].strAgencyCode + "  b on a.file_name = b.tele_name and a.isPhoto = 0 ";
                    strSQL = strSQL + " and b.tele_time >= DateAdd(month, -1, GetDate())";
                }               
                
            }
            // Edit Stephen 2022-10-05
            else
            {
                if (strNewsType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                    //strSQL = String.Format("select replace(tele_name,'__',':') from {0}", strAgencyCode);
                    strSQL = String.Format("select reuter_news_id from wires_reuters_record where isPhoto = 0");
                else
                    strSQL = String.Format("select reuter_news_id from wires_reuters_record where isPhoto = 1");
            }
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
                    //Pat 20160812 ignore file for bloomberg---------------------------------------------
                    if (setting.strXmlType.Equals("Bloomberg"))
                    {

                        if (setting.BBbIgnoreFile)
                        {
                            String strIgnore = setting.BBstrIgnoreStr;
                            String[] strIgnoreCode = strIgnore.Split(',');
                            foreach (String str in strIgnoreCode)
                            {
                                if (!str.Equals(""))
                                {
                                    List<FileInfo> lst_FilesTemp = dir.GetFiles().ToList().FindAll(delegate(FileInfo file)
                                    {
                                        return file.Extension.Equals(".xml", StringComparison.OrdinalIgnoreCase);
                                    }
                                    );
                                    if (lst_FilesTemp.Count == 0)
                                        break;
                                    foreach (FileInfo fi in lst_FilesTemp)
                                    {
                                        if (fi.Name.Contains(str))
                                        {
                                            if (setting.BBIgnoreFileAction == 0)//delete
                                            {
                                                fi.Delete();
                                            }
                                            else if (setting.BBIgnoreFileAction == 1)//move
                                            {
                                                String strTempPath = setting.BBstrMovePath;
                                                char last = strTempPath[strTempPath.Length - 1];
                                                if (last.Equals("\\"))
                                                {

                                                }
                                                else
                                                    strTempPath += "\\";
                                                if (!Directory.Exists(strTempPath))
                                                    Directory.CreateDirectory(strTempPath);
                                                fi.MoveTo(strTempPath + fi.Name);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //-----------------------------------------------------------------------------------
                    List<FileInfo> lst_Files = dir.GetFiles().ToList().FindAll(delegate(FileInfo file)
                    {
                        return file.Extension.Equals(".xml", StringComparison.OrdinalIgnoreCase);
                    }
                    );
                    lstNewList = lst_Files.ConvertAll<NewsItem>(delegate(FileInfo file) 
                        { 
                            NewsItem item = new NewsItem();
                            item.id = file.FullName;
                            item.strFilePhyName = file.Name;
                            return item; });
                }
                catch (Exception ex)
                {
                    SetTextToMain(ex.Message.Trim() + ":" + setting.strChannelsName);
                }
            }
            // Edit Stephen 2022-10-05
            List<String> lst_DBRecord = GetDBDataIntoList(setting.strAgencyType, setting.bIsAP, setting.strAgencyTableCode, setting.lst_FileSpec);
            // Edit Stephen 2022-10-05

            for (int j = 0; j < lstNewList.Count; j++)
            {
                //******* More log
//                main.SetTextOnRichTextBox(String.Format("Start Process on {0}\n ", lstNewList[j].id.ToString()));
//                SetTextToMain(String.Format("Start Process on {0} ", lstNewList[j].id.ToString()));
                //******* More log

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
                //Pat 20160721 add bloomberg-----------------------------------------------
                //XML_Reader reader = new XML_Reader("", false);
                Reader reader;
/*                if (setting.strXmlType.Equals("Bloomberg"))
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
*/
                
                //-------------------------------------------------------------------------
                if (setting.bIsChannel)
                {
                    try
                    {
                        strXMLContent = service.getItem(lstNewList[j].id.ToString(), setting.strAliasCode, null, null);
                    }
                    catch (Exception ex)
                    {
                       SetTextToMain(String.Format("Process on {0} catch the following error:\n {1} ", lstNewList[j].id.ToString(), ex.Message.Trim()));
                        continue;
                    }
                    //Pat 20160721 add bloomberg-----------------------------------------------
                    if (setting.strXmlType.Equals("Bloomberg"))
                    {
                        reader = new BB_XML_Reader(strXMLContent, setting.bIsChannel);
                        ((BB_XML_Reader)reader).SetStrFilePhyName(lstNewList[j].strFilePhyName.ToString());
                    }
                    else if (setting.strXmlType.Equals("新華社"))
                        reader = new XHS_XML_Reader(strXMLContent, setting.bIsChannel);
                    else
                        reader = new XML_Reader(strXMLContent, setting.bIsChannel);
                    //reader = new XML_Reader(strXMLContent, setting.bIsChannel);
                    //-------------------------------------------------------------------------
                    reader.SetNewsID(lstNewList[j].id.ToString());
                }
                else
                {
                    //Pat 20160721 add bloomberg-----------------------------------------------
                    if (setting.strXmlType.Equals("Bloomberg"))
                    {
                        reader = new BB_XML_Reader(lstNewList[j].id, setting.bIsChannel);
                        ((BB_XML_Reader)reader).SetStrFilePhyName(lstNewList[j].strFilePhyName.ToString());
                    }
                    else if (setting.strXmlType.Equals("新華社"))
                        reader = new XHS_XML_Reader(lstNewList[j].id, setting.bIsChannel);
                    else
                    {
                        //********************************************************
                        // Edit Stephen 2022-08-01
                        if (setting.bIsAP == true)
                        //********************************************************
                            reader = new AP_EMS6_XML_Reader(lstNewList[j].id);
                        else
                        // Edit Stephen 2022-08-01
                            reader = new XML_Reader(lstNewList[j].id, setting.bIsChannel);
                    }
                    //reader = new XML_Reader(lstNewList[j].id, setting.bIsChannel);
                    //-------------------------------------------------------------------------
                }
                //******* More log
//               SetTextToMain("Load reader finish");
                //******* More log
                reader.Read();
                //******* More log
//               SetTextToMain("Reader XML data finish");
                //******* More log

                //**************************************************************************************************
                // Edit 2021-08-04
                DateTime dtPhototime = reader.GetNewsDateTime().ToLocalTime();
                //**************************************************************************************************
                if (setting.bProcessTime)
                {
                    dtPhototime = DateTime.Now.Subtract(new TimeSpan(setting.dtBuffer.Hour, setting.dtBuffer.Minute, 0));
                }
                string str_Photo_time = dtPhototime.ToString("yyyy-MM-dd HH:mm:ss");
                //**************************************************************************************************


                if (reader.GetErrorMessage() != "")
                {
                    // Edit Stephen 2022-08-03
                    if (setting.bIsAP == true && reader.GetErrorMessage() == "Read News Content Fail")
                        continue;
                    // Edit Stephen 2022-08-03

                    FileInfo info = new FileInfo(lstNewList[j].id);
                    if (info.Length == 0)
                        SetTextToMain(String.Format("{0} is empty", lstNewList[j].id));
                    else
                        SetTextToMain(String.Format("{0} cannot insert to {1} -- {2}", lstNewList[j].id, setting.strAgencyTableCode, reader.GetErrorMessage()));
                    continue;
                }
                if (!setting.bIsChannel)
                {
                    String strFind = lst_DBRecord.Find(
                        delegate(String str)
                        {
                            // Edit 2023-07-07
//                            if (!setting.strXmlType.Equals("Reuters"))
                                return str == reader.GetNewsID().Replace("__", ":");
  //                          else   
    //                            return str == reader.GetXMLFileName().Replace("__", ":");
                            // Edit 2023-07-07
                        }
                        );
                    if (strFind != null)
                    {
                        SetTextToMain(".");

                        // Delete File if already process
                        if (setting.bIsAP == true && setting.bNeedDeleteFile)
                        {
                            try
                            {
                                // delete text
                                if (((AP_EMS6_XML_Reader)reader).GetXMLNewsType() == "TEXT")
                                    DeleteFiles(setting, reader.GetXMLFileName(), null, false);
                                else // delete photo
                                {
                                    PhotoInfo info = reader.GetPhotoInfo();
                                    if (info != null)
                                    {
                                        if (info.strSourcePhotoName.Trim() != "")
                                        {
                                            info.strPhotoSourcePath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                                            setting.strChannelsName.Trim() + info.strSourcePhotoName : setting.strChannelsName.Trim() + "\\" + info.strSourcePhotoName;

                                            //
                                            info.strSourcePath = info.strPhotoSourcePath;

                                            String strDestPath_Source = setting.strPhotoSourcePath_Format.Replace("%Y", dtPhototime.Year.ToString());
                                            strDestPath_Source = strDestPath_Source.Replace("%M", dtPhototime.ToString("MM"));
                                            strDestPath_Source = strDestPath_Source.Replace("%D", dtPhototime.ToString("dd"));

                                            String strDestSourcePath = "";

                                            if (strDestPath_Source.Trim() != "")
                                            {
                                                if (!Directory.Exists(strDestPath_Source))
                                                    Directory.CreateDirectory(strDestPath_Source);
                                                strDestSourcePath = strDestPath_Source.Trim().LastIndexOf("\\") == strDestPath_Source.Trim().Length ?
                                                    strDestPath_Source.Trim() + info.strSourcePhotoName : strDestPath_Source.Trim() + "\\" + info.strSourcePhotoName;
                                                info.strPhotoSourcePath = strDestSourcePath;
                                            }
                                            
                                            DeleteFiles(setting, reader.GetXMLFileName(), info, true);
                                        }
                                    }
                                }
                            }
                            catch (Exception)
                            {
                            }
                        }

                        continue;
                    }

                    //******* More log
//                   SetTextToMain("Start Insert Data");
                    //******* More log
                
                }
                String strValue = "", strFileSpecTableCode = "";
                bool bInsertToDefault = true;
                List<FileSpec> lst_Insert = new List<FileSpec>();
                GetInsertTableList(reader, setting.lst_FileSpec, ref lst_Insert);
                //******* More log
//               SetTextToMain("End Get Insert Table List");
                //******* More log

                if (setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    // Edit Stephen 2022-10-05
                    if (setting.bIsAP == true && (((AP_EMS6_XML_Reader)reader).GetXMLNewsType() != "TEXT"))
                    {
                    }
                    // Edit Stephen 2022-10-05
                    else
                    {
                        //******* More log
//                       SetTextToMain("Start Insert Text File");
                        //******* More log
                        int iFirst = 0, iLast = 0;
                        String[] words = setting.str_SubstringName.Split(',');
                        iFirst = int.Parse(words[0]);
                        iLast = int.Parse(words[1]);
                        String str_TeleId = "";
                        try
                        {
                            //Pat 20160721 add bloomberg
                            if (setting.strXmlType.Equals("Bloomberg"))
                                str_TeleId = reader.GetNewsID().Replace(":", "__");
                            else
                            {
                                // Edit 2023-07-07
                                if (!setting.strXmlType.Equals("Reuters"))
                                    str_TeleId = reader.GetNewsID().Replace(":", "__").Substring(iFirst, reader.GetNewsID().Replace(":", "__").Length - iLast - iFirst);
                                else
                                    str_TeleId = reader.GetXMLFileName().Replace(":", "__").Substring(iFirst, reader.GetXMLFileName().Replace(":", "__").Length - iLast - iFirst);
                                //
                            }
                        }
                        catch (Exception ex)
                        {
                            // Edit 2023-07-07
                            if (!setting.strXmlType.Equals("Reuters"))
                            {
                                SetTextToMain(ex.Message.Trim() + ":" + reader.GetNewsID().Replace(":", "__"));
                                str_TeleId = reader.GetNewsID().Replace(":", "__");
                            }
                            else
                            {
                                SetTextToMain(ex.Message.Trim() + ":" + reader.GetXMLFileName().Replace(":", "__"));
                                str_TeleId = reader.GetXMLFileName().Replace(":", "__");
                            }
                            // Edit 2023-07-07
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
                        //******* More log
//                       SetTextToMain("End Getting HeadLine");
                        //******* More log
                        strValue = String.Format("(N'{0}',N'{1}',{2},N'{3}',N'{4}',N'{5}',N'{6}',N'{7}',N'{8}',N'{9}',N'{10}')",
                            //                        str_TeleId, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                            str_TeleId, str_Photo_time,
                            reader.GetWordCounts(), strHeadline + "\r\n" + reader.GetStrContent().Replace("   \r\n", "_|_").Replace("\r\n", "\n").Replace("_|_", "   \r\n"), strHeadline,
                            "", "", reader.GetUrgent(), "N", "", "E");
                        bool nRet = true;
                        for (int i = 0; i < lst_Insert.Count; i++)
                        {
                            {
                                InsertRecordToDB(strValue, lst_Insert[i].strAgencyCode, setting.strAgencyType);
                                bInsertToDefault = false;
                                // Edit 2023-07-07
                                //                                String strMessage = String.Format("{0} was insert into {1}", reader.GetNewsID(), lst_Insert[i].strAgencyCode);
                                String strMessage = "";
                                if (!setting.strXmlType.Equals("Reuters"))
                                    strMessage = String.Format("{0} was insert into {1}", reader.GetNewsID(), lst_Insert[i].strAgencyCode);
                                else
                                    strMessage = String.Format("{0} was insert into {1}", reader.GetXMLFileName(), lst_Insert[i].strAgencyCode);
                                // Edit 2023-07-07
                                SetTextToMain(strMessage);
                            }
                        }
                        if (bInsertToDefault)
                        {
                            nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType);
                            // Edit 2023-07-07
                            String strMessage = "";
                            if (!setting.strXmlType.Equals("Reuters"))
                                strMessage = String.Format("{0} was insert into {1}", reader.GetNewsID(), setting.strAgencyTableCode);
                            else
                                strMessage = String.Format("{0} was insert into {1}", reader.GetXMLFileName(), setting.strAgencyTableCode);
                            // Edit 2023-07-07
                            SetTextToMain(strMessage);
                        }
                        if (nRet)
                        {
                            // Edit 2023-07-07
//                            String strSql = "";
//                            if (!setting.strXmlType.Equals("Reuters"))
                              String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", str_TeleId, reader.GetNewsID(), 0);
//                            else
  //                              strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", str_TeleId, reader.GetXMLFileName(), 0);
                            // Edit 2023-07-07
                            DB.ExecuteSQL(strSql);
                            iTotalNewsCount++;
                            if (setting.bIsChannel && setting.bNeedCopyFile)
                                nRet = DownloadXMLFile(setting.strXMLDownloadPath.Trim().LastIndexOf("\\") == setting.strXMLDownloadPath.Trim().Length ?
                                    setting.strXMLDownloadPath + reader.GetXMLFileName() : setting.strXMLDownloadPath + "\\" + reader.GetXMLFileName(), strXMLContent);
                            if (!setting.bIsChannel && setting.strFileCopyPath.Length > 0)
                                //                        CopyXML(setting, reader.GetXMLFileName(), reader.GetNewsDateTime().ToLocalTime());
                                CopyXML(setting, reader.GetXMLFileName(), dtPhototime);
                            if (!setting.bIsChannel && setting.bNeedDeleteFile)
                                nRet = DeleteFiles(setting, reader.GetXMLFileName(), null, false);
                        }
                        else
                        {
                            // Edit 2023-07-07
                            if (!setting.strXmlType.Equals("Reuters"))
                                SetTextToMain(String.Format("{0} cannot insert to {1}", reader.GetNewsID(), setting.strAgencyTableCode));
                            else
                                SetTextToMain(String.Format("{0} cannot insert to {1}", reader.GetXMLFileName(), setting.strAgencyTableCode));
                        }
                        // Edit 2023-07-07
                        // Edit Stephen 2022-10-05
                    }
                    // Edit Stephen 2022-10-05
                }

                // Edit Stephen 2022-10-05
                if (setting.bIsAP == true || setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase) == false)
                // Edit Stephen 2022-10-05
                //else
                {
                    // Edit Stephen 2022-10-05
                    if (setting.bIsAP == true)
                    {
                        if (((AP_EMS6_XML_Reader)reader).GetXMLNewsType() != "PICTURE") continue;
                    }
                    // Edit Stephen 2022-10-05
                    //******* More log
//                   SetTextToMain("Start Insert Photo File");
                    //******* More log

                    if (reader==null)
                    {
                        // Edit 2023-07-07
                        String strMessage = "";
                        if (!setting.strXmlType.Equals("Reuters"))
                            strMessage = String.Format("Photo {0} processing error (reader==null)", reader.GetNewsID());
                        else
                            strMessage = String.Format("Photo {0} processing error (reader==null)", reader.GetXMLFileName());
                        // Edit 2023-07-07
                        SetTextToMain(strMessage);
                        continue;
                    }

                    PhotoInfo info = reader.GetPhotoInfo();
                    if (info == null)
                    {
                        // Edit 2023-07-07
                        String strMessage = "";
                        if (!setting.strXmlType.Equals("Reuters"))
                            strMessage = String.Format("Photo {0} processing error (info==null)", reader.GetNewsID());
                        else
                            strMessage = String.Format("Photo {0} processing error (info==null)", reader.GetXMLFileName());
                        // Edit 2023-07-07
                        SetTextToMain(strMessage);
                        continue;
                    }
                    // Cannot Get related Photo data, skip to process next round
                    if (info.strSourcePhotoName == null || info.strSourcePhotoName.Trim() == "")
                        continue;
                    // Cannot Get related Photo data, skip to process next round

                    if(Path.GetExtension(info.strSourcePhotoName).Equals(".eps", StringComparison.OrdinalIgnoreCase))
                        continue;
//**********************************************************************************************************************
                    //if (!DownloadPhoto(ref info, setting, reader.GetNewsDateTime().ToLocalTime()))
                    // Edit Stephen 2022-10-05
                    if (setting.bIsAP == false)
                    {
                        if (!DownloadPhoto(ref info, setting, dtPhototime))
                            continue;
                    }
                    else
                    {
                        //******* More log
//                        SetTextToMain("Start Make Small Photo");
                        //******* More log

                        if (!MakeSmallPhoto_AP(ref info, setting, dtPhototime))
                            continue;
                    }
                    // Edit Stephen 2022-10-05
                    //**************************************************************************************************
                    //******* More log
//                   SetTextToMain("End Processing Samll Photo File");
                    //******* More log
                    
                    strPhotoAddInfo = "";
                    if (main.isLoadAddInfo)
                        strPhotoAddInfo = GetPhotoAdditionalInfo(info.strPhotoSourcePath);

                    String strHeadline = "";
                    if (reader.GetStrHeadLine() == null)
                    {
                        //                    if (setting.bIsAP == false)
                        strHeadline = reader.GetStrHeadLine().Length > 250 ? reader.GetStrHeadLine().Substring(0, 250) : reader.GetStrHeadLine();
                        //                    else
                        //                        strHeadline = reader.GetPhotoInfo().strHeadLine;
                    }

                    if (strHeadline == null)
                        strHeadline ="";
                    if (strHeadline == "") 
                        strHeadline = reader.GetStrHeadLine();
                    if (strHeadline.Length > 250) 
                        strHeadline = strHeadline.Substring(0, 250);
//                    strHeadline = strHeadline.Length > 250 ? strHeadline.Substring(0, 250) : reader.GetStrHeadLine();
                    while (true)
                    {
                        if (strHeadline.Length <= 0)
                            break;
                        if (strHeadline[strHeadline.Length - 1] == '\'' && strHeadline.Length > 0)
                        {
                            strHeadline = strHeadline.Remove(strHeadline.Length - 1);
                        }
                        else
                            break;
                    }
                    //******* More log
//                   SetTextToMain("End Getting HeadLine");
                    //******* More log
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
//                               lst_Insert[i].strAgencyCode, strPhotoID, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                               lst_Insert[i].strAgencyCode, strPhotoID, str_Photo_time,
                               DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), strHeadline
//                               , reader.GetPhotoInfo().strSize, "jpg", "", "N", reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                               , reader.GetPhotoInfo().strSize, "jpg", "", "N", str_Photo_time,
                                /*DB.GetAgencyId(setting.strAgencyTableCode)*/DB.GetAgencyId(lst_Insert[i].strAgencyCode), "0", info.strPhotoSourcePath, info.strPhotoHeight.Length == 0 ? "0" : info.strPhotoHeight,
                               info.strPhotoWidth.Length == 0 ? "0" : info.strPhotoWidth, "24", info.strPhotoMiddlePath, info.strPhotoSmallPath, info.strPhotoSourcePath,
                               DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"),
                               reader.GetStrCaption() + "\r\n" + strPhotoAddInfo);
                            // Edit Stephen 2022-10-05
                            if (setting.bIsAP == true)
                                nRet = InsertRecordToDB(strValue, lst_Insert[i].strAgencyCode, "Photo");
                            else
                            // Edit Stephen 2022-10-05
                                nRet = InsertRecordToDB(strValue, lst_Insert[i].strAgencyCode, setting.strAgencyType);
                            if (nRet)
                            {
                                // Edit 2023-07-07
//                                String strSql = "";
//                                if (!setting.strXmlType.Equals("Reuters"))
                                  String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetNewsID().Replace("__", ":"), 1);
//                                else
//                                    strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetXMLFileName().Replace("__", ":"), 1);
                                // Edit 2023-07-07
                                DB.ExecuteSQL(strSql);
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
                            "{10}, {11},N'{12}', {13},{14}, {15},N'{16}', N'{17}',N'{18}', N'{19}',N'{20}',N'{21}')",
//                        setting.strAgencyTableCode, strPhotoID, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                        setting.strAgencyTableCode, strPhotoID, str_Photo_time,
                        DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), strHeadline
//                        , reader.GetPhotoInfo().strSize, "jpg", "", "N", reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"),
                        , reader.GetPhotoInfo().strSize, "jpg", "", "N", str_Photo_time,
                        DB.GetAgencyId(setting.strAgencyTableCode), "0", info.strPhotoSourcePath, info.strPhotoHeight.Length == 0 ? "0" : info.strPhotoHeight,
                        info.strPhotoWidth.Length == 0 ? "0" : info.strPhotoWidth, "24", info.strPhotoMiddlePath, info.strPhotoSmallPath, info.strPhotoSourcePath,
                        DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"),
                        reader.GetStrCaption() + "\r\n" + strPhotoAddInfo);
                        // Edit Stephen 2022-10-05
                        if (setting.bIsAP == true)
                            nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, "Photo");
                        else
                         // Edit Stephen 2022-10-05
                            nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType);

                        if (nRet)
                        {
                            // Edit 2023-07-07
//                            String strSql = "";
//                            if (!setting.strXmlType.Equals("Reuters"))
                              String strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetNewsID().Replace("__", ":"), 1);
//                            else    
//                                strSql = String.Format("insert into wires_reuters_record values('{0}','{1}',{2})", strPhotoID, reader.GetXMLFileName().Replace("__", ":"), 1);
                            DB.ExecuteSQL(strSql);
                        }
                    }
                    if (nRet)
                    {
                        String strMessage = String.Format("Photo {0} was prcessed and inserted into Database", info.strSourcePhotoName);
                        SetTextToMain(strMessage);
                        iTotalNewsCount++;
                        try
                        {
                            //******* More log
//                           SetTextToMain("Start making caption");
                            //******* More log
                            String strPath = "";
                            if (setting.strPhotoSourcePath_Format.Trim() == "")
                            {
                                strPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                                setting.strChannelsName.Trim() + info.strSourcePhotoName : setting.strChannelsName.Trim() + "\\";
                            }
                            else
                            {
                                //                            String strPath = setting.strPhotoSourcePath_Format.Replace("%Y", reader.GetNewsDateTime().ToLocalTime().Year.ToString());
                                //                            strPath = strPath.Replace("%M", reader.GetNewsDateTime().ToLocalTime().ToString("MM"));
                                //                            strPath = strPath.Replace("%D", reader.GetNewsDateTime().ToLocalTime().ToString("dd"));
                                strPath = setting.strPhotoSourcePath_Format.Replace("%Y", dtPhototime.Year.ToString());
                                strPath = strPath.Replace("%M", dtPhototime.ToString("MM"));
                                strPath = strPath.Replace("%D", dtPhototime.ToString("dd"));
                            }                            //**************************************************************************************************

                            strPath = strPath.Trim().LastIndexOf("\\") == strPath.Trim().Length ? strPath.Trim() : strPath.Trim() + "\\";
                            //String strTxtFileName = reader.GetXMLFileName().Substring(0, reader.GetXMLFileName().LastIndexOf(".")) + ".txt";// Picture Caption
                            String strTxtFileName = reader.GetPhotoInfo().strSourcePhotoName.Substring(0, reader.GetPhotoInfo().strSourcePhotoName.LastIndexOf(".")) + ".txt";// Picture Caption
                            File.WriteAllText(strPath + strTxtFileName, reader.GetStrCaption().Trim() + "\r\n" + strPhotoAddInfo);
                            //******* More log
//                           SetTextToMain("End making caption");
                            //******* More log
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
    //                        CopyXML(setting, reader.GetXMLFileName(), reader.GetNewsDateTime().ToLocalTime());
                            CopyXML(setting, reader.GetXMLFileName(), dtPhototime);
                        //******* More log
 //                      SetTextToMain("Start Delete Files");
                        //******* More log
                        if (!setting.bIsChannel && setting.bNeedDeleteFile)
                            nRet = DeleteFiles(setting, reader.GetXMLFileName(), info, true);
                        //******* More log
 //                      SetTextToMain("End Delete Files");
                        //******* More log
                    }
                    else
                    {
                        String strMessage = String.Format("Photo {0} cannot insert into Database", info.strSourcePhotoName);
                        SetTextToMain(strMessage);
                    }
                }
                //******* More log
//               SetTextToMain(String.Format("End Process on {0}\n ", lstNewList[j].id.ToString()));
                //******* More log
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
        private bool DeleteFiles(SettingParameter setting, String strXMLFileName,PhotoInfo info, bool b_AP_Photo)
        {
            try
            {
                if (setting.bNeedDeleteFile && setting.strAgencyType.Equals("text", StringComparison.OrdinalIgnoreCase))
                {
                    // Edit Stephen 2022-10-12
                    if (setting.bIsAP)
                    {
                        // delete source photo
                        if (b_AP_Photo)
                        {
                            if (info.strPhotoSourcePath != null && info.strSourcePath != null)
                            {
                                if (info.strPhotoSourcePath.Trim() != "" && info.strSourcePath != info.strPhotoSourcePath)
                                {
                                    if (File.Exists(info.strPhotoSourcePath))
                                    {
                                        if (File.Exists(info.strSourcePath))
                                            File.Delete(info.strSourcePath);
                                    }
                                }
                            }
                        }
                    }
                    // Edit Stephen 2022-10-12
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
                    //*************************************************************************
                    // Edit Stephen 2018-05-17
                    //*************************************************************************
                    // String strMessage = String.Format("Now start process {0}", lst_Setting[i].strChannelsName);
                    String strMessage = String.Format("Now start process {0}", lst_Setting[i].strChannelsName_Format);
                    strMessage = strMessage.Replace("%Y", DateTime.Now.Year.ToString());
                    strMessage = strMessage.Replace("%M", DateTime.Now.ToString("MM"));
                    strMessage = strMessage.Replace("%D", DateTime.Now.ToString("dd"));
                    //*************************************************************************
                    SetTextToMain(strMessage);
                    Start(lst_Setting[i]);
                }
                strError = "";
                SetTextToMain("Finish.");
                Thread.Sleep(iInterval);
            }
        }

        private bool MakeSmallPhoto_AP(ref PhotoInfo info, SettingParameter setting, DateTime dt_NewsDate)
        {
            try
            {

                info.strPhotoSourcePath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                setting.strChannelsName.Trim() + info.strSourcePhotoName : setting.strChannelsName.Trim() + "\\" + info.strSourcePhotoName;

                info.strSourcePath = info.strPhotoSourcePath;

                String strDestPath_Source = setting.strPhotoSourcePath_Format.Replace("%Y", dt_NewsDate.Year.ToString());
                strDestPath_Source = strDestPath_Source.Replace("%M", dt_NewsDate.ToString("MM"));
                strDestPath_Source = strDestPath_Source.Replace("%D", dt_NewsDate.ToString("dd"));
                String strDestPath_Middle = setting.strPhotoMiddlePath_Format.Replace("%Y", dt_NewsDate.Year.ToString());
                strDestPath_Middle = strDestPath_Middle.Replace("%M", dt_NewsDate.ToString("MM"));
                strDestPath_Middle = strDestPath_Middle.Replace("%D", dt_NewsDate.ToString("dd"));
                String strDestPath_Small = setting.strPhotoSmallPath_Format.Replace("%Y", dt_NewsDate.Year.ToString());
                strDestPath_Small = strDestPath_Small.Replace("%M", dt_NewsDate.ToString("MM"));
                strDestPath_Small = strDestPath_Small.Replace("%D", dt_NewsDate.ToString("dd"));

                String strDestSourcePath = "";
                String strDestMiddlePath = "";
                String strDestSmallPath = "";

                if (strDestPath_Source.Trim() != "")
                {
                    if (!Directory.Exists(strDestPath_Source))
                        Directory.CreateDirectory(strDestPath_Source);
                    strDestSourcePath = strDestPath_Source.Trim().LastIndexOf("\\") == strDestPath_Source.Trim().Length ?
                        strDestPath_Source.Trim() + info.strSourcePhotoName : strDestPath_Source.Trim() + "\\" + info.strSourcePhotoName;
                }
                if (strDestPath_Middle.Trim() != "")
                {
                    if (!Directory.Exists(strDestPath_Middle))
                        Directory.CreateDirectory(strDestPath_Middle);
                    strDestMiddlePath = strDestPath_Middle.Trim().LastIndexOf("\\") == strDestPath_Middle.Trim().Length ?
                        strDestPath_Middle.Trim() + info.strMiddlePhotoName : strDestPath_Middle.Trim() + "\\" + info.strMiddlePhotoName;
                }
                if (strDestPath_Small.Trim() != "")
                {
                    if (!Directory.Exists(strDestPath_Small))
                        Directory.CreateDirectory(strDestPath_Small);
                    strDestSmallPath = strDestPath_Small.Trim().LastIndexOf("\\") == strDestPath_Small.Trim().Length ?
                        strDestPath_Small.Trim() + info.strSmallPhotoName : strDestPath_Small.Trim() + "\\" + info.strSmallPhotoName;
                }

                List<String> lst_DestPhotoName = new List<String>();
                lst_DestPhotoName.Add(strDestSourcePath);
                lst_DestPhotoName.Add(strDestMiddlePath);
                lst_DestPhotoName.Add(strDestSmallPath);
                List<String> lst_PhotoLink = new List<String>();
                lst_PhotoLink.Add(info.strPhotoSourcePath);
                lst_PhotoLink.Add(info.strPhotoMiddlePath);
                lst_PhotoLink.Add(info.strPhotoSmallPath);

                if (strDestSourcePath.Trim() != "")
                {
                    if (info.strPhotoSourcePath != strDestSourcePath)
                    {
                        FileInfo fileInfo = new FileInfo(info.strPhotoSourcePath);
                        if (fileInfo.Exists)
                        {
                            fileInfo.CopyTo(strDestSourcePath, true);
                            FileInfo destInfo = new FileInfo(strDestSourcePath);
                            if (destInfo.Exists)
                                info.strPhotoSourcePath = strDestSourcePath;
                            else
                            {
                                SetTextToMain(String.Format("Error ocurrs in processing picture : {0}", info.strPhotoSourcePath));
                                return false;
                            }
                        }
                        else
                        {
                            SetTextToMain(String.Format("Error ocurrs in load picture : {0}", info.strPhotoSourcePath));
                            return false;
                        }
                    }
                }
                info.strPhotoMiddlePath = strDestMiddlePath;
                if (strDestSmallPath.Trim() != "")
                    info.strPhotoSmallPath = strDestSmallPath;
                else
                {
//                    info.strPhotoSmallPath = info.strSourcePath;
                    int iTempPos = info.strPhotoSourcePath.LastIndexOf("\\");
                    if (iTempPos > 0)
                        info.strPhotoSmallPath = info.strPhotoSourcePath.Substring(0, iTempPos + 1) + info.strSmallPhotoName;
                    else
                        info.strPhotoSmallPath = "";
                }
                try
                {
                    //LEADLib.LEAD lead = new LEADLib.LEAD();
                    if (m_lead==null) m_lead = new LEADLib.LEAD();

                    m_lead.Load(info.strPhotoSourcePath, 0, 0, 1);
                    info.strHeadLine = "";
                    try
                    {
                        // 184 = IptcObjectName
                        try
                        {
                            info.strHeadLine = m_lead.ReadComment(info.strPhotoSourcePath, 0, 184);
                        }
                        catch (Exception)
                        {
                        }
                        info.strHeadLine = info.strHeadLine.Replace("'", "''");
                    }
                    catch (Exception)
                    {
                    }

                    float bitH = m_lead.BitmapHeight;
                    float bitW = m_lead.BitmapWidth;
                    info.strPhotoHeight = bitH.ToString();
                    info.strPhotoWidth = bitW.ToString();
                    info.strSize = (bitH * bitW).ToString();

                     float height, width = 0;
                     if (info.strPhotoSmallPath.Trim() != "")
                    {
                        height = float.Parse(DB.GetConfig("photo_height"));
                        width = float.Parse(DB.GetConfig("photo_width"));
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
                        m_lead.Size(width, height, 2);
                        m_lead.Save(info.strPhotoSmallPath, 10, 24, 10, 0);
                    }
                     if (info.strPhotoMiddlePath.Trim() != "")
                    {
                        height = float.Parse(DB.GetConfig("photo_heightM"));
                        width = float.Parse(DB.GetConfig("photo_widthM"));
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
                        m_lead.Size(width, height, 2);
                        m_lead.Save(info.strPhotoMiddlePath, 10, 24, 10, 0);
                    }
                }
                catch (Exception ex)
                {
                    SetTextToMain(String.Format("Failed to process small or middle picture from picture {0} reason by: {1}", info.strSourcePhotoName, ex.Message.Trim()));
                    return false;
                }
            }
            catch (Exception ex)
            {
                SetTextToMain(String.Format("Error ocurrs in picture processing " + ex.Message));
                return false;
            }
            return true;
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

                String strDestSourcePath = strDestPath_Source.Trim().LastIndexOf("\\") == strDestPath_Source.Trim().Length ?
                    strDestPath_Source.Trim() + info.strSourcePhotoName : strDestPath_Source.Trim() + "\\" + info.strSourcePhotoName;
                String strDestMiddlePath = strDestPath_Middle.Trim().LastIndexOf("\\") == strDestPath_Middle.Trim().Length ?
                    strDestPath_Middle.Trim() + info.strMiddlePhotoName : strDestPath_Middle.Trim() + "\\" + info.strMiddlePhotoName;
                String strDestSmallPath = strDestPath_Small.Trim().LastIndexOf("\\") == strDestPath_Small.Trim().Length ?
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
                    if (info.strSourcePhotoName != info.strSmallPhotoName)
                        strSourceSmallPhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                            setting.strChannelsName.Trim() + info.strSmallPhotoName : setting.strChannelsName.Trim() + "\\" + info.strSmallPhotoName;
                    else
                        strSourceSmallPhotoPath = strDestSmallPath;
                    if (info.strSourcePhotoName != info.strMiddlePhotoName)
                        strSourceMiddlePhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                            setting.strChannelsName.Trim() + info.strMiddlePhotoName : setting.strChannelsName.Trim() + "\\" + info.strMiddlePhotoName;
                    else
                        strSourceMiddlePhotoPath = strDestMiddlePath;
                    strSourceBasePhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                        setting.strChannelsName.Trim() + info.strSourcePhotoName : setting.strChannelsName.Trim() + "\\" + info.strSourcePhotoName;
                    info.strPhotoSourcePath = strDestSourcePath;
                    info.strPhotoMiddlePath = strDestMiddlePath;
                    info.strPhotoSmallPath = strDestSmallPath;
                    try
                    {                        //FileInfo fileInfo = new FileInfo(strSourceBasePhotoPath);
                        //if (fileInfo.Exists)
                        //{
                        //    if (strSourceBasePhotoPath != strDestSourcePath)
                        //        fileInfo.CopyTo(strDestSourcePath, true);
                        //}
                        //else { return false; }
                        FileInfo fileInfo;
                        int i = 0;
                        for (; i < settingHandler.GetSourcePhotoSizeType().Count; i++)
                        {
                            int j = 0;
                            for (; j < info.lstPhoto.Count; j++)
                            {
                                if (settingHandler.GetSourcePhotoSizeType()[i] == info.lstPhoto[j].strPhotoSizeType)
                                {
                                    strSourceBasePhotoPath = setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Trim().Length ?
                                        setting.strChannelsName.Trim() + info.lstPhoto[j].strPhotoName : setting.strChannelsName.Trim() + "\\" + info.lstPhoto[j].strPhotoName;
                                    strDestSourcePath = strDestPath_Source.Trim().LastIndexOf("\\") == strDestPath_Source.Trim().Length ?
                                        strDestPath_Source.Trim() + info.lstPhoto[j].strPhotoName : strDestPath_Source.Trim() + "\\" + info.lstPhoto[j].strPhotoName;

                                    fileInfo = new FileInfo(strSourceBasePhotoPath);
                                    if (fileInfo.Exists)
                                    {
                                        if (strSourceBasePhotoPath != strDestSourcePath)
                                        {
                                            fileInfo.CopyTo(strDestSourcePath, true);
                                        }
                                        info.strPhotoSourcePath = strDestSourcePath;
                                        break;
                                    }
                                }
                            }
                            if (j != info.lstPhoto.Count)
                                break;
                        }
                        if (i == settingHandler.GetSourcePhotoSizeType().Count)
                            return false;
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
                                //LEADLib.LEAD lead = new LEADLib.LEAD();
                                if (m_lead == null) m_lead = new LEADLib.LEAD();
                                m_lead.Load(strSourceBasePhotoPath, 0, 0, 1);
                                float bitH = m_lead.BitmapHeight;
                                float bitW = m_lead.BitmapWidth;
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
                                m_lead.Size(width, height, 2);

                                m_lead.Save(strDestMiddlePath, 10, 24, 10, 0);
 
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
                                //LEADLib.LEAD lead = new LEADLib.LEAD();
                                if (m_lead == null) m_lead = new LEADLib.LEAD();
                                m_lead.Load(strSourceBasePhotoPath, 0, 0, 1);
                                float bitH = m_lead.BitmapHeight;
                                float bitW = m_lead.BitmapWidth;
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
                                m_lead.Size(width, height, 2);
                                m_lead.Save(strDestSmallPath, 10, 24, 10, 0);
                            }
                            catch (Exception ex)
                            {
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
                //LEADLib.LEAD lead = new LEADLib.LEAD();
                if (m_lead == null) m_lead = new LEADLib.LEAD();
                m_lead.Load(strProcessFilePath, 0, 0, 1);
                float bitH = m_lead.BitmapHeight;
                float bitW = m_lead.BitmapWidth;
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
                m_lead.Size(width, height, 2);
                m_lead.Save(strDestPath, 10, 24, 10, 0);
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

        private bool InsertRecordToDB(String strValue, String strTableName, String strTableType)
        {
            bool nRet = true;
            String strSQL = "";
            if (strTableType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                strSQL = String.Format("Insert into {0} ( tele_name , tele_time , tele_words , tele_content,tele_title, tele_location , tele_keyword,tele_urgent,tele_flag,tele_note , tele_language) values {1}",
                    strTableName, strValue);
            else
                strSQL =  String.Format("Insert into wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"+
                    "photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,"+
                    "photo_src_path,last_modify_time,real_time, photo_caption) values {0}", strValue);
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
//                main.SetTextOnRichTextBox("Path Setting Error: "+ex.Message.Trim()+" ");
                SetTextToMain("Path Setting Error: " + ex.Message.Trim() + " ");
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
                //LEADLib.LEAD lead = new LEADLib.LEAD();
                if (m_lead == null) m_lead = new LEADLib.LEAD();
                m_lead.Load(strPhotoPath, 0, 0, 1);
                for (int i = 0; i < lst_CommentCons.Count; i++)
                {
                    strTemp = m_lead.ReadComment(strPhotoPath, 0, lst_CommentCons[i]);
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
    }
}

public class NewsItem
{
    public String id;
    public DateTime dtNewsTime;
    public int iUrgentPriority;
    public int iWordCount;
    public String strFilePhyName;
}

public class NewsChannel
{
    public String strAlias;
    public String strDescrpition;
    public String strCateId;
    public String strField;
    public DateTime dtLastUpdateTime;
}


public class FaultXMLList
{
    [XmlElement("strNewId")]
    public String strNewId;
    [XmlElement("iRetryTime")]
    public int iRetryTime;
    [XmlElement("dtLastProcTime")]
    public DateTime dtLastProcTime;
    [XmlElement("strAlias")]
    public String strAlias;
    [XmlElement("strAgencyCode")]
    public String strAgencyCode;
}