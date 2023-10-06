using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SqlClient;
using System.Data;
/*
 * All Database event can be handle in this class
*/
namespace Reuters_with_Web_Service
{
    public class DBHandler
    {
        String strConnection;
        SqlConnection myConnection;
        String strDataSource;
        String strInitialCatalog;
        String strSqlUserName ;
        String strSqlPassword;
        String strDBError;
        Boolean isUsingWindowsAuthentication;

        public DBHandler()
        {
            strDataSource = "";
            strInitialCatalog = "";
            strSqlUserName = "";
            strSqlPassword = "";
            strDBError = "";
            isUsingWindowsAuthentication = false;
        }

        public String GetNextPhotoID()
        {
            String strId = "", strSql = String.Format("declare @filecode nvarchar(13)\n "+
			"Exec GetImageFileName @filecode output \n"+
			"select @filecode");
            DataTable table = GetRecordToDataTable(strSql);
            try
            {
                strId = table.Rows[0].ItemArray[0].ToString();
            }
            catch (Exception ex)
            {
                strId = "";
            }
            return strId;
        }

        public String GetAgencyId(String strAgencyCode)
        {
            String strId = "", strSql = String.Format("select tele_id from news_agency_photo where tele_code=N'{0}'", strAgencyCode);
            DataTable table = GetRecordToDataTable(strSql);
            strId = table.Rows[0].ItemArray[0].ToString();
            return strId;
        }

        public DBHandler(String strConnection)
        {
            this.strConnection = strConnection;
            myConnection = new SqlConnection(strConnection);
        }

        public void PrePareConnStr(String strSqlUserName, String strSqlPassword, Boolean isUsingWindowsAuthentication)
        {
            //this.strDataSource = strDataSource;
            //this.strInitialCatalog = strInitialCatalog;
            this.strDataSource = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["DataSource"]);
            this.strInitialCatalog = Convert.ToString(System.Configuration.ConfigurationSettings.AppSettings["InitialCatalog"]);
            this.strSqlUserName = strSqlUserName;
            this.strSqlPassword = strSqlPassword;
            this.isUsingWindowsAuthentication = isUsingWindowsAuthentication;
            if (isUsingWindowsAuthentication)
                strConnection = "Data Source=" + strDataSource + ";Initial Catalog=" + strInitialCatalog + ";TRUSTED_CONNECTION=YES;";
            else
                strConnection = "Data Source=" + strDataSource + ";Initial Catalog=" + strInitialCatalog + ";User ID=" + strSqlUserName + ";Password=" + strSqlPassword;
        }

        public Boolean Connect()
        {   
            myConnection = new SqlConnection(strConnection);
            return OpenConnection();
        }

        public String GetConfig(String strEntryName)
        {
            String strResult = "", strSql = String.Format("select value from sys_newsroom where entry = N'{0}'", strEntryName);
            DataTable table = GetRecordToDataTable(strSql);
            try
            {
                strResult = table.Rows[0].ItemArray[0].ToString();
            }
            catch (Exception ex)
            { strResult = "";}
            return strResult;
        }

        public DataTable GetRecordToDataTable(String strSql)
        {
            DataTable table = new DataTable();
            try
            {
                if (myConnection.State == ConnectionState.Closed)
                    OpenConnection();
                // Edit Stephen 2022-10-06
                using (SqlCommand sqlCommand = myConnection.CreateCommand())
                {
                    sqlCommand.CommandText = strSql;
                    using (SqlDataAdapter adapter = new SqlDataAdapter(sqlCommand))
                    {
                        adapter.Fill(table);
                   //     CloseConnection();
                    }
                }
                // Edit Stephen 2022-10-06
                return table;
            }catch(Exception ex)
            {
                strDBError = "Error: " + ex.Message.Trim();
                CloseConnection();
            }
            return table;
        }

        public Boolean ExecuteSQL(String strSql)
        {
            try
            {
                if (myConnection.State == ConnectionState.Closed)
                    OpenConnection();
                // Edit Stephen 2022-10-06
                using (SqlCommand sqlCommand = myConnection.CreateCommand())
                {
                    sqlCommand.CommandText = strSql;
                    sqlCommand.ExecuteNonQuery();
                  //  CloseConnection();
                }
                // Edit Stephen 2022-10-06
            }
            catch (Exception ex)
            {
                strDBError = "Error: " + ex.Message.Trim();
                CloseConnection();
                return false;
            }
            return true;
        }

