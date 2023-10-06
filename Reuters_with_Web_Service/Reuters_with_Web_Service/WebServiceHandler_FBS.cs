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
/*
 * This class for handle data insert on "Metro-FBS" Version
*/
namespace Reuters_with_Web_Service
{
    public class WebServiceHandler_FBS : WebServiceHandler
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
        int iSequence;
        char cAlphabet;
        SettingHandler settingHandler;

        public WebServiceHandler_FBS(String strUserName, String strPassword)
        {
            this.strUserName = strUserName;
            this.strPassword = strPassword;
            iSequence = 0;
            cAlphabet = 'A';
            settingHandler = new SettingHandler();
            this.strUserName = "metro_hk";
            this.strPassword = "RtrsM3tro";
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
                res = service.getItems(null, strMaxAge, new String[] { channel.strAlias }, null, 3000, true, new String[] { channel.strCateId }, "default", 1000, true, false, true, null);
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

        public List<String> GetDBDataIntoList(String strNewsType, String strAgencyCode, bool bIsAP)
        {
            //process picture later
            DataTable table = new DataTable();
            String strSQL= "";

            // Edit Stephen 2022-10-10
            if (bIsAP)
            {
                strSQL = String.Format("select replace(Reuter_News_ID,'__',':') from Wires_Reuters_Record a ");
                strSQL = strSQL + " inner join dbo.WireFiles b on a.FileID = b.FileID ";
                strSQL = strSQL + " and b.Date >= DateAdd(month, -1, GetDate())";
            }
            else
                strSQL = String.Format("select replace(Reuter_News_ID,'__',':') from Wires_Reuters_Record");
            // Edit Stephen 2022-10-10
    
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
            GetDBDataIntoList(setting.strAgencyType, setting.strAgencyTableCode, setting.bIsAP);
            int iTotalNewsCount = 0;
            if (setting.strWireID.Length == 0 || (setting.strFileCode.Length == 0 && !setting.bIsGIS))
            {
                SetTextToMain("WiresID and File Code must be insert. All record refer to this setting will be skipped.");
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
                if (!Directory.Exists(setting.strChannelsName))
                    Directory.CreateDirectory(setting.strChannelsName);

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
            List<String> lst_DBRecord = GetDBDataIntoList(setting.strAgencyType, setting.strAgencyTableCode, setting.bIsAP);
            DateTime dt_ProcessTime = DateTime.Now;
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
                Reader reader;
                if (setting.bIsChannel)
                {
                    try
                    {
                        strXMLContent = service.getItem(lstNewList[j].id.ToString(), setting.strAliasCode, null, null);
                    }
                    catch (Exception ex)
                    {
//                        main.SetTextOnRichTextBox(String.Format("Process on {0} catch the following error:\n {1} ", lstNewList[j].id.ToString(), ex.Message.Trim()));
                        SetTextToMain(String.Format("Process on {0} catch the following error:\n {1} ", lstNewList[j].id.ToString(), ex.Message.Trim()));
                        continue;
                    }
                    reader = new XML_Reader(strXMLContent, setting.bIsChannel);
                    reader.SetNewsID(lstNewList[j].id.ToString());
                }
                else
                {
                    if (setting.bIsAP == true)
                    {
                        // Edit Stephen 2022-08-01
                        //                        reader = new AP_XML_Reader(lstNewList[j].id);
                        reader = new AP_EMS6_XML_Reader(lstNewList[j].id);
                        ((AP_EMS6_XML_Reader)reader).SetDoubleLineFeed();
                        //((AP_XML_Reader)reader).SetProcessTime(dt_ProcessTime);
                        // Edit Stephen 2022-08-01
                    
                    }
                    else
                    {
                        if (setting.bIsGIS == true)
                            reader = new GIS_XML_Reader(lstNewList[j].id);
                        else
                            reader = new XML_Reader(lstNewList[j].id, setting.bIsChannel);
                    }
                }
                bool bRet = reader.Read();

                // Edit Stephen 2022-08-03
                if (reader.GetErrorMessage() != "")
                {
                    if (setting.bIsAP == true && reader.GetErrorMessage() == "Read News Content Fail")
                        continue;
                    if (setting.bIsAP == true && reader.GetStrContent() == "")
                        continue;
                }
                // Edit Stephen 2022-08-03

                if (bRet == false)
                {
                    if (setting.bIsAP == true)
                    {
//                        if (((AP_XML_Reader)reader).GetErrorMessage() != "ERROR")
//                            DeleteFiles(setting, reader.GetXMLFileName(), null, false);
                    }
                    if (setting.bIsGIS)
                    {
                        SetTextToMain(((GIS_XML_Reader)reader).GetErrorMessage());
                    }
                    continue;
                }
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
                if (setting.bIsAP == true)
                {
//                    iTotalNewsCount += HandleAPNews(((AP_XML_Reader)reader), setting, dt_ProcessTime);
                    iTotalNewsCount += HandleAPNews_EMS6(((AP_EMS6_XML_Reader)reader), setting, dt_ProcessTime);
                }
                else
                {
                    String strValue = "";
                    if (setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                    {
                        String strFileID;
                        String strResult = "1", strTemp;
                        DataTable table;
                        do
                        {
                            if (setting.bIsGIS)
                            {
                                strFileID = reader.GetNewsID();

                                if (setting.strFileCode.Trim() != "")
                                {
                                    if (strFileID.IndexOf(setting.strFileCode) < 0)
                                        strFileID = setting.strFileCode + strFileID;
                                }
                            }
                            else
                            {
                                GetSequence();
                                strFileID = setting.strFileCode + dt_ProcessTime.ToString("yyMMdd") + cAlphabet + iSequence.ToString("D4");
                            }
                            strTemp = String.Format("select count(*) from WireFiles where FileID = '{0}'", strFileID);
                            table = DB.GetRecordToDataTable(strTemp);
                            try
                            {
                                strResult = table.Rows[0].ItemArray[0].ToString();
                            }
                            catch (Exception ex)
                            { strResult = "1"; }
                        } while (strResult == "1");
                        String strHeadLine = reader.GetStrHeadLine();
                        strHeadLine = strHeadLine.Replace("''", "'");

                        if (!setting.bIsGIS)
                        {
                            byte[] bytes = Encoding.Unicode.GetBytes(strHeadLine);
                            bytes = Encoding.Convert(Encoding.Unicode, Encoding.ASCII, bytes);
                            strHeadLine = Encoding.ASCII.GetString(bytes);
                        }

                        strHeadLine = strHeadLine.Length > 80 ? strHeadLine.Substring(0, 80) : strHeadLine;
                        strHeadLine = strHeadLine.Replace("'", "''");

                        String strClassName = "";
                        if (setting.bIsGIS)
                        {
                            strClassName = ((GIS_XML_Reader)reader).GetClass();
                        }
                        else
                            strClassName = GetClassName(reader, setting.lst_FileSpec);
    
                        if (strClassName.Length == 0)
                            strClassName = "Normal";
                        int iPriority = reader.GetPriority();
                        try
                        {
                            Priority priority = setting.lst_Priority.Find(delegate(Priority p) { return p.iTagValue == iPriority; });
                            if (priority != null)
                                iPriority = priority.iPriorityValue;
                            else
                                iPriority = 0;
                        }
                        catch (Exception ex)
                        {
                            iPriority = 0;
                        }
                        string strNewContent = "";

                        if (!setting.bIsGIS)
                        {
                            strNewContent = reader.GetStrContent().Replace("   \r\n", "_|_").Replace("\r\n", "\n").Replace("_|_", "   \r\n");
                            strNewContent = strHeadLine + "   \r\n" + strNewContent;
                        }
                        else
                            strNewContent = reader.GetStrContent();

                        strValue = String.Format("('{0}','{1}','{2}','{3}','{4}',{5},'{6}','{7}')",
                            strFileID, setting.strWireID, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd 00:00:00"),
                            reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"), strClassName, iPriority,
                            strHeadLine, strNewContent );

/*
                            strValue = String.Format("('{0}','{1}',{2},{3},'{4}',{5},'{6}','{7}')",
                            strFileID, setting.strWireID, "DATEADD(day, DATEDIFF(day, 0, GETDATE()), 0)",
                            "DATEADD(ms, -DATEPART(ms, GETDATE()), GETDATE())", strClassName, iPriority,
                            strHeadLine, strNewContent );
*/
                        bool nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType);
                        if (nRet)
                        {
                            String strSql = String.Format("insert into Wires_Reuters_Record values('{0}', '{1}')", strFileID, reader.GetNewsID());
                            DB.ExecuteSQL(strSql);
                            iTotalNewsCount++;
                            String strMessage = String.Format("{0} was insert into WiresFile", reader.GetNewsID());
                            SetTextToMain(strMessage);
                            if (setting.bIsChannel && setting.bNeedCopyFile)
                                nRet = DownloadXMLFile(setting.strXMLDownloadPath.Trim().LastIndexOf("\\") == setting.strXMLDownloadPath.Trim().Length ?
                                    setting.strXMLDownloadPath + reader.GetXMLFileName() : setting.strXMLDownloadPath + "\\" + reader.GetXMLFileName(), strXMLContent);
                            if (!setting.bIsChannel && setting.strFileCopyPath.Length > 0)
                            {
                                CopyXML(setting, reader.GetXMLFileName(), reader.GetNewsDateTime().ToLocalTime(), setting.bIsGIS);
                            }
                            if (!setting.bIsChannel && setting.bNeedDeleteFile)
                                nRet = DeleteFiles(setting, reader.GetXMLFileName(), null, setting.bIsGIS);
                        }
                        else
                            SetTextToMain(String.Format("{0} cannot insert to {1}", reader.GetNewsID(), setting.strAgencyTableCode));
                    }
                }
            }
            SetTextToMain(String.Format("Total news processed on {0} : {1}", setting.strChannelsName, iTotalNewsCount.ToString()));
            strError = "";
        }

        private String GetClassName(Reader reader, List<FileSpec> lst_Spec)
        {
            String strClassName = "";
            List<FileSpec> lst_Temp = new List<FileSpec>();
            List<String> lst_Subject = reader.GetSubjectList();
            // search the value on File Spec list where the settled File Spec are same as subject list value
            var matches = from subject in lst_Subject
                          join spec in lst_Spec on subject equals spec.strFileSpecName
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
            }
            if (lst_Temp.Count > 1)
               lst_Temp.RemoveRange(1, lst_Temp.Count - 1);
            if (lst_Temp.Count > 0)//only one Class Name will be used and that is the first one on the list.
                strClassName = lst_Temp[0].strAgencyCode;
            return strClassName;
        }

