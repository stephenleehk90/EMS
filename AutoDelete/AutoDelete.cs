using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Timers;
using System.Data.SqlClient;
using System.IO;
using AutoDelete;
using System.Xml;
using System.Configuration;
using System.IO.Compression;
using System.IO.Packaging;

namespace MyNewService
{
    public partial class AutoDelete : ServiceBase
    {
        // private Timer _timer;
        private DateTime _lastRun = DateTime.Now.AddDays(-1);
        //DateTime nowDate = new DateTime();
        // private StreamWriter sw = null;
        string strDbConnStr;
        string strStartTime;    //everyday start time
        string strStartDate;    //every week start date
        string strRunDate = "";      //the date runned this service
        int strCurWeek;      //current week 
        int strWeekYear = 0;     //the week has called windows service for reset strRunDate weekly 
      //  int nInterval = 1000*60*60*2;  // default interval time 2 hour
        int nInterval = 1000 *5;  // default interval time 2 hour
        String strLogFilePath = "C:\\founder\\temp\\AutoDelete_Log\\";
        int iNoOfLogFile;
        double dFileSize;
        int iCurrentLogNo;
        String strComposeFileSubDir;
        String strComposeDateFormat;
        Timer serviceTimer;
        Boolean bTestingThread;
        List<String> lst_PhotoDelPath;
        List<String> lst_PhotoDelSubPath;
        String strErrorFileMovePath;

        public AutoDelete()
        {
            InitializeComponent();
           // this.AutoLog = false;
//               if (!System.Diagnostics.EventLog.SourceExists("MySource"))
//               {
//                System.Diagnostics.EventLog.CreateEventSource("MySource", "AutoDelete");
//               }
            System.Globalization.GregorianCalendar gc = new System.Globalization.GregorianCalendar();
            strWeekYear = gc.GetWeekOfYear(DateTime.Now, System.Globalization.CalendarWeekRule.FirstDay, DayOfWeek.Monday);
            lst_PhotoDelPath = new List<String>();
            lst_PhotoDelSubPath = new List<String>();
        }

