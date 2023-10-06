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
    class AP_EMS6_XML_Reader : Reader
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
        // Edit Stephen 2022-08-03
        private String strSeqNo;// Seq No for XML created by AP
        private bool bDoubleLineFeed;
        // Edit Stephen 2022-08-03


        public AP_EMS6_XML_Reader(String strFilePath) //strFilePath :can be file path or XML content in String
        {
            photoInfo = new PhotoInfo();
            photoInfo.strPhotoWidth = "";
            photoInfo.strPhotoHeight = "";
            photoInfo.strSourcePhotoName = "";
            photoInfo.strSmallPhotoName = "";
            photoInfo.strMiddlePhotoName = "";
            photoInfo.lstPhoto = new List<Photo>();
            // Edit Stephen 2022-08-03
            photoInfo.strHeadLine = "";
            // Edit Stephen 2022-08-03

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
            // Edit Stephen 2022-08-03
            strHeadLine = "";
            strSeqNo = "";
            bDoubleLineFeed = false;
            // Edit Stephen 2022-08-03
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

        public void SetDoubleLineFeed()
       {
           bDoubleLineFeed = true;
       }

        public String GetNewsID()
        {
            //return strId = strId.Replace(":", "__");

            // Edit Stephen 2022-08-03
//            if (strSeqNo != "")
//                return strId + "-" + strSeqNo;
//            else            
//                return strId;

//            return strFileName.ToLower().Replace(".xml", "").Replace("-item", "");
            return strFileName.ToLower().Replace(".xml", "");

            // Edit Stephen 2022-08-03
        }

        public AP_EMS6_XML_Reader(XDocument doc)
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


            if (strSeqNo.Length == 0)
            {
                errorMessage = "Read News Sequence NumberFail";
                return false;
            }


            if (strHeadLine.Length == 0)
                strHeadLine = "No Title";

//            ReadContent();

            if (strContent.Length == 0)
                strContent = "No Content";
/*            {
                errorMessage = "Read News Content Fail";
                return false;
            }
*/            
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
/*
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
*/
        public String GetXMLFileName()
        {
            return strFileName;
        }

/*
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
                           //newsitem
                           if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
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
                                                   dtNewsDateTime = DateTime.ParseExact(strParseDate, "yyyyMMddTHH:mm:ssZ", null, System.Globalization.DateTimeStyles.AssumeLocal);
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

                                                   try
                                                   {
                                                       if (eNodeItemMetaChild.Attribute("role").Value.IndexOf("cRole:source") > -1)
                                                       {
                                                           strPressType = eNodeItemMetaChild.Attribute("type").Value.Replace("apsourcetype:", "") + " ";
                                                           continue;
                                                       }
                                                   }
                                                   catch (Exception ex) { }

                                               }
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("altId", StringComparison.OrdinalIgnoreCase))
                                               {
                                                   if (eNodeItemMetaChild.Attribute("type").Value.ToString().Equals("ap:ItemId", StringComparison.OrdinalIgnoreCase))
                                                   {
                                                       strId = eNodeItemMetaChild.Value;
                                                       continue;
                                                   }
                                               }
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("altId", StringComparison.OrdinalIgnoreCase))
                                               {
                                                   if (eNodeItemMetaChild.Attribute("type").Value.ToString().Equals("ap:recordSequenceNumber", StringComparison.OrdinalIgnoreCase))
                                                   {
                                                       strSeqNo = eNodeItemMetaChild.Value;
                                                       continue;
                                                   }
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
                                                       if (strType.ToUpper() == "TEXT" || strType.ToUpper() == "PICTURE")
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

                                       if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentSet", StringComparison.OrdinalIgnoreCase))
                                       {
                                           IEnumerable<XElement> childNoteItemMetaList =
                                                from el in eNodeNewItemChild.Elements()
                                                select el;
                                           foreach (XElement eNodeItemMetaChild in childNoteItemMetaList)
                                           {
                                               try
                                               {
                                                   if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("inlineXML", StringComparison.OrdinalIgnoreCase))
                                                   {

                                                       foreach (XElement eNodeItemInlineChild in eNodeItemMetaChild.Elements())
                                                       {
                                                           if (eNodeItemInlineChild.Name.LocalName.ToString().Equals("Nitf", StringComparison.OrdinalIgnoreCase))
                                                           {
                                                               foreach (XElement eNodeItemNitfChild in eNodeItemInlineChild.Elements())
                                                               {
                                                                   if (eNodeItemNitfChild.Name.LocalName.ToString().Equals("Body", StringComparison.OrdinalIgnoreCase))
                                                                   {
                                                                       foreach (XElement eNodeItemBodyChild in eNodeItemNitfChild.Elements())
                                                                       {
                                                                           if (eNodeItemBodyChild.Name.LocalName.ToString().Equals("Body.Content", StringComparison.OrdinalIgnoreCase))
                                                                           {
                                                                               foreach (XElement eNodeItemBodyContentChild in eNodeItemBodyChild.Elements())
                                                                               {
                                                                                   //strContent = eNodeItemBodyContentChild.Value.ToString();
//                                                                                   var reader = eNodeItemBodyContentChild.CreateReader();
  //                                                                                 reader.MoveToContent();
    //                                                                               strContent = reader.ReadInnerXml();

                                                                                   
                                                                                   if (eNodeItemBodyContentChild.Name.LocalName.ToString().Equals("Block", StringComparison.OrdinalIgnoreCase))
                                                                                   {
                                                                                       foreach (XElement eNodeItemBlockChild in eNodeItemBodyContentChild.Elements())
                                                                                       {
                                                                                           var reader = eNodeItemBlockChild.CreateReader();
                                                                                           reader.MoveToContent();
                                                                                           strContent = strContent + reader.ReadInnerXml() + "\r\n";
                                                                                       }
                                                                                   }
 
                                                                               }
                                                                           }
                                                                       }
                                                                   }
                                                               }
                                                           }
                                                       }
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
*/


        public int ReadPriority()
        {

            try
            {
                XDocument new_doc = null;

                int ipos = strFilePath.LastIndexOf("--");
                string strNewPath = strFilePath.Substring(0, ipos) + "-item.xml";

                if (File.Exists(strNewPath))
                {
                    FileInfo file = new FileInfo(strNewPath);
                    if (file.Extension.Equals(".xml", StringComparison.OrdinalIgnoreCase))
                    {
                        new_doc = XDocument.Load(strNewPath);
                    }

                    IEnumerable<XElement> childList =
                    from el in new_doc.Root.Elements()
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
                                if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
                                {
                                    IEnumerable<XElement> childNoteNewItemList =
                                        from el in eNodeItemSetChild.Elements()
                                        select el;
                                    foreach (XElement eNodeNewItemChild in childNoteNewItemList)
                                    {
                                        if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentMeta", StringComparison.OrdinalIgnoreCase))
                                        {
                                            IEnumerable<XElement> childNoteContentSetList =
                                                from el in eNodeNewItemChild.Elements()
                                                select el;
                                            foreach (XElement eNodeContentSetChild in childNoteContentSetList)
                                            {
                                                if (eNodeContentSetChild.Name.LocalName.ToString().Equals("urgency", StringComparison.OrdinalIgnoreCase))
                                                {
                                                    return Convert.ToInt32(eNodeContentSetChild.Value);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                new_doc = null;
            }
            catch (Exception)
            {
                return 0;
            }

            return 0;
        }

        public void ReadXML()
        {
            lst_Subject.Clear();
            bParseDateError = false;
            strHeadLine = "";
            strPressType = "AP";
            try
            {
                IEnumerable<XElement> childList =
                     from el in doc.Root.Elements()
                     select el;
                foreach (XElement e in childList)
                {

                    if (e.Name.LocalName.ToString().Equals("Head", StringComparison.OrdinalIgnoreCase))
                    {
                        foreach (XElement eNodeItemHeadChild in e.Elements())
                        {
                            if (eNodeItemHeadChild.Name.LocalName.ToString().Equals("docdata", StringComparison.OrdinalIgnoreCase))
                            {
                                foreach (XElement eNodeItemDocDataChild in eNodeItemHeadChild.Elements())
                                {

                                    try
                                    {
                                        if (eNodeItemDocDataChild.Name.LocalName.ToString().Equals("date.issue", StringComparison.OrdinalIgnoreCase))
                                        {
                                            string strParseDate = eNodeItemDocDataChild.Attribute("norm").Value;
                                            if (strParseDate.IndexOf("+") > -1)
                                                strParseDate = strParseDate.Substring(0, strParseDate.IndexOf("+"));
                                            strParseDate = strParseDate.Replace("-", "");
                                            dtNewsDateTime = DateTime.ParseExact(strParseDate, "yyyyMMddTHHmmssZ", null, System.Globalization.DateTimeStyles.AssumeLocal);
                                        }


                                        if (eNodeItemDocDataChild.Name.LocalName.ToString().Equals("identified-content", StringComparison.OrdinalIgnoreCase))
                                        {
                                            foreach (XElement eNodeItemIdentifiedContentChild in eNodeItemDocDataChild.Elements())
                                            {
                                                if (eNodeItemIdentifiedContentChild.Attribute("type").Value == "apcategorycode" &&
//                                                    eNodeItemIdentifiedContentChild.Name == "classifier")
                                                    eNodeItemIdentifiedContentChild.Name.LocalName == "classifier")
                                                {
                                                    try
                                                    {
                                                        lst_Subject.Add(eNodeItemIdentifiedContentChild.Attribute("value").Value);
                                                    }
                                                    catch (Exception ex) { }

                                                }
                                            }
                                        }
                                    
                                    }
                                    catch (Exception ex)
                                    {
                                        bParseDateError = true;
                                        return;
                                    }
                                }
                            }
                        }
                    }

                    if (e.Name.LocalName.ToString().Equals("Body", StringComparison.OrdinalIgnoreCase))
                    {
                        foreach (XElement eNodeItemBodyChild in e.Elements())
                        {

                            if (eNodeItemBodyChild.Name.LocalName.ToString().Equals("Body.Head", StringComparison.OrdinalIgnoreCase))
                            {
                                foreach (XElement eNodeItemBodyHeadContentChild in eNodeItemBodyChild.Elements())
                                {

                                    if (eNodeItemBodyHeadContentChild.Name.LocalName.ToString().Equals("hedline", StringComparison.OrdinalIgnoreCase))
                                    {
                                        foreach (XElement eNodeItemHeadLineChild in eNodeItemBodyHeadContentChild.Elements())
                                        {
                                            if (eNodeItemHeadLineChild.Attribute("id").Value == "headline")
                                            {
                                                if (strHeadLine != "") 
                                                    strHeadLine = strHeadLine + "\r\n";
                                                strHeadLine = strHeadLine + eNodeItemHeadLineChild.Value;
                                                strHeadLine = strHeadLine.Replace("amp;", "");
                                            }
                                        }
                                    }

                                }
                            }
                            
                            
                            if (eNodeItemBodyChild.Name.LocalName.ToString().Equals("Body.Content", StringComparison.OrdinalIgnoreCase))
                            {
                                foreach (XElement eNodeItemBodyContentChild in eNodeItemBodyChild.Elements())
                                {
                                    //strContent = eNodeItemBodyContentChild.Value.ToString();
                                    //                                                                                   var reader = eNodeItemBodyContentChild.CreateReader();
                                    //                                                                                 reader.MoveToContent();
                                    //                                                                               strContent = reader.ReadInnerXml();


                                    if (eNodeItemBodyContentChild.Name.LocalName.ToString().Equals("Block", StringComparison.OrdinalIgnoreCase))
                                    {
                                        foreach (XElement eNodeItemBlockChild in eNodeItemBodyContentChild.Elements())
                                        {
                                            var reader = eNodeItemBlockChild.CreateReader();
                                            reader.MoveToContent();
                                            string strTempContent = reader.ReadInnerXml();
                                           // string strTemputerContent = reader.ReadString();

 //                                           if (strTempContent.IndexOf("<table>") > -1)
                                            if (eNodeItemBlockChild.Name.LocalName == "table" || strTempContent.IndexOf("<table>") > -1)
                                            {
                                                    try
                                                    {
                                                        string strTable = "";
                                                        foreach (XElement eNodeItemTableChild in eNodeItemBlockChild.Elements())
                                                        {
                                                            foreach (XElement eNodeItemTableHeadChild in eNodeItemTableChild.Elements())
                                                            {
                                                                if (eNodeItemTableHeadChild.Name.LocalName == "tr")
                                                                {
                                                                    strTable = strTable + "<tr>";
                                                                    foreach (XElement eNodeItemTableHeadItemChild in eNodeItemTableHeadChild.Elements())
                                                                    {
                                                                        if (eNodeItemTableHeadItemChild.Name.LocalName == "th")
                                                                        {
                                                                            strTable = strTable + "<th>" + (String.IsNullOrEmpty(eNodeItemTableHeadItemChild.Value)?" ":eNodeItemTableHeadItemChild.Value) + "</th>";
                                                                        }
                                                                        else if (eNodeItemTableHeadItemChild.Name.LocalName == "td")
                                                                        {
                                                                            strTable = strTable + "<td>" + (String.IsNullOrEmpty(eNodeItemTableHeadItemChild.Value)? " " : eNodeItemTableHeadItemChild.Value) + "</td>";
                                                                        }
                                                                    }
                                                                    strTable = strTable + "</tr>";
                                                                }
                                                            }
                                                        }

                                                        
                                                        String strTemp = "", strReplace = "";
                                                        //iLastIndex = strTempContent.IndexOf("<table>");
                                                        //String strTable = strTempContent.Substring(iLastIndex, strTempContent.IndexOf("</table>") - strTempContent.IndexOf("<table>") + "</table>".Length);
                                                        //String strTable = strTempTable;
                                                        //String strNewTable = strTable;
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
                                                        strTable = strTable.Replace("<tr><td></td>", strReplace);
                                                        strTable = strTable.Replace("<table>", "");
                                                        strTable = strTable.Replace("</table>", "");
                                                        strTable = strTable.Replace("<td>", "");
                                                        strTable = strTable.Replace("</td>", " ");
                                                        strTable = strTable.Replace("<tr>", "");
                                                        strTable = strTable.Replace("</tr>", " \r\r\n");
                                                        strTable = strTable.Replace("<th>", "");
                                                        strTable = strTable.Replace("</th>", " ");
                                                     //   strTempContent = strTempContent.Replace(strTable, strTable);
                                                        strTempContent = strTable;
                                                    }
                                                    catch (Exception ex)
                                                    {
                                                        bParseDateError = true;
                                                        return;
                                                    }
                                            }

                                            strTempContent = strTempContent.Replace("<p>", "   \r\n");
                                            strTempContent = strTempContent.Replace("<body>", ""); 
                                            strTempContent = strTempContent.Replace("</body>", "");
                                           // strTempContent = strTempContent.Replace("\n", "\r\n");
                                            strTempContent = strTempContent.Replace("</p>", "   \r\n");
                                            strTempContent = strTempContent.Replace("<p />", "   \r\n");
                                            strTempContent = strTempContent.Replace("—", "-");
                                            strTempContent = strTempContent.Replace("–", "-");
                                            strTempContent = strTempContent.Replace("“","\"");
                                            strTempContent = strTempContent.Replace("”","\"");
                                            
                                            strTempContent.Trim();
                                            
                                            if (bDoubleLineFeed)
                                                strContent = strContent + strTempContent + " \r\r\n\r\r\n";
                                            else
                                                strContent = strContent + strTempContent + " \r\r\n";
                                        }
                                    }

                                }
                            }
                        }
                    }

                    if (e.Name.LocalName.ToString().Equals("itemSet", StringComparison.OrdinalIgnoreCase))
                    {
                        String strNodeValue = e.ToString();
                        IEnumerable<XElement> childNoteItemSetList =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeItemSetChild in childNoteItemSetList)
                        {
                            if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
                            {
                                IEnumerable<XElement> childNoteNewItemList =
                                    from el in eNodeItemSetChild.Elements()
                                    select el;
                                foreach (XElement eNodeNewItemChild in childNoteNewItemList)
                                {
                                    if (eNodeNewItemChild.Name.LocalName.ToString().Equals("itemMeta", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentSetList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentSetChild in childNoteContentSetList)
                                        {
                                            if (eNodeContentSetChild.Name.LocalName.ToString().Equals("versionCreated", StringComparison.OrdinalIgnoreCase))
                                            {
                                                string strParseDate = eNodeContentSetChild.Value;
                                                if (strParseDate.IndexOf("+") > -1)
                                                    strParseDate = strParseDate.Substring(0, strParseDate.IndexOf("+"));
                                                strParseDate = strParseDate.Replace("-", "");
                                                dtNewsDateTime = DateTime.ParseExact(strParseDate, "yyyyMMddTHH:mm:ssZ", null, System.Globalization.DateTimeStyles.AssumeLocal);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                
                
                }


                if (strFileName.IndexOf("_caption", StringComparison.OrdinalIgnoreCase) < 0 )
//                if (strHeadLine != "")
                    strType = "TEXT";
                else
                    strType = "PICTURE";

                strId = "-";
                strSeqNo = "-";

            }
            catch (Exception ex)
            {

            }
        }


        public String GetErrorMessage()
        {
            return errorMessage;
        }

        public int GetPriority()
        {
//            return iPriority;
            return ReadPriority();
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
//                strCaption_temp = strCaption.Replace("'", "''");
                strCaption_temp = strContent.Replace("'", "''");
            }
            catch (Exception ex) { return ""; }
            return strCaption_temp;
        }

        public String GetStrHeadLine()
        {
            String strHeadLine_temp;
            try
            {
                if (strType == "PICTURE")
                    strHeadLine_temp = strContent.Replace("'", "''");
                else
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
            try
            {
                int ipos = strFilePath.LastIndexOf("\\");
                string strDir = strFilePath.Substring(0, ipos);

                ipos = strFileName.LastIndexOf("--");
                string strNameHead = strFileName.Substring(0, ipos);

                var files = Directory.EnumerateFiles(strDir, strNameHead + "*.*", SearchOption.AllDirectories)
                            .Where(s => s.EndsWith(".jpeg") || s.EndsWith(".jpg") || s.EndsWith(".eps"));

                foreach (string filePath in files)
                {
                    if (filePath.IndexOf("_small.") > 0) continue;
                    if (filePath.IndexOf("_mid.") > 0) continue;

                    //                photoInfo.strPhotoSourcePath = filePath;
                    //                photoInfo.strPhotoSmallPath = filePath.Replace(".","_small.");
                    ipos = filePath.LastIndexOf("\\");
                    photoInfo.strSourcePhotoName = filePath.Substring(ipos + 1);
                    photoInfo.strSmallPhotoName = photoInfo.strSourcePhotoName.Replace(".", "_small.");
                    photoInfo.strMiddlePhotoName = photoInfo.strSourcePhotoName.Replace(".", "_mid.");

                    break;
                }

                return photoInfo;
            }
            catch (Exception ex)
            {
                return null;

            }
        }

    }
}