        private String GetClassNameForAP(List<String> lst_SubjectAP, List<FileSpec> lst_Spec)
        {
            String strClassName = "";
            List<FileSpec> lst_Temp = new List<FileSpec>();
            List<String> lst_Subject = lst_SubjectAP;
            // search the value on File Spec list where the settled File Spec are same as subject list value
            var matches = from subject in lst_Subject
                          join spec in lst_Spec on subject equals spec.strFileSpecName
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
            }
            if (lst_Temp.Count > 1)
                lst_Temp.RemoveRange(1, lst_Temp.Count - 1);
            if (lst_Temp.Count > 0)//only one Class Name will be used and that is the first one on the list.
                strClassName = lst_Temp[0].strAgencyCode;
            return strClassName;
        }

        private bool CopyXML(SettingParameter setting, String strXMLFileName, DateTime dt_NewsTime, bool bCopyTextFile)//From a local path to another local path
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

            if (bCopyTextFile)
            {
                String strCopyTextPath = "";
                string strTextFileName = strXMLFileName.Replace(".xml", ".txt");
                String strSourceTextPath = setting.strChannelsName.Trim().EndsWith("\\") ? setting.strChannelsName + "txt\\" + strTextFileName : setting.strChannelsName + "\\txt\\" + strTextFileName;
                try
                {

                    if (strCopyPath.EndsWith("\\"))
                        strCopyTextPath = strCopyPath + "txt\\";
                    else
                        strCopyTextPath = strCopyPath + "\\txt\\";
                    if (!Directory.Exists(strCopyTextPath))
                        Directory.CreateDirectory(strCopyTextPath);
                    String strDestTextPath = strCopyTextPath + strTextFileName;
                    File.Copy(strSourceTextPath, strDestTextPath, true);
                }
                catch (Exception ex)
                {
                    SetTextToMain(String.Format("Transfer {0} to {1} Failed. {2} : [{3}][{4}][{5}]",
                        strSourceTextPath, strCopyTextPath, ex.Message.Trim(), setting.strChannelsName.Trim(), setting.strFileCopyPath.Trim(), strTextFileName));
                    return false;
                }
            }             
            return true;
        }

        private bool DeleteFiles(SettingParameter setting, String strXMLFileName,PhotoInfo info,bool bDeleteTextFile )
        {
            try
            {
                if (File.Exists(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                    setting.strChannelsName + strXMLFileName : setting.strChannelsName + "\\" + strXMLFileName))
                    File.Delete(setting.strChannelsName.Trim().LastIndexOf("\\") == setting.strChannelsName.Length ?
                        setting.strChannelsName + strXMLFileName : setting.strChannelsName + "\\" + strXMLFileName);

                if (bDeleteTextFile)
                {
                    string strTextFileName = strXMLFileName.Replace(".xml", ".txt");
                    String strSourceTextPath = setting.strChannelsName.Trim().EndsWith("\\") ? setting.strChannelsName + "txt\\" + strTextFileName : setting.strChannelsName + "\\txt\\" + strTextFileName;

                    if (File.Exists(strSourceTextPath))
                        File.Delete(strSourceTextPath);
                
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
//                    String strMessage = String.Format("Now start process {0}", lst_Setting[i].strChannelsName);
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
      
        private bool InsertRecordToDB(String strValue, String strTableName, String strTableType)
        {
            bool nRet = true;
            String strSQL = "";
            if (strTableType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                strSQL = String.Format("Insert into WireFiles( FileID , WireID , Date , Time , Class , Priority , Abstract, Content) values {0}",
                     strValue);
            nRet = DB.ExecuteSQL(strSQL);
            return nRet;
        }

        private int HandleAPNews(AP_XML_Reader reader, SettingParameter setting, DateTime dt_ProcessTime)
        {
            int iTotalNewsCount = 0;
            List<AP_News> lst_APNews = reader.GetAPNews();
            foreach (AP_News ap in lst_APNews)
            {
                String strValue = "";
                if (setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    String strFileID;
                    String strResult = "1", strSql;
                    DataTable table;
                    do
                    {
                        GetSequence();
                        strFileID = setting.strFileCode + dt_ProcessTime.ToString("yyMMdd") + cAlphabet + iSequence.ToString("D4");
                        strSql = String.Format("select count(*) from WireFiles where FileID = '{0}'", strFileID);
                        table = DB.GetRecordToDataTable(strSql);
                        try
                        {
                            strResult = table.Rows[0].ItemArray[0].ToString();
                        }
                        catch (Exception ex)
                        { strResult = "1"; }
                    } while (strResult == "1");
                    String strHeadLine = ap.strNewsHeadLine;
                    strHeadLine = strHeadLine.Replace("''", "'");
                    byte[] bytes = Encoding.Unicode.GetBytes(strHeadLine);
                    bytes = Encoding.Convert(Encoding.Unicode, Encoding.ASCII, bytes);
                    strHeadLine = Encoding.ASCII.GetString(bytes);
                    strHeadLine = strHeadLine.Length > 80 ? strHeadLine.Substring(0, 80) : strHeadLine;
                    strHeadLine = strHeadLine.Replace("'", "''");

                    String strClassName = GetClassNameForAP(ap.lst_Subject, setting.lst_FileSpec);
                    if (strClassName.Length == 0)
                        strClassName = "Normal";
                    int iPriority = ap.iPriority;
                    try
                    {
                        Priority priority = setting.lst_Priority.Find(delegate(Priority p) { return p.iTagValue == iPriority; });
                        if (priority != null)
                            iPriority = priority.iPriorityValue;
                        else
                            iPriority = 0;
                    }
                    catch (Exception ex)
                    {
                        iPriority = 0;
                    }

                    if (ap.strNewsContent == null)
                        ap.strNewsContent = "";

                    strValue = String.Format("('{0}','{1}','{2}','{3}','{4}',{5},'{6}','{7}')",
                        strFileID, setting.strWireID, ap.dt_NewsDate.ToLocalTime().ToString("yyyy-MM-dd 00:00:00"),
                        ap.dt_NewsDate.ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"), strClassName, iPriority,
                        strHeadLine, strHeadLine + "   \r\n" + ap.strNewsContent.Replace("   \r\n", "_|_").Replace("\r\n", "\n").Replace("_|_", "   \r\n"));
/*
                    strValue = String.Format("('{0}','{1}',{2},{3},'{4}',{5},'{6}','{7}')",
                        strFileID, setting.strWireID, "DATEADD(day, DATEDIFF(day, 0, GETDATE()), 0)",
                        "DATEADD(ms, -DATEPART(ms, GETDATE()), GETDATE())", strClassName, iPriority,
                        strHeadLine, strHeadLine + "   \r\n" + ap.strNewsContent.Replace("   \r\n", "_|_").Replace("\r\n", "\n").Replace("_|_", "   \r\n"));
*/
                    bool nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType);
                    if (nRet)
                    {
                        //String strSql = String.Format("insert into Wires_Reuters_Record values('{0}', '{1}')", strFileID, ap.strNewsID);
                        //DB.ExecuteSQL(strSql);
                        iTotalNewsCount++;
                        String strMessage = String.Format("{0} was insert into WiresFile", ap.strNewsID);
                        SetTextToMain(strMessage);
                    }
                        
                }
            }
            if (!setting.bIsChannel && setting.strFileCopyPath.Length > 0)
                CopyXML(setting, reader.GetXMLFileName(), reader.GetNewsDateTime().ToLocalTime(), false);
            if (!setting.bIsChannel && setting.bNeedDeleteFile)
                DeleteFiles(setting, reader.GetXMLFileName(), null, false);
            return iTotalNewsCount;
        }

        private void GetSequence()
        {
            try
            {
                String strSequence = settingHandler.GetKeyValue("News_sequence");
                String strSequenceAlphabet = settingHandler.GetKeyValue("News_sequence_Alphabet");
                String strDate = settingHandler.GetKeyValue("Sequence_reset_date");
                if (strDate.Length == 0)
                {
                    cAlphabet = 'A';
                    iSequence = 0;
                    iSequence++;
                    settingHandler.UpdateKeyValue(DateTime.Today.ToString(), "Sequence_reset_date");
                    settingHandler.UpdateKeyValue(iSequence.ToString(), "News_sequence");
                    settingHandler.UpdateKeyValue(cAlphabet.ToString(), "News_sequence_Alphabet");
                }
                else
                {
                    DateTime strResetDate = DateTime.Parse(strDate);
                    iSequence = int.Parse(strSequence);
                    if (strResetDate < DateTime.Today)
                    {
                        cAlphabet = 'A';
                        iSequence = 0;
                        iSequence++;
                        settingHandler.UpdateKeyValue(DateTime.Today.ToString(), "Sequence_reset_date");
                        settingHandler.UpdateKeyValue(iSequence.ToString(), "News_sequence");
                        settingHandler.UpdateKeyValue(cAlphabet.ToString(), "News_sequence_Alphabet");
                    }
                    else if (iSequence >= 9999)
                    {
                        cAlphabet++;
                        iSequence = 0;
                        iSequence++;
                        settingHandler.UpdateKeyValue(iSequence.ToString(), "News_sequence");
                        settingHandler.UpdateKeyValue(cAlphabet.ToString(), "News_sequence_Alphabet");
                    }
                    else
                    {
                        iSequence++;
                        settingHandler.UpdateKeyValue(iSequence.ToString(), "News_sequence");
                    }
                }
            }catch(Exception ex)
            {
                SetTextToMain("Configuration Date Error: "+ex.Message.Trim());
                settingHandler.UpdateKeyValue(DateTime.Today.ToString(), "Sequence_reset_date");
            }
        }

        private void UpdateSettingPath(SettingParameter setting)
        {
            setting.strChannelsName = setting.strChannelsName_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strChannelsName = setting.strChannelsName.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strChannelsName = setting.strChannelsName.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strFileCopyPath = setting.strFileCopyPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strFileCopyPath = setting.strFileCopyPath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strFileCopyPath = setting.strFileCopyPath.Replace("%D", DateTime.Now.ToString("dd"));

            setting.strXMLDownloadPath = setting.strXMLDownloadPath_Format.Replace("%Y", DateTime.Now.Year.ToString());
            setting.strXMLDownloadPath = setting.strFileCopyPath.Replace("%M", DateTime.Now.ToString("MM"));
            setting.strXMLDownloadPath = setting.strFileCopyPath.Replace("%D", DateTime.Now.ToString("dd"));


        }

        //******************************************************
        // Edit Stephen 2022-10-06
        //******************************************************
        private int HandleAPNews_EMS6(AP_EMS6_XML_Reader reader, SettingParameter setting, DateTime dt_ProcessTime)
        {
            int iTotalNewsCount = 0;
            String strValue = "";

            if (reader.GetStrContent() != "No Content")
            {
                if (setting.strAgencyType.Equals("Text", StringComparison.OrdinalIgnoreCase))
                {
                    String strFileID;
                    String strResult = "1", strSql;
                    DataTable table;
                    do
                    {
                        GetSequence();
                        strFileID = setting.strFileCode + dt_ProcessTime.ToString("yyMMdd") + cAlphabet + iSequence.ToString("D4");
                        strSql = String.Format("select count(*) from WireFiles where FileID = '{0}'", strFileID);
                        table = DB.GetRecordToDataTable(strSql);
                        try
                        {
                            strResult = table.Rows[0].ItemArray[0].ToString();
                        }
                        catch (Exception ex)
                        { strResult = "1"; }
                    } while (strResult == "1");
                    //                String strHeadLine = ap.strNewsHeadLine;
                    String strHeadLine = reader.GetStrHeadLine();
                    strHeadLine = strHeadLine.Replace("''", "'");
                    byte[] bytes = Encoding.Unicode.GetBytes(strHeadLine);
                    bytes = Encoding.Convert(Encoding.Unicode, Encoding.ASCII, bytes);
                    strHeadLine = Encoding.ASCII.GetString(bytes);
                    strHeadLine = strHeadLine.Length > 80 ? strHeadLine.Substring(0, 80) : strHeadLine;
                    strHeadLine = strHeadLine.Replace("'", "''");

                    String strClassName = GetClassNameForAP(reader.GetSubjectList(), setting.lst_FileSpec); ;
                    // String strClassName = "Normal";
                    if (strClassName.Length == 0)
                        strClassName = "Normal";
                    int iPriority = reader.GetPriority();
                    //                int iPriority = 0;
                    try
                    {
                        Priority priority = setting.lst_Priority.Find(delegate(Priority p) { return p.iTagValue == iPriority; });
                        if (priority != null)
                            iPriority = priority.iPriorityValue;
                        else
                            iPriority = 0;
                    }
                    catch (Exception ex)
                    {
                        iPriority = 0;
                    }

                    //                if (ap.strNewsContent == null)
                    //                    ap.strNewsContent = "";

                    strValue = String.Format("('{0}','{1}','{2}','{3}','{4}',{5},'{6}','{7}')",
                        strFileID, setting.strWireID, reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd 00:00:00"),
                        reader.GetNewsDateTime().ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"), strClassName, iPriority,
                        strHeadLine, strHeadLine + "   \r\n\r\n" + reader.GetStrContent().Replace("   \r\n", "_|_").Replace("\r\n", "\n").Replace("_|_", "   \r\n"));
                    bool nRet = InsertRecordToDB(strValue, setting.strAgencyTableCode, setting.strAgencyType);
                    if (nRet)
                    {
                        //                    String strSql = String.Format("insert into Wires_Reuters_Record values('{0}', '{1}')", strFileID, reader.GetNewsID());
                        strSql = String.Format("insert into Wires_Reuters_Record values('{0}', '{1}')", strFileID, reader.GetNewsID());
                        DB.ExecuteSQL(strSql);
                        iTotalNewsCount++;
                        String strMessage = String.Format("{0} was insert into WiresFile", reader.GetNewsID());
                        SetTextToMain(strMessage);
                    }

                }
            }
            if (!setting.bIsChannel && setting.strFileCopyPath.Length > 0)
                CopyXML(setting, reader.GetXMLFileName(), reader.GetNewsDateTime().ToLocalTime(), false);
            if (!setting.bIsChannel && setting.bNeedDeleteFile)
                DeleteFiles(setting, reader.GetXMLFileName(), null, false);
            return iTotalNewsCount;
        }


    }
}