        public Boolean SpecicalHandleEMS4InsertPhoto(ADOPhotoObject photo)
        {
            try
            {
              SqlCommand command = new SqlCommand(
                "INSERT INTO wires_photo_src (tele_code,photo_name ,photo_time ,photo_title ,photo_size ,"+
                "photo_format ,photo_caption ,photo_location ,photo_flag ,photo_begin_time, photo_bitmap)  " +
                "Values(@tele_code,@photo_name ,@photo_time ,@photo_title ,@photo_size ," +
                "@photo_format ,@photo_caption ,@photo_location ,@photo_flag ,@photo_begin_time, @photo_bitmap)  ", myConnection);

              command.Parameters.Add("@tele_code",
                 SqlDbType.Char, 33).Value = photo.str_TeleCode;
              command.Parameters.Add("@photo_name", 
                  SqlDbType.Char, 250).Value = photo.str_PhotoName;
              command.Parameters.Add("@photo_time",
                  SqlDbType.DateTime).Value = photo.str_PhotoTime;
              command.Parameters.Add("@photo_title",
                  SqlDbType.Char,254).Value = photo.str_Title;
              command.Parameters.Add("@photo_size",
                    SqlDbType.Int).Value = photo.iSize;
              command.Parameters.Add("@photo_format",
                    SqlDbType.Char,3).Value = photo.str_Format;
              command.Parameters.Add("@photo_caption",
                    SqlDbType.Text).Value = photo.str_Caption;
              command.Parameters.Add("@photo_location",
                    SqlDbType.Char,20).Value = photo.str_Location;
              command.Parameters.Add("@photo_flag",
                     SqlDbType.Char,1).Value = photo.str_PhotoFlag;
              command.Parameters.Add("@photo_begin_time",
                       SqlDbType.Char,32).Value = photo.str_BeginTime;

              command.Parameters.Add("@photo_bitmap",
                  SqlDbType.Image, photo.photo_Bitmap.Length).Value = photo.photo_Bitmap;

              myConnection.Open();
              command.ExecuteNonQuery();
            }
            catch (Exception ex) { }
            return true;
        }

        public Boolean PrepareOnInsertWires(ADOOjectWires wires, String str_Table)
        {
            try
            {
                SqlCommand command = new SqlCommand(
                  "INSERT INTO " + str_Table + " ( tele_name , tele_time , tele_words , tele_content,tele_title, tele_location , tele_keyword,tele_urgent,tele_flag,tele_note , tele_language)  " +
                  "Values( @tele_name , @tele_time , @tele_words , @tele_content,@tele_title, @tele_location , @tele_keyword,@tele_urgent,@tele_flag,@tele_note , @tele_language)  ", myConnection);

                command.Parameters.Add("@tele_name",
                   SqlDbType.Char, 250).Value = wires.str_TeleName;
                command.Parameters.Add("@tele_time",
                    SqlDbType.DateTime).Value = wires.dt_TeleTime;
                command.Parameters.Add("@tele_words",
                    SqlDbType.Int).Value = wires.iwords;
                command.Parameters.Add("@tele_content",
                    SqlDbType.Text).Value = wires.str_Content;
                command.Parameters.Add("@tele_title",
                      SqlDbType.Char, 254).Value = wires.str_Title;
                command.Parameters.Add("@tele_keyword",
                      SqlDbType.Char, 19).Value = wires.str_Keyword;
                command.Parameters.Add("@tele_urgent",
                      SqlDbType.Char, 1).Value = wires.str_Urgent;
                command.Parameters.Add("@tele_flag",
                      SqlDbType.Char, 1).Value = wires.str_Flag;
                command.Parameters.Add("@tele_note",
                       SqlDbType.Char, 254).Value = wires.str_Note;
                command.Parameters.Add("@tele_language",
                         SqlDbType.Char, 6).Value = wires.str_Language;
                command.Parameters.Add("@tele_location",
                         SqlDbType.Char, 19).Value = wires.str_Location;

                myConnection.Open();
                command.ExecuteNonQuery();
            }catch(Exception ex){}
            return true;
        }


        public String GetConnectionStatus()
        {
            return myConnection.State.ToString();
        }
   
        public bool OpenConnection()
        {
            try
            {
                myConnection.Open();
                return true;
            }
            catch (Exception ex)
            {
                strDBError = ex.Message.Trim();
                return false;
            }
        }

        public void CloseConnection()
        {
            try
            {
                myConnection.Close();
            }
            catch (Exception ex) { strDBError = ex.Message.Trim(); }
        }

        public String GetDBError()
        {
            return strDBError;
        }

        public List<String> GetRootPathList()
        {
            List<String> lst_StrRoot = new List<string>();  
            String strFormat = "";
            String strResult = "", strSql = String.Format("select rootpath_id, rootpath from rootpath ");
            DataTable table = GetRecordToDataTable(strSql);
            try
            {
                for (int i = 0; i < table.Rows.Count; i++)
                {
                    strFormat = String.Format("{0};{1}", table.Rows[i].ItemArray[0].ToString(), table.Rows[i].ItemArray[1].ToString());
                    lst_StrRoot.Add(strFormat);
                }
            }
            catch (Exception ex)
            { strResult = ""; }
            return lst_StrRoot;
        }
    }
}
