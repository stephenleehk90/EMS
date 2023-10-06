using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Xml.Linq;
using System.Text.RegularExpressions;

namespace Reuters_with_Web_Service
{
    class GIS_XML_Reader:Reader
    {
        private String strContent;
        private String strHeadLine;
        private String strCaption;
        private String strLanguage;
        private String strPressType;
        private MediaMeta meta;
        private String errorMessage;
        private String strId;// News ID for XML created by Reuters
        private bool isUrgent;// I dentify the news is urgent or not by reading the <Priority> tab in the XML
        private DateTime dtNewsDateTime;//** The time read in XML is UTC format. So when using the news time, must be convert it the local time
        private String strFileName;
        private String strFilePath;
        private String strType;
        private XDocument doc;
        private Boolean bIsXMLFile;
        private int iPriority;
        private List<String> lst_Subject;
        private PhotoInfo photoInfo;
        private bool bParseDateError;

        public GIS_XML_Reader(String strFilePath) //strFilePath :can be file path or XML content in String
        {
            meta = new MediaMeta();
            errorMessage = "";
            strCaption = "";
            strType = "";
            strContent = "";
            strPressType = "";
            strLanguage = "";
            isUrgent = false;
            bIsXMLFile = false;
            dtNewsDateTime = DateTime.MinValue;
            iPriority = 0;
            strId = "";
            bParseDateError = false;
            lst_Subject = new List<String>();
            try
            {
                int iPos = strFilePath.LastIndexOf("\\");
                if (iPos!=-1)
                    strFileName = strFilePath.Substring(iPos+1);

                FileInfo file = new FileInfo(strFilePath);
                if (file.Extension.Equals(".xml", StringComparison.OrdinalIgnoreCase))
                {
                    doc = XDocument.Load(strFilePath);
                    bIsXMLFile = true;
                }
            }
            catch (Exception ex)
            {
                errorMessage = "";
            }
            this.strFilePath = strFilePath;
        }

        public void SetNewsID(String strId)
        {
            this.strId = strId;
        }

        public String GetNewsID()
        {
            //return strId = strId.Replace(":", "__");
            return strId;
        }

        public GIS_XML_Reader(XDocument doc)
        {
            this.doc = doc;
        }

        public String GetXMLNewsType()
        {
            return strType;
        }

        public String GetXMLLanguage()
        {
            return strLanguage;
        }

        public List<String> GetSubjectList()
        {
            return lst_Subject;
        }

        public String GetClass()
        {
            string strClass = "Normal";
	        if (strPressType == "WR")	// WR
		        strClass = "WR & API";
	        else if (strHeadLine.ToLower().StartsWith("air pollution index") || strHeadLine.StartsWith("空氣污染指數") || strHeadLine.ToUpper().StartsWith("API"))	// API
		        strClass = "WR & API";	
	        else
		        strClass = "Normal";

            if (strLanguage.ToLower().StartsWith("en"))
		        strClass += "(English)";
            return strClass;
        }

        public Boolean Read()
        {
            ReadXML();
            if (strType.Length == 0)
            {
                errorMessage = "Selected XML format does not match";
                return false;
            }

            if (bParseDateError)
            {
                errorMessage = "Read News Date Time Error";
                return false;
            }

            if (strId.Length == 0)
            {
                errorMessage = "Read News ID Fail";
                return false;
            }

            if (strHeadLine.Length == 0)
                strHeadLine = "No Title";

            ReadContent();

            if (strContent.Length == 0)
            {
                errorMessage = "Read News Content Fail";
                return false;
            }
            
            return true;
        }

        public DateTime GetNewsDateTime()
        {
            return dtNewsDateTime;
        }

        public String GetUrgent()
        {
            if (isUrgent)
                return "Y";
            return "N";
        }

        private void ReadContent()
        {
            try
            {
                int iPos = strFilePath.LastIndexOf("\\");
                string strTxtFilePath = strFilePath.Substring(0, iPos);
                strTxtFilePath = strTxtFilePath + "\\txt\\" + strId + ".txt";
                strContent = File.ReadAllText(strTxtFilePath);
            }
            catch (Exception ex)
            {

            }
        }

        public String GetXMLFileName()
        {
            return strFileName;
        }