        private void PrepareConnStr()
        {
            strDbConnStr = "";
            String strConnType = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["ConnectionType"]);
            String strDataSource = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["DataSource"]);
            String strInitialCatalog = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["InitialCatalog"]);
            String strSqlUserName = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["SqlUserName"]);
            String strSqlPassword = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["SqlPassword"]);
            
            if (strConnType.Equals("Windows", StringComparison.OrdinalIgnoreCase))
                strDbConnStr = "Data Source=" + strDataSource + ";Initial Catalog=" + strInitialCatalog + ";TRUSTED_CONNECTION=YES;";
            else
                strDbConnStr = "Data Source=" + strDataSource + ";Initial Catalog=" + strInitialCatalog + ";User ID=" + strSqlUserName + ";Password=" + strSqlPassword;
        }

        protected override void OnStart(string[] args)
        {
            try
            {
                iCurrentLogNo = 1;
                strStartTime = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["StartTime"]);
                //strDbConnStr = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["OpenDbConnStr"]);
                PrepareConnStr(); 
                strStartDate = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["StartDate"]);
                nInterval = Convert.ToInt32(System.Configuration.ConfigurationSettings.AppSettings["LOG_INTERVAL"]);
                if (nInterval == 0)
                {
                    nInterval = 5 * 1000;
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    config.AppSettings.Settings.Add("LOG_INTERVAL", (nInterval/1000).ToString());
                    config.Save(ConfigurationSaveMode.Full);
                    ConfigurationManager.RefreshSection("appSettings");
                    config.AppSettings.Settings["LOG_INTERVAL"].Value = (nInterval / 1000).ToString();
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                }
                else
                    nInterval = nInterval * 1000;
                iNoOfLogFile = Convert.ToInt32(System.Configuration.ConfigurationSettings.AppSettings["LOG_NO_ROTATE"]);
                if (iNoOfLogFile==0)
                { 
                    iNoOfLogFile = 5;
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    config.AppSettings.Settings.Add("LOG_NO_ROTATE", iNoOfLogFile.ToString());
                    config.Save(ConfigurationSaveMode.Full);
                    ConfigurationManager.RefreshSection("appSettings");
                    config.AppSettings.Settings["LOG_NO_ROTATE"].Value = iNoOfLogFile.ToString();
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                }

                try
                {
                    strLogFilePath = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["LOG_FILE_PATH"]);
                    if (strLogFilePath.Length > 0)
                    {
                        if (strLogFilePath.Last() != '\\')
                        {
                            strLogFilePath += "\\";
                        }
                        if (Directory.Exists(strLogFilePath) == false)
                        {
                            Directory.CreateDirectory(strLogFilePath);
                        }
                    }
                }
                catch(Exception ex)
                {
                    strLogFilePath = "C:\\founder\\temp\\AutoDelete_Log\\";
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    config.AppSettings.Settings.Add("LOG_FILE_PATH", strLogFilePath);
                    config.Save(ConfigurationSaveMode.Full);
                    ConfigurationManager.RefreshSection("appSettings");
                    config.AppSettings.Settings["LOG_FILE_PATH"].Value = strLogFilePath;
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                }

              
                dFileSize = Convert.ToDouble(System.Configuration.ConfigurationSettings.AppSettings["LOG_MAX_FILE_SIZE"]);
                if (dFileSize == 0)
                { 
                    dFileSize = 500;
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    config.AppSettings.Settings.Add("LOG_MAX_FILE_SIZE", dFileSize.ToString());
                    config.Save(ConfigurationSaveMode.Full);
                    ConfigurationManager.RefreshSection("appSettings");
                    config.AppSettings.Settings["LOG_MAX_FILE_SIZE"].Value = dFileSize.ToString();
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                }
               
                iCurrentLogNo = Convert.ToInt32(System.Configuration.ConfigurationSettings.AppSettings["CURRENT_LOG_NO"]);
                if (iCurrentLogNo==0)
                {
                    iCurrentLogNo = 1;
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    KeyValueConfigurationElement element = new KeyValueConfigurationElement("CURRENT_LOG_NO", iCurrentLogNo.ToString());
                    config.AppSettings.Settings.Add(element);
                    config.Save(ConfigurationSaveMode.Full);
                    ConfigurationManager.RefreshSection("appSettings");
                    config.AppSettings.Settings["CURRENT_LOG_NO"].Value = iCurrentLogNo.ToString();
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                }

                try
                {
                    String strTestingThread = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["LOG_TESTING_DATA"]);
                    bTestingThread = Int32.Parse(strTestingThread)==0?false:true;
                }
                catch (Exception ex)
                {
                    bTestingThread = false;
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    config.AppSettings.Settings.Add("LOG_TESTING_DATA", bTestingThread == false?"0":"1");
                    config.Save(ConfigurationSaveMode.Full);
                    ConfigurationManager.RefreshSection("appSettings");
                    config.AppSettings.Settings["LOG_TESTING_DATA"].Value = bTestingThread == false ? "0" : "1";
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                }

                try
                {
                    strErrorFileMovePath = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["ErrorFileMovePath"]);
                    if (strErrorFileMovePath.Length > 0)
                    {
                        if (strErrorFileMovePath.Last() != '\\')
                        {
                            strErrorFileMovePath += "\\";
                        }
                        if (Directory.Exists(strErrorFileMovePath) == false)
                        {
                            Directory.CreateDirectory(strErrorFileMovePath);
                        }
                    }

                }
                catch (Exception ex)
                {
                    strErrorFileMovePath = "C:\\founder\\temp\\AutoDelete_Error\\";
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                    config.AppSettings.Settings.Add("ErrorFileMovePath", strErrorFileMovePath);
                    config.Save(ConfigurationSaveMode.Full);
                    ConfigurationManager.RefreshSection("appSettings");
                    config.AppSettings.Settings["ErrorFileMovePath"].Value = strErrorFileMovePath;
                    config.Save(ConfigurationSaveMode.Modified);
                    ConfigurationManager.RefreshSection("appSettings");
                }

                System.Globalization.GregorianCalendar gc = new System.Globalization.GregorianCalendar();
                strCurWeek = gc.GetWeekOfYear(DateTime.Now, System.Globalization.CalendarWeekRule.FirstDay, DayOfWeek.Monday);
                System.Timers.Timer aTimer = new System.Timers.Timer(1000);
                aTimer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
                aTimer.Interval = nInterval;
                //aTimer.Enabled = true;
                serviceTimer = aTimer;
                serviceTimer.Enabled = true;
            }
            catch (Exception e)
            {
                WriteToFile("Error :" + e.Message.Trim());
            }
        }


        //  private void timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        private void OnTimedEvent(object source, ElapsedEventArgs e)
        {
            // ignore the time, just compare the date
            DateTime dt1 = DateTime.ParseExact(strStartTime, "HH:mm", System.Globalization.CultureInfo.CurrentCulture);
            if (DateToRun() && DateTime.Now.CompareTo(dt1) > 0)
            {
                String message = "Windows service checking started.";
                WriteToFile(message);
                if (DeleteFile(strDbConnStr) == true)
                {
                    message = "Windows service checking finished. " + "\r\n\r\n";
                    //eventLog1.WriteEntry(message);
                    WriteToFile(message);
                    if (strRunDate.Length == 0)
                        strRunDate = DateTime.Now.DayOfWeek.ToString();
                    else
                        strRunDate = strRunDate + ";" + DateTime.Now.DayOfWeek.ToString();
                }
                else
                {
                    message = "Windows service error " + "\r\n\r\n";
                    WriteToFile(message);
                }
            }
            else
                WriteToFile("Not on running time, waiting...");
        }

        private bool DateToRun()
        {
            string day = DateTime.Now.DayOfWeek.ToString();
            System.Globalization.GregorianCalendar gc = new System.Globalization.GregorianCalendar();
            strCurWeek = gc.GetWeekOfYear(DateTime.Now, System.Globalization.CalendarWeekRule.FirstDay, DayOfWeek.Monday);
            if (strRunDate.CompareTo(strStartDate) == 0 || strCurWeek != strWeekYear)
            {
                strRunDate = "";
                strWeekYear = strCurWeek;
            }
            if (strStartDate.Length == 0)
            {
                if (_lastRun.Date < DateTime.Now.Date)
                {
                    return true;
                }
                else if (_lastRun.Date == DateTime.Now.Date)
                    return false;
            }
            if (strStartDate.Contains(day) && !strRunDate.Contains(day))
            {
                return true;
            }
            else
                return false;
        }
        
        private static int iMaxRecordRecord = 10000;
        private bool DeleteFile(string strDbConnStr)
        {
            serviceTimer.Stop();
            DataTable AutoDelet_Table = new DataTable();
            AutoDelet_Table.Clear();
            string message, path, type, date;
            //string sqlEMSSelect = "select * from auto_delete_record where status!='Succeed'";
            string sqlEMSSelect = "select count(*), max(id), min(id) from auto_delete_record where status!='Succeed'";

            System.Data.SqlClient.SqlConnection connection = new System.Data.SqlClient.SqlConnection(strDbConnStr);
            try
            {
                connection.Open();
            }
            catch (Exception ex)
            {
                WriteToFile("Database connection error: "+ex.Message.Trim());
                serviceTimer.Start();
                return false;
            }
            SqlDataAdapter da = new SqlDataAdapter(sqlEMSSelect, strDbConnStr);
            try
            {
                da.Fill(AutoDelet_Table);
                SqlCommandBuilder Obuilder = new SqlCommandBuilder(da);
            }
            catch (Exception ex)
            {
                WriteToFile("Database error: check user acount or database");
                serviceTimer.Start();
                return false;
            }
            CheckAdminProFolderPathSetting();
            if (bTestingThread)
                WriteToFile("AdminPro setting [" + strComposeDateFormat + "]" + "[" + strComposeFileSubDir + "]");

            int total = int.Parse(AutoDelet_Table.Rows[0].ItemArray[0].ToString());
            if (total == 0)
            {
                serviceTimer.Start();
                return true;
            }
            int iMaxId = int.Parse(AutoDelet_Table.Rows[0].ItemArray[1].ToString());
            int iMinId = int.Parse(AutoDelet_Table.Rows[0].ItemArray[2].ToString());
            int iCurrentid = iMinId;
            int iTotalAfter = 0;
            while (iCurrentid < iMaxId)
            {
                int iSearchindex = iCurrentid + iMaxRecordRecord;

                sqlEMSSelect = "select * from (SELECT *, " +
                    "ROW_NUMBER() OVER (ORDER BY id) AS RowNumber " +
                    "FROM auto_delete_record " +
                    "where status!='Succeed')a " +
                    "where a.id between " + iCurrentid + " and " + iSearchindex;
               
                da = new SqlDataAdapter(sqlEMSSelect, strDbConnStr);
                try
                {
                    AutoDelet_Table.Clear();
                    da.Fill(AutoDelet_Table);
                    SqlCommandBuilder Obuilder = new SqlCommandBuilder(da);
                }
                catch (Exception ex)
                {
                    WriteToFile("Database error: check user acount or database");
                    serviceTimer.Start();
                    return false;
                }
                foreach (DataRow row in AutoDelet_Table.Rows)
                {
                    try
                    {
                        iCurrentid = Int32.Parse(row["id"].ToString());
                    }
                    catch (Exception e)
                    {
                        WriteToFile("Wrong Id captured :" + e.Message.Trim());
                    }
                    message = row["path"].ToString() + "  " + row["type"].ToString() + "  " + row["delete_time"].ToString();
                    path = row["path"].ToString();
                    type = row["type"].ToString();
                    date = row["delete_time"].ToString();

                    //if (bTestingThread)
                        //WriteToFile("Processing [" + path + "]" + "[" + type + "]" + "[" + date + "]");

                    bool nRet, iCheckDatePath = false;
                    try
                    {
                        if (type.Equals("wires_attach", StringComparison.OrdinalIgnoreCase))
                        {
                            nRet = DeleteWiresAttach(path);
                            if (nRet == true)
                            {
                                try
                                {
                                    FileInfo file = new FileInfo(path);
                                    ClearEmptyWiresAttachFolder(file.Directory.ToString());
                                }
                                catch (Exception ex)
                                {
                                }
                            }
                        }
                        else if (type.Equals("push_dir", StringComparison.OrdinalIgnoreCase))
                        {
                            nRet = DeletePushDirPhoto(path);
                        }
                        else if (date.Length == 0)
                        {
                            nRet = DeleteFileFolder(path, "", date);
                            iCheckDatePath = true;
                        }
                        else
                        {
                            nRet = DeleteFileFolder(path, type, date);
                        }

                    }
                    catch (Exception e)
                    {
                        WriteToFile("Error Catched: " + e.Message.Trim());
                        if (e.Message.Trim().IndexOf("Could not find file") > -1 ||
                            e.Message.Trim().IndexOf("Could not find a part of the path") > -1)
                        {
                            WriteToFile("No action take because of:" + e.Message.Trim());
                            nRet = true;
                        }
                        else
                            //continue;
                            nRet = false;
                    }
                    //if (DeleteFileFolder(path, type, date))
                    if (nRet)
                    {
                        row["status"] = "Succeed";
                        if (iCheckDatePath)
                        {
                            OnCheckFolderWithDateFormat(path, type);
                        }
                    }
                    else
                    {
                        row["status"] = "Failed";
                    }
                    row["last_modify_time"] = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
                    row["count"] = Convert.ToInt32(row["count"].ToString()) + 1;
                    String sql = "update auto_delete_record set last_modify_time = N'" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + "'," +
                        " count = " + row["count"] + ", status = N'" + row["status"] + "' where id = " + row["id"];

                    System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand(sql, connection);
                    try
                    {
                        command.ExecuteNonQuery();
                    }
                    catch (Exception e)
                    {
                        WriteToFile(sql + " : \r\n" + e.Message.Trim());
                    }
                }
                iCurrentid = iSearchindex;
            }


            DeletePushDir();
            
            connection.Close();
            ClearEmptyPhotoFolder();
            serviceTimer.Start();
            
            return true;
        }

        public void ClearEmptyWiresAttachFolder(String strPathFormat)
        {
            DirectoryInfo dii = new DirectoryInfo(strPathFormat);
            FileInfo[] filearray = dii.GetFiles();
            DirectoryInfo[] directarray = dii.GetDirectories();
            if ((directarray.Count() == 0 && filearray.Count() == 1 && filearray[0].Name == "Thumbs.db") ||
                (directarray.Count() == 0 && filearray.Count() == 0))
            {
                dii.Delete(true);
            }
        }

        public void ClearEmptyPhotoFolder()
        {
            if (lst_PhotoDelSubPath.Count > 0)
            {
                for (int i = 0; i < lst_PhotoDelSubPath.Count; i++)
                {
                    try
                    {
                        DirectoryInfo dii = new DirectoryInfo(lst_PhotoDelSubPath[i]);
                        FileInfo[] filearray = dii.GetFiles();
                        DirectoryInfo[] directarray = dii.GetDirectories();
                        if ((directarray.Count() == 0 && filearray.Count() == 1 && filearray[0].Name == "Thumbs.db") ||
                            (directarray.Count() == 0 && filearray.Count() == 0))
                        {
                            dii.Attributes = FileAttributes.Normal;
                            foreach (var info in dii.GetFileSystemInfos("*", SearchOption.AllDirectories))
                            {
                                info.Attributes = FileAttributes.Normal;
                            }
                            dii.Delete(true);
                            lst_PhotoDelSubPath.RemoveAt(i);
                            i = -1;
                        }
                    }
                    catch (Exception ex)
                    {
                        WriteToFile("Delete Empty Subfolder Failed: " + ex.Message.Trim() + "[" + lst_PhotoDelSubPath[i] + "]");
                    }
                }
            }
            if (lst_PhotoDelPath.Count > 0)
            {
                for (int i = 0; i < lst_PhotoDelPath.Count; i++)
                {
                    try
                    {
                        DirectoryInfo dii = new DirectoryInfo(lst_PhotoDelPath[i]);
                        FileInfo[] filearray = dii.GetFiles();
                        DirectoryInfo[] directarray = dii.GetDirectories();
                        if ((directarray.Count() == 0 && filearray.Count() == 1 && filearray[0].Name == "Thumbs.db") ||
                            (directarray.Count() == 0 && filearray.Count() == 0))
                        {
                            dii.Attributes = FileAttributes.Normal;
                            foreach (var info in dii.GetFileSystemInfos("*", SearchOption.AllDirectories))
                            {
                                info.Attributes = FileAttributes.Normal;
                            }
                            dii.Delete(true);
                            lst_PhotoDelPath.RemoveAt(i);
                            i = -1;
                        }
                    }
                    catch (Exception ex)
                    {
                        WriteToFile("Delete Empty Folder Failed: " + ex.Message.Trim() + "[" + lst_PhotoDelPath[i] + "]");
                    }
                }
            }
        }

        public void DeleteEmptyFolder(String path, String type, bool isColorSep)
        {
            try
            {
                if ((strComposeFileSubDir.IndexOf("DATE") == 0 && !strComposeFileSubDir.Equals("DATE")) || isColorSep)
                {
                    DirectoryInfo dii = new DirectoryInfo(path);
                    List<DirectoryInfo> lst_dir = dii.GetDirectories().ToList();
                    for (int i = 0; i < lst_dir.Count; i++)
                    {
                        if (lst_dir[i].GetDirectories().ToList().Count == 0)
                            DeleteEmptyDirs(lst_dir[i].FullName, "");
                    }
                }
                if (strComposeFileSubDir.IndexOf("CODE") > -1 && !strComposeFileSubDir.Equals("CODE"))
                {
                    DeleteEmptyDirs(path + "\\" + type, path + "\\" + type);
                }
            }catch(Exception ex){}
            return;
        }

        public String CheckLogFile()
        {
            //serviceTimer.Stop();
            String strLogFileName = strLogFilePath + "AutoDelete_"+iCurrentLogNo+"_log.txt";
            FileInfo fileInfo = new FileInfo(strLogFileName);
            try
            {
                if (fileInfo.Length > dFileSize * 1024 * 1024)
                {
                    //WriteToTestFile(strLogFileName + ":" + fileInfo.Length);
                    iCurrentLogNo++;
                    if (iCurrentLogNo > iNoOfLogFile)
                    {
                        iCurrentLogNo = 1;
                        ZipLogFile();
                    }
                    System.Configuration.Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
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
                    strLogFileName = strLogFilePath + "AutoDelete_" + iCurrentLogNo + "_log.txt";
                    fileInfo = new FileInfo(strLogFileName);
                    if (fileInfo.Length > dFileSize * 1024 * 1024)
                    {

                    }
                }
            }
            catch (Exception ex)
            {
                //WriteToTestFile("File does not exists");
                FileStream file = File.Create(strLogFileName);
                file.Close();
                //WriteToTestFile("File create: " + strLogFileName);
            }
            //serviceTimer.Start();
            return strLogFileName;
        }

        public void ZipLogFile()
        {
            try
            {
                WriteToFile("Start zip file, please wait");
                String str_ZipFilePath = strLogFilePath + "AutoDelete" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".zip";
                //WriteToTestFile("ZIP all log file to " + str_ZipFilePath);
                for (int i = iCurrentLogNo; i <= iNoOfLogFile; i++)
                {
                    String sourceFile = strLogFilePath + "AutoDelete_" + i + "_log.txt";
                    //WriteToTestFile("Processing: " + sourceFile);
                    AddFileToZip(str_ZipFilePath, sourceFile);
                    File.Delete(sourceFile);
                }
            }catch(Exception ex)
            {
                WriteToFile("Error ocurr when zip log file: "+ex.Message.Trim());
            }
            //WriteToTestFile("ZIP finish");
            //WriteToTestFile("Start on next round\r\n");
        }

        private void AddFileToZip(string zipFilename, string fileToAdd)
        {
            try
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
            catch (Exception ex)
            {
                WriteToFile("Add file to zip error: "+ex.Message.Trim());
            }
        }

        private const long BUFFER_SIZE = 4096;
        private void CopyStream(System.IO.FileStream inputStream, System.IO.Stream outputStream)
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
            //WriteToTestFile("Byte Written: "+bytesWritten.ToString());
        }

        public void WriteToTestFile(String message)
        {
            //return;
            try
            {
                StreamWriter sw = new StreamWriter("C:\\Founder\\AutoDeletelog_Test.txt", true);
                Encoding encode = System.Text.UnicodeEncoding.Unicode;
                sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm   ") + message);
                sw.Flush();
                sw.Close();
            }
            catch (Exception ex)
            { }
        }

        public void WriteToFile(String message)
        {
            //WriteToTestFile
            try
            {
                String strFileName = CheckLogFile();
                //StreamWriter sw = new StreamWriter("C:\\Founder\\AutoDeletelog_" + DateTime.Now.ToString("yyyy_MM_dd") + ".txt", true);
                Encoding encode = System.Text.UnicodeEncoding.Unicode;
                StreamWriter sw = new StreamWriter(strFileName, true, encode);
                //for (int i = 0; i < 50;i++ )
                sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm   ") + message);
                sw.Flush();
                sw.Close();
            }catch(Exception ex)
            {
            }
        }

        public void CheckAdminProFolderPathSetting()
        {
            String strReturnPath = "";
            DataTable AutoDelet_Table = new DataTable();
            string sqlEMSSelect = "select * from sys_newsroom  where entry in(N'paper_lib_datefmt', N'paper_lib_subdir')";

            System.Data.SqlClient.SqlConnection connection = new System.Data.SqlClient.SqlConnection(strDbConnStr);
            connection.Open();
            SqlDataAdapter da = new SqlDataAdapter(sqlEMSSelect, strDbConnStr);
            try
            {
                da.Fill(AutoDelet_Table);
                SqlCommandBuilder Obuilder = new SqlCommandBuilder(da);
            }
            catch (Exception ex)
            {
                WriteToFile("Database error: check user acount or database");
                return ;
            }
            foreach (DataRow row in AutoDelet_Table.Rows)
            {
                if (row["entry"].Equals("paper_lib_datefmt"))
                {
                    strComposeDateFormat = row.ItemArray[1].ToString().Substring(1);
                }
                if (row["entry"].Equals("paper_lib_subdir"))
                {
                    strComposeFileSubDir = row.ItemArray[1].ToString().Substring(1);
                }
            }
            return;
        }

        public int CheckDate(String strFolderDate, DateTime dt_CheckDate) // 0 = should not nr delete, 1 = should be delete, 2 = return failed
        {
            int bRet = 0;
            int indexY = -1, indexD = -1, indexM = -1;
            indexY = strComposeDateFormat.IndexOf("Y");
            indexD = strComposeDateFormat.IndexOf("D");
            indexM = strComposeDateFormat.IndexOf("M");
            String year = "0", month = "0", date = "0";
            try
            {
                if (indexY != -1)
                {
                    year = strFolderDate.Substring(indexY, 4);
                }
                if (indexD != -1)
                {
                    date = strFolderDate.Substring(indexD, 2);
                }
                if (indexM != -1)
                {
                    month = strFolderDate.Substring(indexM, 2);
                }
                if (year.Equals("0"))
                    year = DateTime.Now.Year.ToString();

                DateTime dt1 = new DateTime(int.Parse(year), int.Parse(month), int.Parse(date));
                if (dt_CheckDate.CompareTo(dt1) >= 0)
                    bRet = 1;
                else
                    bRet = 0;
            }catch(Exception ex)
            {
                WriteToFile("Error catched on date time convert with the following message: "+ex.Message);
                if(bTestingThread)
                    WriteToFile("Date get from database to be parse:(" + dt_CheckDate.ToString() + ") Date get from Folder Path to be parse:(" + strFolderDate + ") Target time format:(" + strComposeDateFormat + ")");
                bRet = 2;
            }
            return bRet;
        }

        public bool DeleteWiresAttach(string path)
        {
            bool nRet = true;
            if (File.Exists(path))
            {
                try
                {
                    FileAttributes attributes = File.GetAttributes(path);
                    if ((attributes & FileAttributes.ReadOnly) != FileAttributes.ReadOnly)
                        File.Delete(path);
                    else
                    {
                        File.SetAttributes(path, FileAttributes.Normal);
                        File.Delete(path);
                    }
                
                }
                catch (Exception ex)
                {
                    WriteToFile("Error Catched: " + ex.Message.Trim());
                    nRet = false;
                }
            }
//            return true;

            if (nRet)
            {
                string message = "File deleted: " + path;
                if (bTestingThread)
                    WriteToFile(message);
            }            
            return nRet;
        }

        public bool DeleteFileFolder(string path, string type, string date)
        {
            string message, errorPath, subFolderPath;
            errorPath = path;
            bool bAllSuccess = true;
            String strInProgressPath= "";
            try
            {
                if (type.Length == 0 && date.Length == 0)
                {
                    strInProgressPath = path;
                    if (File.Exists(path))
                    {
                        if ((File.GetAttributes(path) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                            File.SetAttributes(path, FileAttributes.Normal);
                        File.Delete(path);
                        message = "File deleted: " + path;
                        if (bTestingThread)
                            WriteToFile(message);
                    }
                    else
                    {
                        message = "File path not exists: " + path + " Try Delete directly";
                        WriteToFile(message);
                        try
                        {
                            if ((File.GetAttributes(path) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                                File.SetAttributes(path, FileAttributes.Normal);
                        }
                        catch (Exception ex)
                        {
                            message = "Failed to change File Attributes:  Because of : " + ex.Message.Trim();
                            WriteToFile(message);
                        }
                        try
                        {
                            File.Delete(path);
                            message = "File deleted: " + path;
                            if (bTestingThread)
                                WriteToFile(message);
                        }
                        catch (Exception ex)
                        {
                            message = "Failed to delete:  Because of : " + ex.Message.Trim();
                            WriteToFile(message);
                        }
                    }
                    return true;
                }
                else
                {
                    DateTime dt_CheckDate = DateTime.Parse(date);
                    if (strComposeFileSubDir.Equals("CODE"))
                    {
                        List<string> dir = Directory.GetDirectories(@path).ToList();
                        foreach (string item in dir)
                        {
                            try
                            {
                                if (Directory.Exists(item))
                                {
                                    List<string> files = Directory.GetFiles(@item).ToList();
                                    foreach (string file in files)
                                    {
                                        File.Delete(file);
                                        message = "File deleted: " + file;
                                        if (bTestingThread)
                                            WriteToFile(message);
                                    }
                                }
                                else if (!Directory.Exists(item))
                                {
                                    message = "Folder path not exists: " + item + " No action take";
                                    WriteToFile(message);
                                }
                            }
                            catch (Exception ex)
                            {
                                message = "Failed to delete: " + errorPath + " Because of : " + ex.Message.Trim();
                                WriteToFile(message);
                                bAllSuccess = false;
                            }
                        }
                    }
                    //if (strComposeFileSubDir.IndexOf("DATE") == 0)
                    if (strComposeFileSubDir.IndexOf("DATE") == 0 || path.IndexOf("\\colorsep") > -1 || path.IndexOf("\\photo_sign") > -1)
                    {
                        List<string> dir = Directory.GetDirectories(@path).ToList();
                        if (dir.Count <= 0)
                        {
                            if (bTestingThread)
                                WriteToFile("No action take because no directories can be deleted." + "[" + path + "]" + "[" + type + "]" + "[" + dt_CheckDate.ToString("yyyy-MM-dd") + "]");
                        }
                        foreach (string item in dir)
                        {
                            try
                            {
                                subFolderPath = "";
                                errorPath = subFolderPath = item;
                                string str = System.IO.Path.GetFileNameWithoutExtension(item);
                                int nRet = CheckDate(str, dt_CheckDate);
                                if (nRet == 1)
                                {
                                    if (Directory.Exists(subFolderPath))
                                    {
                                        DirectoryInfo dii = new DirectoryInfo(subFolderPath);
                                        foreach (var info in dii.GetFileSystemInfos("*", SearchOption.AllDirectories))
                                        {
                                            info.Attributes = FileAttributes.Normal;
                                        }
                                        if (strComposeFileSubDir.Equals("DATE"))
                                        {
                                            strInProgressPath = subFolderPath;
                                            Directory.Delete(subFolderPath, true);
                                            if (bTestingThread)
                                                WriteToFile("Folder deleted: " + subFolderPath);
                                        }
                                        else
                                        {
                                            //DirectoryInfo dii = new DirectoryInfo(subFolderPath);
                                            DirectoryInfo[] directarray = dii.GetDirectories();
                                            for (int i = 0; i < directarray.Count(); i++)
                                            {
                                                if (System.IO.Path.GetFileNameWithoutExtension(directarray[i].FullName).Equals(type))
                                                {
                                                    strInProgressPath = directarray[i].FullName;
                                                    Directory.Delete(directarray[i].FullName, true);
                                                    if (bTestingThread)
                                                        WriteToFile("Folder deleted: " + directarray[i].FullName);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else if (!Directory.Exists(subFolderPath))
                                    {
                                        message = "Folder path not exists: " + subFolderPath + " No action take";
                                        WriteToFile(message);
                                    }
                                }
                                else
                                {
                                    if (nRet == 2)
                                    {
                                        bAllSuccess = false;
                                        message = "Folder path format incorrect: " + subFolderPath;
                                        WriteToFile(message);
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                message = "Failed to delete: " + errorPath + " Because of : " + ex.Message.Trim();
                                WriteToFile(message);

                                String strTemp = "";
                                strTemp = ex.Message.Substring(ex.Message.IndexOf('\'') + 1, ex.Message.LastIndexOf('\'') - ex.Message.IndexOf('\'') - 1);
                                // Edit Stephen 2020-07-02
                                if (strTemp.IndexOf('\\') < 0 && strTemp.IndexOf('.') > 0) 
                                // Edit Stephen 2020-07-02
                                    HandleFaultDeletion(strTemp, strInProgressPath, date.Length > 0);

                                bAllSuccess = false;
                            }
                        }
                        DeleteEmptyFolder(path, type, true);
                    }
                    else
                    {
                        List<string> dir = Directory.GetDirectories(@path).ToList();
                        bool bFound = false;
                        foreach (string item in dir)
                        {
                            String strPathName = System.IO.Path.GetFileNameWithoutExtension(item);
                            if (!strPathName.Equals(type))
                                continue;
                            bFound = true;
                            List<string> subDir = Directory.GetDirectories(@item).ToList();
                            foreach (string SubItem in subDir)
                            {
                                string str = System.IO.Path.GetFileNameWithoutExtension(SubItem);
                                try
                                {
                                    int nRet = CheckDate(str, dt_CheckDate);
                                    if (nRet == 1)
                                    {
                                        if (Directory.Exists(SubItem))
                                        {
                                            strInProgressPath = SubItem;
                                            Directory.Delete(SubItem, true);
                                            if (bTestingThread)
                                                WriteToFile("Folder deleted: " + SubItem);
                                        }
                                        else if (!Directory.Exists(SubItem))
                                        {
                                            message = "Folder path not exists: " + SubItem + " No action take";
                                            WriteToFile(message);
                                        }
                                    }
                                    else
                                    {
                                        if (nRet == 2)
                                        {
                                            bAllSuccess = false;
                                            message = "Folder path format incorrect: " + SubItem;
                                            WriteToFile(message);
                                        }
                                    }

                                }
                                catch (Exception ex)
                                {
                                    message = "Failed to delete: " + errorPath + " Because of : " + ex.Message.Trim();
                                    WriteToFile(message);
                                    String strFileName;

                                    String strTemp = "";
                                    strTemp = ex.Message.Substring(ex.Message.IndexOf('\'') + 1, ex.Message.LastIndexOf('\'') - ex.Message.IndexOf('\'')-1);

                                    // Edit Stephen 2020-07-02
                                    if (strTemp.IndexOf('\\') < 0 && strTemp.IndexOf('.') > 0)
                                    // Edit Stephen 2020-07-02
                                       HandleFaultDeletion(strTemp, strInProgressPath, date.Length > 0);
 
                                    bAllSuccess = false;
                                }
                            }
                        }
                        if (!bFound)
                        {
                            if (bTestingThread)
                                WriteToFile("No action take because path not found." + "[" +path+ "]" + "[" +type+ "]");
                        }
                        DeleteEmptyFolder(path, type, false);
                    }
                    return bAllSuccess;
                }
            }
            catch (Exception e)
            {
                message = "Failed to delete: " + errorPath + " Becuse of : "+e.Message.Trim();
                WriteToFile(message);

                HandleFaultDeletion(errorPath, "", date.Length > 0);
                return false;
            } 
        }
        protected override void OnStop()
        {
            WriteToFile("Windows service OnStop \r\n");
            //eventLog1.WriteEntry("In onStop.");
        }

        protected override void OnContinue()
        {
            WriteToFile("Windows service OnContinue \r\n");
           // eventLog1.WriteEntry("In OnContinue.");
        }

        private void DeleteEmptyDirs(string dir, string dirNoDelete)
        {
            if (String.IsNullOrEmpty(dir))
                throw new ArgumentException(
                    "Starting directory is a null reference or an empty string",
                    "dir");

            try
            {
                foreach (var d in Directory.EnumerateDirectories(dir))
                {
                    DeleteEmptyDirs(d, dirNoDelete);
                }

                var entries = Directory.EnumerateFileSystemEntries(dir);

                if (!entries.Any())
                {
                    try
                    {
                        if (!dir.Equals(dirNoDelete))
                            Directory.Delete(dir);
                    }
                    catch (UnauthorizedAccessException) { }
                    catch (DirectoryNotFoundException) { }
                }
            }
            catch (UnauthorizedAccessException) { }
        }

        protected bool OnCheckFolderWithDateFormat(String str_Path, String str_Type)
        {
            bool nRet = false;
            try
            {
                if (str_Type.Length == 0)
                    return true;
                
                String str_Directory = str_Path.Remove(str_Path.LastIndexOf("\\"));
                DataTable DbGraphicSetting_Table = new DataTable();
                DbGraphicSetting_Table.Clear();
                string message, path, type, date;
                if (str_Directory.Substring(str_Directory.LastIndexOf("\\")) == "\\mid" || str_Directory.Substring(str_Directory.LastIndexOf("\\")) == "\\small")
                {
                    //try
                    //{
                    //    DirectoryInfo di = new DirectoryInfo(str_Directory);
                    //    message = "Folder deleted: " + str_Directory;
                    //    di.Delete(false);
                    //    if (bTestingThread)
                    //        WriteToFile(message);
                    //}
                    //catch (Exception e) { message = "Failed to delete: " + str_Directory + " Because of: "+e.Message.Trim(); }
                    String strFound = lst_PhotoDelSubPath.Find(delegate(String str) { return str == str_Directory; });
                    if (strFound == null)
                        lst_PhotoDelSubPath.Add(str_Directory);
                }
                string sqlEMSSelect;
                if (str_Type.IndexOf("(A)") > -1)
                {
                    str_Type = str_Type.Replace("(A)", "");
                    sqlEMSSelect = "select * from dbgraphi_info where db_agencyid =" + str_Type;
                }
                else
                {
                    str_Type = str_Type.Replace("(D)", "");
                    sqlEMSSelect = "select * from dbgraphi_info where db_dpmtid =" + str_Type;
                }
                SqlDataAdapter da = new SqlDataAdapter(sqlEMSSelect, strDbConnStr);
                try
                {
                    da.Fill(DbGraphicSetting_Table);
                    SqlCommandBuilder Obuilder = new SqlCommandBuilder(da);
                }
                catch (Exception ex)
                {
                    WriteToFile("Database error: check user acount or database");
                    return false;
                }
                foreach (DataRow row in DbGraphicSetting_Table.Rows)
                {
                    path = row["db_PicSource"].ToString();
                    int iPosY = -1, iPosM = -1, iPosD = -1, iYear = 0, iMonth = 0, iDay = 0;
                    if (path.IndexOf("%Y") != -1)
                        iPosY = path.IndexOf("%Y");
                    else if (path.IndexOf("%y") != -1)
                        iPosY = path.IndexOf("%y");
                    if (iPosY != -1)
                    {
                        try
                        {
                            iYear = Int32.Parse(str_Path.Substring(iPosY, 4));
                        }
                        catch (Exception e) { }
                    }
                    if (path.IndexOf("%m") != -1)
                        iPosM = path.IndexOf("%m");
                    else if (path.IndexOf("%M") != -1)
                        iPosM = path.IndexOf("%M");
                    if (iPosM != -1)
                    {
                        try
                        {
                            if (iPosY != -1 && iPosY < iPosM)
                                iPosM += 2;
                            iMonth = Int32.Parse(str_Path.Substring(iPosM, 2));
                        }
                        catch (Exception e) { }
                    }
                    if (path.IndexOf("%d") != -1)
                        iPosD = path.IndexOf("%d");
                    else if (path.IndexOf("%D") != -1)
                        iPosD = path.IndexOf("%D");
                    if (iPosD != -1)
                    {
                        try
                        {
                            if (iPosY != -1 && iPosY < iPosD)
                                iPosD += 2;
                            iDay = Int32.Parse(str_Path.Substring(iPosD, 2));
                        }
                        catch (Exception e) { }
                    }
                    String str_Test = path;
                    if (iDay != 0)
                    {
                        str_Test = str_Test.Replace("%d", iDay.ToString("D2"));

                        str_Test = str_Test.Replace("%D", iDay.ToString("D2"));
                    }
                    if (iMonth != 0)
                    {
                        str_Test = str_Test.Replace("%m", iMonth.ToString("D2"));
                        str_Test = str_Test.Replace("%M", iMonth.ToString("D2"));
                    }
                    if (iYear != 0)
                    {
                        str_Test = str_Test.Replace("%Y", iYear.ToString());
                        str_Test = str_Test.Replace("%y", iYear.ToString());
                    }
                    if (iPosD == -1 && iPosM == -1 && iPosY == -1)
                        return false;
                    //if (str_test == str_Test)
                    //WriteToFile(str_Test);
                    {
                        //try
                        //{
                        //    DirectoryInfo dii = new DirectoryInfo(str_Test);
                        //    message = "Root Folder deleted: " + str_Test;
                        //    FileInfo[] filearray = dii.GetFiles();
                        //    DirectoryInfo[] directarray = dii.GetDirectories();
                        //    if (directarray.Count() == 0 && filearray.Count() == 1 && filearray[0].Name == "Thumbs.db")
                        //        dii.Delete(true);
                        //    else
                        //        dii.Delete(false);
                        //    if (bTestingThread)
                        //        WriteToFile(message);
                        //}
                        //catch (Exception e)
                        //{
                        //    message = "Failed to delete Root Folder: " + str_Test+" \r\n Because of: "+e.Message.Trim();
                        //    WriteToFile(message);
                        //}
                        String strFound = lst_PhotoDelPath.Find(delegate(String str) { return str == str_Test; });
                        if (strFound == null)
                            lst_PhotoDelPath.Add(str_Test);
                    }
                }
                return nRet;
            }
            catch (Exception ex)
            {
                WriteToFile("Check Folder Date Format Error: "+ex.Message.Trim());
                return nRet;
            }
        }

		private void HandleFaultDeletion(String strFileName, String strInProgressPath, bool bHaveDate)
        {
            try
            {
                String strTempErrorPath = strErrorFileMovePath;
                String strErrorFilePath = "";
                DateTime dt_CheckDateTemp = DateTime.Now;
                String strDate;
                strDate = String.Format("{0:0000}{1:00}{2:00}", dt_CheckDateTemp.Year, dt_CheckDateTemp.Month, dt_CheckDateTemp.Day);
                strTempErrorPath += strErrorFileMovePath.LastIndexOf("\\") == strErrorFileMovePath.Length - 1 ?
                    strDate + "\\" : "\\" + strDate + "\\";

                if (bHaveDate == true)
                {
                    strFileName = strInProgressPath.LastIndexOf("\\") == strInProgressPath.Length - 1 ?
                    strInProgressPath += strFileName : strInProgressPath += "\\" + strFileName;

                    Directory.CreateDirectory(strTempErrorPath);
                    strTempErrorPath = strTempErrorPath + strFileName.Substring(strFileName.LastIndexOf("\\") + 1);
                    File.Move(strFileName, strTempErrorPath);

                }
                else
                {
                    strTempErrorPath = strTempErrorPath + strFileName.Substring(strFileName.LastIndexOf("\\") + 1);
                    File.Move(strFileName, strTempErrorPath);
                }
            }
            catch (Exception ex)
            {
                WriteToFile("Error on Handling move error file: "+ex.Message);
            }
        }

        public bool DeletePushDirPhoto(String strPath)
        {
            if (strPath.LastIndexOf("\\") < 0) return true;
            if (strPath.LastIndexOf(".") < 0) return true;

            string message = "";
            string strFileName = strPath.Substring(strPath.LastIndexOf("\\")+1);
            string strFileName_NoExt = strFileName.Substring(0, strFileName.LastIndexOf("."));
            bool nRet = true;

            try
            {
                if (File.Exists(strPath))
                {
                    if ((File.GetAttributes(strPath) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                        File.SetAttributes(strPath, FileAttributes.Normal);
                    File.Delete(strPath);
                    message = "File deleted: " + strPath;
                    if (bTestingThread)
                        WriteToFile(message);
                }
                else
                {
//                    message = "File path not exists: " + strPath + " No action take";
//                    WriteToFile(message);
                }

                string strDir = strPath.Substring(0, strPath.LastIndexOf("\\"));
                string strDirUpperLevel = strDir.Substring(0, strDir.LastIndexOf("\\"));
                string strExt = strPath.Substring(strPath.LastIndexOf("."));
                strExt = strExt.Replace(".", "");
/*
                // delete caption
                strPath = strDir + "\\" + strFileName_NoExt + ".txt";
                if (File.Exists(strPath))
                {
                    if ((File.GetAttributes(strPath) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                        File.SetAttributes(strPath, FileAttributes.Normal);
                    File.Delete(strPath);
                }

                // delete DAT
                strPath = strDir + "\\" + strFileName + ".DAT";
                if (File.Exists(strPath))
                {
                    if ((File.GetAttributes(strPath) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                        File.SetAttributes(strPath, FileAttributes.Normal);
                    File.Delete(strPath);
                }
 */
                strPath = strDir + "\\small\\" + strFileName_NoExt + "_" + strExt + "_s.jpg";
                if (File.Exists(strPath))
                {
                    if ((File.GetAttributes(strPath) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                        File.SetAttributes(strPath, FileAttributes.Normal);
                    File.Delete(strPath);
                }

                strPath = strDir + "\\caption\\" + strFileName_NoExt + ".txt";
                if (File.Exists(strPath))
                {
                    if ((File.GetAttributes(strPath) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                        File.SetAttributes(strPath, FileAttributes.Normal);
                    File.Delete(strPath);
                }


                if (Directory.Exists(strDir + "\\small"))
                {
                    DirectoryInfo dii = new DirectoryInfo(strDir + "\\small");
                    FileInfo[] filearray = dii.GetFiles();
                    DirectoryInfo[] directarray = dii.GetDirectories();
                    if ((directarray.Count() == 0 && filearray.Count() == 1 && filearray[0].Name == "Thumbs.db") ||
                        (directarray.Count() == 0 && filearray.Count() == 0))
                    {
                        dii.Delete(true);
                    }
                }

                if (Directory.Exists(strDir + "\\caption"))
                {
                    DirectoryInfo dii = new DirectoryInfo(strDir + "\\caption");
                    FileInfo[] filearray = dii.GetFiles();
                    DirectoryInfo[] directarray = dii.GetDirectories();
                    if ((directarray.Count() == 0 && filearray.Count() == 1 && filearray[0].Name == "Thumbs.db") ||
                        (directarray.Count() == 0 && filearray.Count() == 0))
                    {
                        dii.Delete(true);
                    }
                }
                if (Directory.Exists(strDir))
                {
                    DirectoryInfo dii = new DirectoryInfo(strDir);
                    FileInfo[] filearray = dii.GetFiles();
                    DirectoryInfo[] directarray = dii.GetDirectories();
                    if ((directarray.Count() == 0 && filearray.Count() == 1 && filearray[0].Name == "Thumbs.db") ||
                        (directarray.Count() == 0 && filearray.Count() == 0))
                    {
                        dii.Delete(true);
                    }
                }

                String strFound = lst_PhotoDelPath.Find(delegate(String str) { return str == strDirUpperLevel; });
                if (strFound == null)
                    lst_PhotoDelPath.Add(strDirUpperLevel);
            }
            catch (Exception ex)
            {
                nRet = false;
            }

            return nRet;
        }

        public void DeletePushDir()
        {
            DataTable dtPushDir = new DataTable();
            dtPushDir.Clear();
            string sqlStr = "select source_path, keep_days_dir from push_directory where source_path <> ''";
            //
            try
            {
                using (SqlDataAdapter da = new SqlDataAdapter(sqlStr, strDbConnStr))
                {
                    da.Fill(dtPushDir);
                    SqlCommandBuilder Obuilder = new SqlCommandBuilder(da);
                }
                foreach (DataRow row in dtPushDir.Rows)
                {
                    string strSource = row["source_path"].ToString();
                    if (strSource.Trim() == "") continue;
                    int iKeepDays = Convert.ToInt32(row["keep_days_dir"]);

                    DateTime dtEnd = DateTime.Now.Date.AddDays(-iKeepDays);
                    DateTime dtStart = dtEnd.AddDays(-3000);
                    string strDelPath = "";
                    while (dtStart <= dtEnd)
                    {
                        strDelPath = strSource.Replace("%Y", dtStart.Year.ToString());
                        strDelPath = strDelPath.Replace("%y", dtStart.Year.ToString());
                        strDelPath = strDelPath.Replace("%M", dtStart.Month.ToString().PadLeft(2, '0'));
                        strDelPath = strDelPath.Replace("%m", dtStart.Month.ToString().PadLeft(2, '0'));
                        strDelPath = strDelPath.Replace("%D", dtStart.Day.ToString().PadLeft(2, '0'));
                        strDelPath = strDelPath.Replace("%d", dtStart.Day.ToString().PadLeft(2, '0'));
                        if (Directory.Exists(strDelPath))
                        {
                            try
                            {
                                Directory.Delete(strDelPath, true);
                            }
                            catch (Exception exp)
                            {
                                WriteToFile(" Delete Push Dir Fail: " + strDelPath + " " + exp.Message);
                                dtStart = dtStart.AddDays(1);
                                continue;
                            }
                            WriteToFile(" Delete Push Dir Success: " + strDelPath);
                        }

                        dtStart = dtStart.AddDays(1);
                    }
                }
                dtPushDir = null;
            }
            catch (Exception ex)
            {
                dtPushDir = null;
                return;
            }


        }
    
    }
}
