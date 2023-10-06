using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Linq;
using System.Xml;

namespace Reuters_with_Web_Service
{
    class AP_XML_Reader:Reader
    {
        private String strContent;
        private String strHeadLine;
        private String strNewID;
        private int iPriority;
        private XmlDocument doc;
        private String errorMessage;
        private String strFileName;
        private List<String> lst_Subject;
        private DateTime dtNewsDateTime, dt_ProcessTime;
        private List<AP_News> lst_APNews;

        public AP_XML_Reader(String strFilePath)
        {
            lst_Subject = new List<String>();
            try
            {
                strFileName = strFilePath;
                FileInfo file = new FileInfo(strFilePath);
                if (file.Extension.Equals(".xml", StringComparison.OrdinalIgnoreCase))
                {
                    doc = new XmlDocument();
                    doc.Load(strFilePath);
                }
            }
            catch (Exception ex)
            {
                errorMessage = "";
            }
            lst_APNews = new List<AP_News>();
        }

        public Boolean Read()
        {
            //ReadHeadLine();
            String strResult = ReadContent();
            if (strResult == "ERROR")
            {
                errorMessage = "ERROR";
                return false;
            }
            if (strNewID == null)
            {
                ReadUpdateDate();
                return false;
            }
            return true;
        }

        public void SetProcessTime(DateTime time)
        {
            dt_ProcessTime = time;
        }

        public String GetErrorMessage()
        {
            return errorMessage;
        }

        public String GetNewsID()
        {
            return strNewID;
        }

        public DateTime GetNewsDateTime()
        {
            return dtNewsDateTime;
        }

        public String GetStrContent()
        {
            String strContent_temp;
            try
            {
                strContent_temp = strContent.Replace("'", "''");
            }
            catch (Exception ex) { return ""; }
            return strContent_temp;
        }

        public String GetStrHeadLine()
        {
            return strHeadLine;
        }

        public int GetPriority()
        {
            return iPriority;
        }

        public String GetXMLFileName()
        {
            return strFileName.Substring(strFileName.LastIndexOf("\\"));
        }

        public void SetNewsID(String strID)
        { }

        public List<String> GetSubjectList()
        { 
            return lst_Subject; 
        }

        private void ReadUpdateDate()
        {
            XmlNode main = doc.SelectSingleNode(".");
            foreach (XmlNode chi in main.ChildNodes)
            {
                if (chi.LocalName == "feed")
                {
                    foreach (XmlNode child in chi.ChildNodes)
                    {
                        if (child.LocalName == "updated")
                        {
                            dtNewsDateTime = DateTime.Parse(child.InnerText);
                        }
                    }
                }
            }
        }