        public void ReadXML()
        {
            bParseDateError = false;
            try
            {
               IEnumerable<XElement> childList =
                    from el in doc.Root.Elements()
                    select el;
               foreach (XElement e in childList)
               {
                   if (e.Name.LocalName.ToString().Equals("itemSet", StringComparison.OrdinalIgnoreCase))
                   {
                       String strNodeValue = e.ToString();
                       IEnumerable<XElement> childNoteItemSetList =
                           from el in e.Elements()
                           select el;
                       foreach (XElement eNodeItemSetChild in childNoteItemSetList)
                       {
                           //packageitem
                           if (eNodeItemSetChild.Name.LocalName.ToString().Equals("packageItem", StringComparison.OrdinalIgnoreCase))
                           {
                               IEnumerable<XElement> childNoteNewItemList =
                                   from el in eNodeItemSetChild.Elements()
                                   select el;
                               foreach (XElement eNodeNewItemChild in childNoteNewItemList)
                               {
                                   if (eNodeNewItemChild.Name.LocalName.ToString().Equals("itemMeta", StringComparison.OrdinalIgnoreCase))
                                   {
                                       IEnumerable<XElement> childNoteItemMetaList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                       foreach (XElement eNodeItemMetaChild in childNoteItemMetaList)
                                       {
                                           try
                                           {
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("versionCreated", StringComparison.OrdinalIgnoreCase))
                                               {
                                                   string strParseDate = eNodeItemMetaChild.Value;
                                                   if (strParseDate.IndexOf("+") > -1)
                                                       strParseDate = strParseDate.Substring(0, strParseDate.IndexOf("+"));
                                                   //                                               dtNewsDateTime = DateTime.Parse(eNodeItemMetaChild.Value);
                                                   strParseDate = strParseDate.Replace("-", "");
                                                   dtNewsDateTime = DateTime.ParseExact(strParseDate, "yyyyMMddTHH:mm:ss.fff", null, System.Globalization.DateTimeStyles.AssumeLocal);
                                               }
                                           }
                                           catch (Exception ex) 
                                           {
                                               bParseDateError = true;
                                               return;
                                           }
                                       }
                                   }

                                   if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentMeta", StringComparison.OrdinalIgnoreCase))
                                   {
                                       IEnumerable<XElement> childNoteItemMetaList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                       foreach (XElement eNodeItemMetaChild in childNoteItemMetaList)
                                       {
                                           try
                                           {
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("language", StringComparison.OrdinalIgnoreCase))
                                                   strLanguage = eNodeItemMetaChild.Attribute("tag").Value;
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("infoSource", StringComparison.OrdinalIgnoreCase))
                                               {
                                                  string strLiteral = eNodeItemMetaChild.Attribute("literal").Value;
                                                  if (strLiteral == "prtype")
                                                      strPressType = eNodeItemMetaChild.Value;
                                                  if (strLiteral == "refno")
                                                      strId = eNodeItemMetaChild.Value;
                                               }
                                           }
                                           catch (Exception ex) { }
                                       }
                                   }
                               
                               
                               }
                           }
                           //newsitem
                           if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
                           {
                               IEnumerable<XElement> childNoteNewItemList =
                                   from el in eNodeItemSetChild.Elements()
                                   select el;

                               bool b_Is_Text_File = false;
                               foreach (XElement eNodeNewItemChild in childNoteNewItemList)
                               {
                                   if (eNodeNewItemChild.Name.LocalName.ToString().Equals("itemMeta", StringComparison.OrdinalIgnoreCase))
                                   {
                                       IEnumerable<XElement> childNoteItemMetaList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                       foreach (XElement eNodeItemMetaChild in childNoteItemMetaList)
                                       {
                                           try
                                           {
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("itemClass", StringComparison.OrdinalIgnoreCase))
                                               {
                                                   if (eNodeItemMetaChild.Attribute("qcode").Value.IndexOf("ninat:") > -1)
                                                   {
                                                       strType = eNodeItemMetaChild.Attribute("qcode").Value.Replace("ninat:", "");
                                                       if (strType.ToUpper() == "TEXT")
                                                           b_Is_Text_File = true;
                                                   }
                                               }
                                           }
                                           catch (Exception ex) { }
                                       }
                                   }
                               }

                               if (b_Is_Text_File)
                               {
                                   foreach (XElement eNodeNewItemChild in childNoteNewItemList)
                                   {

                                       if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentMeta", StringComparison.OrdinalIgnoreCase))
                                       {
                                           IEnumerable<XElement> childNoteItemMetaList =
                                                from el in eNodeNewItemChild.Elements()
                                                select el;
                                           foreach (XElement eNodeItemMetaChild in childNoteItemMetaList)
                                           {
                                               try
                                               {
                                                   if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("language", StringComparison.OrdinalIgnoreCase))
                                                       strLanguage = eNodeItemMetaChild.Attribute("tag").Value;
                                                   if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("headline", StringComparison.OrdinalIgnoreCase))
                                                   {
                                                       strHeadLine = eNodeItemMetaChild.Value;
                                                       strHeadLine = strHeadLine.Replace("amp;", "");
                                                   }
                                               }
                                               catch (Exception ex) { }
                                           }
                                       }


                                   }
                               }
                           }
                       
                       
                       }
                   }
               }
           }catch(Exception ex)
           {

           }
        }

        public String GetErrorMessage()
        {
            return errorMessage;
        }

        public int GetPriority()
        {
            return iPriority;
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

        public int GetWordCounts()
        {
            try{
                MatchCollection collection = Regex.Matches(strContent, @"[\S]+");
                return collection.Count;
            }
            catch (Exception ex) { return 0; }
            
        }

        public String GetStrCaption()
        {
            String strCaption_temp;
            try
            {
                strCaption_temp = strCaption.Replace("'", "''");
            }
            catch (Exception ex) { return ""; }
            return strCaption_temp;
        }

        public String GetStrHeadLine()
        {
            String strHeadLine_temp;
            try
            {
                strHeadLine_temp = strHeadLine.Replace("'", "''");
            }
            catch (Exception ex) { return ""; }
            return strHeadLine_temp;
        }

        public MediaMeta GetMediaMeta()
        {
            return meta;
        }

        public PhotoInfo GetPhotoInfo()
        {
            return photoInfo;
        }


    }
}