        private String ReadContent()
        {
            String strResult = "";
            XmlNode main = doc.SelectSingleNode(".");
            foreach (XmlNode chi in main.ChildNodes)
            {
                if (chi.LocalName == "feed")
                {
                    foreach (XmlNode child in chi.ChildNodes)
                    {
                        if (child.LocalName == "entry")
                        {
                            AP_News news = new AP_News();
                            news.lst_Subject = new List<String>();
                            foreach (XmlNode child1 in child.ChildNodes)
                            {
                                if (child1.LocalName == "id")
                                {
                                    strNewID = child1.InnerText;
                                    news.strNewsID = child1.InnerText;
                                }
                                //if (child1.LocalName == "published")
                                //{
                                //    dtNewsDateTime = DateTime.Parse(child1.InnerText);
                                //    news.dt_Publish = DateTime.Parse(child1.InnerText);
                                //}
                                if (child1.LocalName == "updated")
                                {
                                    dtNewsDateTime = DateTime.Parse(child1.InnerText);
                                    news.dt_Update = DateTime.Parse(child1.InnerText);
                                }
                                if (child1.LocalName == "content")
                                {
                                    foreach (XmlNode child2 in child1.ChildNodes)
                                    {
                                        foreach (XmlNode child3 in child2.ChildNodes)
                                        {
                                            if (child3.LocalName == "body")
                                            {
                                                foreach (XmlNode child4 in child3.ChildNodes)
                                                {
                                                    if (child4.LocalName == "body.head")
                                                    {
                                                        foreach (XmlNode child5 in child4.ChildNodes)
                                                        {
                                                            foreach (XmlNode child6 in child5.ChildNodes)
                                                            {
                                                                if (child6.LocalName == "hl1")
                                                                {
                                                                    strHeadLine = child6.InnerXml;
                                                                    news.strNewsHeadLine = child6.InnerXml;
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if (child4.LocalName == "body.content")
                                                    {
                                                        foreach (XmlNode child5 in child4.ChildNodes)
                                                        {
                                                            strContent = child5.InnerXml;
                                                            strContent = strContent.Replace("<p>", "   \r\n");
                                                            strContent = strContent.Replace("<body>", ""); strContent = strContent.Replace("</body>", "");
                                                            strContent = strContent.Replace("\n", "\r\n");
                                                            strContent = strContent.Replace("</p>", "   \r\n");
                                                            strContent = strContent.Replace("<p />", "   \r\n");
                                                            if (strContent.IndexOf("<table>") > -1)
                                                            {
                                                                int iTableCount = 0, n = 0;
                                                                while ((n = strContent.IndexOf("<table>", n) + 1) != 0) 
                                                                    iTableCount++;
                                                                int iLastIndex = 0;
                                                                for (int i = 0; i < iTableCount; i++)
                                                                {
                                                                    try
                                                                    {
                                                                        String strTemp = "", strReplace = "";
                                                                        iLastIndex = strContent.IndexOf("<table>");
                                                                        String strTable = strContent.Substring(iLastIndex, strContent.IndexOf("</table>") - strContent.IndexOf("<table>") + "</table>".Length);
                                                                        String strNewTable = strTable;
                                                                        strTemp = strTable.Replace("<table>", "").Replace("</table>", "");
                                                                        List<String> lst_Tr = strTemp.Split(new string[] { "<tr>" }, StringSplitOptions.None).ToList();
                                                                        int iMax = 0;
                                                                        foreach (String s in lst_Tr)
                                                                        {
                                                                            if (s.Length == 0)
                                                                                continue;
                                                                            if (s.IndexOf("<td>") > -1)
                                                                            {
                                                                                strTemp = s.Substring(s.IndexOf("<td>"), s.IndexOf("</td>") - s.IndexOf("<td>"));
                                                                                strTemp = strTemp.Replace("<td>", "");
                                                                                strTemp = strTemp.Replace("</td>", "");
                                                                                if (iMax == 0 && strTemp.Length > 0)
                                                                                    iMax = strTemp.Length;
                                                                            }
                                                                        }
                                                                        for (int j = 0; j < iMax; j++)
                                                                        {
                                                                            strReplace += " ";
                                                                        }
                                                                        strNewTable = strNewTable.Replace("<tr><td></td>", strReplace);
                                                                        strNewTable = strNewTable.Replace("<table>", "");
                                                                        strNewTable = strNewTable.Replace("</table>", "");
                                                                        strNewTable = strNewTable.Replace("<td>", "");
                                                                        strNewTable = strNewTable.Replace("</td>", "");
                                                                        strNewTable = strNewTable.Replace("<tr>", "");
                                                                        strNewTable = strNewTable.Replace("</tr>", "   \r\n");
                                                                        strNewTable = strNewTable.Replace("<th>", "\r\n");
                                                                        strNewTable = strNewTable.Replace("</th>", "\r\n");
                                                                        strContent = strContent.Replace(strTable, strNewTable);
                                                                    }
                                                                    catch (Exception ex)
                                                                    {
                                                                        return "ERROR";
                                                                    }
                                                                }
                                                            }
                                                            strContent.Trim();
                                                            news.strNewsContent = strContent.Replace("'", "''");
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                if (child1.LocalName == "ContentMetadata")
                                {
                                    foreach (XmlNode child2 in child1.ChildNodes)
                                    {
                                        if (child2.LocalName == "Priority")
                                        {
                                            for(int i=0;i<child2.Attributes.Count;i++)
                                            {
                                                if (child2.Attributes[i].LocalName == "Numeric")
                                                {
                                                    iPriority = Int32.Parse(child2.Attributes[i].Value);
                                                    news.iPriority = Int32.Parse(child2.Attributes[i].Value);
                                                }
                                            }
                                        }

                                        if (child2.LocalName == "SubjectClassification")
                                        {
                                            bool bFound = false;
                                            for (int i = 0; i < child2.Attributes.Count; i++)
                                            {
                                                if (child2.Attributes[i].LocalName == "Authority")
                                                {
                                                    if (child2.Attributes[i].Value != "AP Category Code")
                                                    {
                                                        i = child2.Attributes.Count + 1;
                                                        continue;
                                                    }
                                                    else
                                                        bFound = true;
                                                }
                                                if (child2.Attributes[i].LocalName == "Value")
                                                {
                                                    if (bFound == true)
                                                    {
                                                        String strFind = news.lst_Subject.Find(
                                                        delegate(String str)
                                                        {
                                                            return str == child2.Attributes[i].Value;
                                                        }
                                                        );
                                                        if (strFind != null)
                                                        {
                                                            continue;
                                                        }
                                                        lst_Subject.Add(child2.Attributes[i].Value);
                                                        news.lst_Subject.Add(child2.Attributes[i].Value);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            //Check New Date
                            /*Some <published> value may contain future time.
                             The following checking is use to avoid that case*/
                            try
                            {
                                //if (news.dt_Update > news.dt_Publish)
                                //    news.dt_NewsDate = news.dt_Publish;
                                //else
                                news.dt_NewsDate = news.dt_Update;
                                if (news.dt_NewsDate > dt_ProcessTime)
                                    news.dt_NewsDate = dt_ProcessTime;
                            }
                            catch (Exception ex)
                            {
                                news.dt_NewsDate = dt_ProcessTime;
                            }
                            //
                            lst_APNews.Add(news);
                        }
                    }
                }
            }
            return strResult;
        }

        public List<AP_News> GetAPNews()
        {
            return lst_APNews;
        }

        //Pat 20160721 add bloomberg-----------------------------------------------
        public int GetWordCounts()
        {
            return 0;
        }
        public String GetUrgent()
        {
            return "N";
        }
        public PhotoInfo GetPhotoInfo()
        {
            return null;
        }
        public String GetStrCaption()
        {
            return "";
        }
        //-------------------------------------------------------------------------
    }
}

class AP_News
{
    public String strNewsID { get; set; }
    //public DateTime dt_Publish { get; set; }
    public DateTime dt_Update { get; set; }
    public DateTime dt_NewsDate { get; set; }
    public String strNewsContent { get; set; }
    public String strNewsHeadLine { get; set; }
    public int iPriority { get; set; }
    public List<String> lst_Subject { get; set; }
}