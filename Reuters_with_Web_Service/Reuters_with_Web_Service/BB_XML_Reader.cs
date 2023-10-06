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
    class BB_XML_Reader:Reader
    {
        private String strContent;
        private String strHeadLine;
        private String strType; //Text (TXT), Picture (PIX), Graphic (NGS), Video (???)
        private String strCaption;
        private MediaMeta meta;
        private PhotoInfo photoInfo;
        private String strFilePath;
        private String errorMessage;
        private String strId;// News ID for XML created by Reuters
        private bool isUrgent;// I dentify the news is urgent or not by reading the <Priority> tab in the XML
        private int iWordCount;
        private DateTime dtNewsDateTime;//** The time read in XML is UTC format. So when using the news time, must be convert it the local time
        private String strFileName;
        private Boolean isFromReuters;// Identify the source is from file or from reuters web service
        private XDocument doc;
        private Boolean bIsXMLFile;
        private int iPriority;
        private String strNewsCategory;// Not useful now.
        private List<String> lst_Subject;
        private String strFilePhyName;

        public BB_XML_Reader(String strFilePath, Boolean isFromReuters) //strFilePath :can be file path or XML content in String
        {
            strFilePhyName = "";
            photoInfo = new PhotoInfo();
            photoInfo.strPhotoWidth = "";
            photoInfo.strPhotoHeight = "";
            meta = new MediaMeta();
            errorMessage = "";
            strCaption = "";
            strType = "";
            strContent = "";
            isUrgent = false;
            this.isFromReuters = isFromReuters;
            bIsXMLFile = false;
            dtNewsDateTime = DateTime.MinValue;
            iWordCount = 0;
            iPriority = 0;
            strId = "";
            strNewsCategory = "";
            lst_Subject = new List<String>();
            if (isFromReuters)
            {
                doc = XDocument.Parse(strFilePath);
            }
            else
            {
                try
                {
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
            //return strId;
            return strFilePhyName.Replace(".xml","");//Pat 20160812 file name being ID
        }

        public BB_XML_Reader(XDocument doc)
        {
            this.doc = doc;
        }

        public String GetXMLNewsType()
        {
            return strType;
        }

        public String GetNewsCategory()
        {
            return strNewsCategory;
        }

        public List<String> GetSubjectList()
        {
            return lst_Subject;
        }

        public Boolean Read()
        {
            ReadXMLFileType();
            if (strType.Length == 0)
            {
                errorMessage = "Selected XML format does not match";
                return false;
            }
            if (strType.Equals("text", StringComparison.OrdinalIgnoreCase))
            {
                ReadHeadLine();
                ReadContent();
            }
            else
            {
                //Pat only for text in bloomberg
                errorMessage = "Only support Text Type XML";
                return false;
                ReadHeadLine();
                ReadCaptionAndMeta();
                ReadAllPhotoName();
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
                IEnumerable<XElement> childList =
                from el in doc.Root.Elements()
                select el;
                foreach (XElement e in childList)
                {
                    if (e.Name.LocalName.ToString().Equals("NewsItem", StringComparison.OrdinalIgnoreCase))
                    {
                        String strNodeValue = e.ToString();
                        IEnumerable<XElement> childNoteNewsItemList =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeNewsItemChild in childNoteNewsItemList)
                        {
                            if (eNodeNewsItemChild.Name.LocalName.ToString().Equals("NewsComponent", StringComparison.OrdinalIgnoreCase))
                            {
                                IEnumerable<XElement> childNoteNewsComponent1List =
                                    from el in eNodeNewsItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeNewsComponent1Child in childNoteNewsComponent1List)
                                {
                                    if (eNodeNewsComponent1Child.Name.LocalName.ToString().Equals("NewsComponent", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteNewsComponent2List =
                                            from el in eNodeNewsComponent1Child.Elements()
                                            select el;
                                        foreach (XElement eNodeNewsComponent2Child in childNoteNewsComponent2List)
                                        {
                                            if (eNodeNewsComponent2Child.Name.LocalName.ToString().Equals("ContentItem", StringComparison.OrdinalIgnoreCase))
                                            {
                                                IEnumerable<XElement> childNoteContentItemList =
                                                    from el in eNodeNewsComponent2Child.Elements()
                                                    select el;
                                                foreach (XElement eNodeContentItemChild in childNoteContentItemList)
                                                {
                                                    if (eNodeContentItemChild.Name.LocalName.ToString().Equals("DataContent", StringComparison.OrdinalIgnoreCase))
                                                    {
                                                        IEnumerable<XElement> childNoteDataContentList =
                                                           from el in eNodeContentItemChild.Elements()
                                                           select el;
                                                        foreach (XElement eNodeDataContentChild in childNoteDataContentList)
                                                        {
                                                            //strContent = eNodeInlineXMLChild.Value.ToString();
                                                            strContent = eNodeDataContentChild.ToString();
                                                            strContent = strContent.Substring(strContent.IndexOf("<body>"), strContent.LastIndexOf("</body>") - strContent.IndexOf("<body>"));
                                                            strContent = strContent.Replace("<p>", "");
                                                            strContent = strContent.Replace("<body>", ""); strContent = strContent.Replace("</body>", "");
                                                            strContent = strContent.Replace("\n", "\r\n");
                                                            strContent = strContent.Replace("</p>", "   \r\n");
                                                            strContent = strContent.Replace("<p />", "   \r\n");
                                                            strContent.Trim();
                                                        }//for DataContent
                                                    }//if DataContent
                                                }//for ContentItem
                                            }//if ContentItem
                                        }//for NewsComponent2
                                    }//if NewsComponent2
                                }//for NewsComponent1
                            }//if NewsComponent1
                        }//for NewsItem
                    }//if NewsItem
                }//for All
            }
            catch (Exception ex)
            {

            }
        }

        public String GetXMLFileName()
        {
            if (!strFilePhyName.Equals(strFileName))
                return strFilePhyName;
            else
                return strFileName;
        }

        private void ReadHeadLine()
        {
            try
            {
                IEnumerable<XElement> childList =
                from el in doc.Root.Elements()
                select el;
                foreach (XElement e in childList)
                {
                    if (e.Name.LocalName.ToString().Equals("NewsItem", StringComparison.OrdinalIgnoreCase))
                    {
                        String strNodeValue = e.ToString();
                        IEnumerable<XElement> childNoteNewsItemList =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeNewsItemChild in childNoteNewsItemList)
                        {
                            if (eNodeNewsItemChild.Name.LocalName.ToString().Equals("NewsManagement", StringComparison.OrdinalIgnoreCase))
                            {
                                IEnumerable<XElement> childNoteNewsManagementList =
                                    from el in eNodeNewsItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeNewsManagementChild in childNoteNewsManagementList)
                                {
                                    if (eNodeNewsManagementChild.Name.LocalName.ToString().Equals("Urgency", StringComparison.OrdinalIgnoreCase))
                                    {
                                        String strUrgency = eNodeNewsManagementChild.Attribute("FormalName").Value;
                                        try
                                        {
                                            if (Int32.Parse(strUrgency) <= 2)
                                                isUrgent = true;
                                        }
                                        catch (Exception ex) 
                                        { 
                                            isUrgent = false; 
                                        }
                                    }
                                }//for NewsManagement
                            }//if NewsManagement
                            if (eNodeNewsItemChild.Name.LocalName.ToString().Equals("NewsComponent", StringComparison.OrdinalIgnoreCase))
                            {
                                IEnumerable<XElement> childNoteNewsComponent1List =
                                    from el in eNodeNewsItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeNewsComponent1Child in childNoteNewsComponent1List)
                                {
                                    if (eNodeNewsComponent1Child.Name.LocalName.ToString().Equals("NewsLines", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteNewsLinesList =
                                            from el in eNodeNewsComponent1Child.Elements()
                                            select el;                                        
                                        String strTempSlugLine = "";
                                        String strTempHeadLine = "";
                                        foreach (XElement eNodeNewsLinesChild in childNoteNewsLinesList)
                                        {
                                            if (eNodeNewsLinesChild.Name.LocalName.ToString().Equals("HeadLine", StringComparison.OrdinalIgnoreCase))
                                            {
                                                strTempHeadLine = eNodeNewsLinesChild.Value;
                                            }
                                            if (eNodeNewsLinesChild.Name.LocalName.ToString().Equals("SlugLine", StringComparison.OrdinalIgnoreCase))
                                            {
                                                strTempSlugLine = eNodeNewsLinesChild.Value;
                                            }
                                            //Pat 20160725 Cannot find
                                            //if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("subject", StringComparison.OrdinalIgnoreCase))
                                            //{
                                            //    try
                                            //    {
                                            //        lst_Subject.Add(eNodeContentMetaChild.Attribute("qcode").Value);
                                            //    }
                                            //    catch (Exception ex) { }
                                            //}
                                        }//for NewsLines
                                        if (strTempHeadLine.Equals("") && strTempSlugLine.Equals(""))
                                        {
                                        }
                                        else
                                        {
                                            strHeadLine = strTempHeadLine;
                                        }
                                    }//if NewsLines
                                }//for NewsComponent
                            }//if NewsComponent
                        }//for NewsItem
                    }//if NewsItem
                }//for all
            }
            catch (Exception ex)
            {

            }
        }

        public void ReadXMLFileType()
        {
            try
            {
               IEnumerable<XElement> childList =
                    from el in doc.Root.Elements()
                    select el;
               foreach (XElement e in childList)
               {
                   //Pat 20160725 Cannot find
                   //if (e.Name.LocalName.ToString().Equals("header", StringComparison.OrdinalIgnoreCase))
                   //{
                   //    String strNodeValue = e.ToString();
                   //    IEnumerable<XElement> childNoteHeaderList =
                   //        from el in e.Elements()
                   //        select el;
                   //    foreach (XElement eNodeHeaderChild in childNoteHeaderList)
                   //    {
                   //        if (eNodeHeaderChild.Name.LocalName.ToString().Equals("transmitId", StringComparison.OrdinalIgnoreCase) && strId.Length == 0)// capture News ID created by Reuters
                   //            strId = eNodeHeaderChild.Value;
                   //        if (eNodeHeaderChild.Name.LocalName.ToString().Equals("priority", StringComparison.OrdinalIgnoreCase) && iPriority == 0)// Will be used to idetify the urgency level
                   //            iPriority = Int32.Parse(eNodeHeaderChild.Value);
                   //    }
                   //}
                   if (e.Name.LocalName.ToString().Equals("NewsItem", StringComparison.OrdinalIgnoreCase))
                   {
                       String strNodeValue = e.ToString();
                       IEnumerable<XElement> childNoteNewsItemList =
                           from el in e.Elements()
                           select el;
                       foreach (XElement eNodeNewsItemChild in childNoteNewsItemList)
                       {
                           if (eNodeNewsItemChild.Name.LocalName.ToString().Equals("Identification", StringComparison.OrdinalIgnoreCase))
                           {
                               IEnumerable<XElement> childNoteIdentificationList =
                                   from el in eNodeNewsItemChild.Elements()
                                   select el;
                               foreach (XElement eNodeIdentificationChild in childNoteIdentificationList)
                               {
                                   if (eNodeIdentificationChild.Name.LocalName.ToString().Equals("NewsIdentifier", StringComparison.OrdinalIgnoreCase))
                                   {
                                       IEnumerable<XElement> childNoteNewsIdentifierList =
                                           from el in eNodeIdentificationChild.Elements()
                                           select el;
                                       foreach (XElement eNodeNewsIdentifierChild in childNoteNewsIdentifierList)
                                       {
                                           if (eNodeNewsIdentifierChild.Name.LocalName.ToString().Equals("NewsItemId", StringComparison.OrdinalIgnoreCase))
                                           {
                                               strId = eNodeNewsIdentifierChild.Value;
                                           }
                                       }
                                   }
                               }//for Identification
                           }//if Identification
                           if (eNodeNewsItemChild.Name.LocalName.ToString().Equals("NewsManagement", StringComparison.OrdinalIgnoreCase))
                           {
                               IEnumerable<XElement> childNoteNewsManagementList =
                                   from el in eNodeNewsItemChild.Elements()
                                   select el;
                               foreach (XElement eNodeNewsManagementChild in childNoteNewsManagementList)
                               {
                                   if (eNodeNewsManagementChild.Name.LocalName.ToString().Equals("FirstCreated", StringComparison.OrdinalIgnoreCase))
                                   {
                                       String strTempTime = "";
                                       strTempTime = eNodeNewsManagementChild.Value;
                                       strTempTime = strTempTime.Insert(4 + 0, "-");
                                       strTempTime = strTempTime.Insert(6 + 1, "-");
                                       strTempTime = strTempTime.Insert(11 + 2, ":");
                                       strTempTime = strTempTime.Insert(13 + 3, ":");
                                       strTempTime = strTempTime.Insert(15 + 4, ".000");
                                       dtNewsDateTime = DateTime.Parse(strTempTime);
                                   }
                               }//for NewsManagement
                           }//if NewsManagement
                           if (eNodeNewsItemChild.Name.LocalName.ToString().Equals("NewsComponent", StringComparison.OrdinalIgnoreCase))
                           {
                               IEnumerable<XElement> childNoteNewsComponent1List =
                                   from el in eNodeNewsItemChild.Elements()
                                   select el;
                               foreach (XElement eNodeNewsComponent1Child in childNoteNewsComponent1List)
                               {
                                   if (eNodeNewsComponent1Child.Name.LocalName.ToString().Equals("AdministrativeMetadata", StringComparison.OrdinalIgnoreCase))
                                   {
                                       IEnumerable<XElement> childNoteAdministrativeMetadataList =
                                            from el in eNodeNewsComponent1Child.Elements()
                                            select el;
                                       foreach (XElement eNodeAdministrativeMetadataChild in childNoteAdministrativeMetadataList)
                                       {
                                           if (eNodeAdministrativeMetadataChild.Name.LocalName.ToString().Equals("FileName", StringComparison.OrdinalIgnoreCase))
                                               strFileName = eNodeAdministrativeMetadataChild.Value.ToString();
                                           //Pat 20160725 Cannot find
                                           //try
                                           //{
                                           //    if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("fileName", StringComparison.OrdinalIgnoreCase))
                                           //        strFileName = eNodeItemMetaChild.Value.ToString();
                                           //    if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("firstCreated", StringComparison.OrdinalIgnoreCase))
                                           //        dtNewsDateTime = DateTime.Parse(eNodeItemMetaChild.Value);
                                           //    if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("signal", StringComparison.OrdinalIgnoreCase))
                                           //    {
                                           //        if (eNodeItemMetaChild.Attribute("qcode").Value.IndexOf("pmt:") > -1)
                                           //            strType = eNodeItemMetaChild.Attribute("qcode").Value.Replace("pmt:", "");
                                           //    }
                                           //    try
                                           //    {
                                           //        //int iCountMemberOf = 0; <---need find position to place
                                           //        if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("memberOf", StringComparison.OrdinalIgnoreCase))
                                           //        {
                                           //            iCountMemberOf++;
                                           //            if (iCountMemberOf == 2)
                                           //                strNewsCategory = eNodeItemMetaChild.Attribute("literal").Value;
                                           //        }
                                           //    }
                                           //    catch (Exception ex) { }
                                           //    if (strType.Length != 0 && dtNewsDateTime != DateTime.MinValue)
                                           //    {
                                           //        return;
                                           //    }
                                           //}
                                           //catch (Exception ex) { }
                                       }//for AdministrativeMetadata
                                   }//if AdministrativeMetadata
                                   if (eNodeNewsComponent1Child.Name.LocalName.ToString().Equals("NewsComponent", StringComparison.OrdinalIgnoreCase))
                                   {
                                       IEnumerable<XElement> childNoteNewsComponent2List =
                                           from el in eNodeNewsComponent1Child.Elements()
                                           select el;
                                       foreach (XElement eNodeNewsComponent2Child in childNoteNewsComponent2List)
                                       {
                                           if (eNodeNewsComponent2Child.Name.LocalName.ToString().Equals("ContentItem", StringComparison.OrdinalIgnoreCase))
                                           {
                                               IEnumerable<XElement> childNoteContentItemList =
                                                   from el in eNodeNewsComponent2Child.Elements()
                                                   select el;
                                               foreach (XElement eNodeContentItemChild in childNoteContentItemList)
                                               {
                                                   if (eNodeContentItemChild.Name.LocalName.ToString().Equals("MediaType", StringComparison.OrdinalIgnoreCase))
                                                   {
                                                       strType = eNodeContentItemChild.Attribute("FormalName").Value;
                                                   }
                                               }//for ContentItem
                                           }//if ContentItem
                                       }//for NewsComponent2
                                   }//if NewsComponent2
                               }//for NewsComponent1
                           }//if NewsComponent1
                       }//for NewsItem
                   }//if NewsItem
               }//for all
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

        private void ReadCaptionAndMeta()
        {
            meta.strDateCreate = dtNewsDateTime.ToString("yyyy-MM-dd HH:mm:ss");
            //try
            //{
            //    IEnumerable<XElement> childList =
            //    from el in doc.Root.Elements()
            //    select el;
            //    foreach (XElement e in childList)
            //    {
            //        if (e.Name.LocalName.ToString().Equals("itemSet", StringComparison.OrdinalIgnoreCase))
            //        {
            //            String strNodeValue = e.ToString();
            //            IEnumerable<XElement> childNoteItemSetList =
            //                from el in e.Elements()
            //                select el;
            //            foreach (XElement eNodeItemSetChild in childNoteItemSetList)
            //            {
            //                if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
            //                {
            //                    IEnumerable<XElement> childNoteNewItemList =
            //                        from el in eNodeItemSetChild.Elements()
            //                        select el;
            //                    foreach (XElement eNodeNewItemChild in childNoteNewItemList)
            //                    {
            //                        if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentMeta", StringComparison.OrdinalIgnoreCase))
            //                        {
            //                            IEnumerable<XElement> childNoteContentMetaList =
            //                                from el in eNodeNewItemChild.Elements()
            //                                select el;
            //                            foreach (XElement eNodeContentMetaChild in childNoteContentMetaList)
            //                            {
            //                                try
            //                                {
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("subject", StringComparison.OrdinalIgnoreCase))
            //                                    {
            //                                        try
            //                                        {
            //                                            lst_Subject.Add(eNodeContentMetaChild.Attribute("qcode").Value);
            //                                            if (eNodeContentMetaChild.Attribute("qcode").Value.IndexOf("N2:") > -1)
            //                                            {
            //                                                meta.strSuppCate += eNodeContentMetaChild.Attribute("qcode").Value.Replace("N2:", "") + " ";
            //                                                continue;
            //                                            }

            //                                            meta.strCityCode = eNodeContentMetaChild.Attribute("literal").Value;

            //                                        }catch(Exception ex){}
            //                                    }
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("altId", StringComparison.OrdinalIgnoreCase))
            //                                    {
            //                                        if (eNodeContentMetaChild.Attribute("type").Value.IndexOf("idType:IID") > -1)
            //                                            meta.strFixidentifier = eNodeContentMetaChild.Value;
            //                                        if (eNodeContentMetaChild.Attribute("type").Value.IndexOf("idType:OTR") > -1)
            //                                            meta.strOTR = eNodeContentMetaChild.Value;
            //                                    }
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("by", StringComparison.OrdinalIgnoreCase))
            //                                        meta.strByLine = eNodeContentMetaChild.Value;
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("subject", StringComparison.OrdinalIgnoreCase))
            //                                    {
            //                                        try
            //                                        {
            //                                        if (eNodeContentMetaChild.ToString().IndexOf("literal") > -1)
            //                                            meta.strCity = eNodeContentMetaChild.Attribute("literal").Value;
            //                                        }
            //                                        catch (Exception ex) { }
            //                                    }
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("infoSource", StringComparison.OrdinalIgnoreCase))
            //                                    {
            //                                        try
            //                                        {
            //                                            if (eNodeContentMetaChild.Attribute("role").Value.IndexOf("cRole:source") > -1)
            //                                            {
            //                                                meta.strSource = eNodeContentMetaChild.Attribute("qcode").Value.Replace("photCode:", "") + " ";
            //                                                continue;
            //                                            }
            //                                        }
            //                                        catch (Exception ex) { }
            //                                    }
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("creditline", StringComparison.OrdinalIgnoreCase))
            //                                        meta.strCredit = eNodeContentMetaChild.Value;
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("description", StringComparison.OrdinalIgnoreCase))
            //                                        strCaption = eNodeContentMetaChild.Value;
            //                                    if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("contributor", StringComparison.OrdinalIgnoreCase))
            //                                    {
            //                                        try
            //                                        {
            //                                            if (eNodeContentMetaChild.ToString().IndexOf("literal") > -1)
            //                                                meta.strCaptionWriter = eNodeContentMetaChild.Attribute("literal").Value;
            //                                        }
            //                                        catch (Exception ex) { }
                                                    
            //                                    }
            //                                }
            //                                catch (Exception ex)
            //                                {
            //                                }
            //                            }
            //                        }
            //                    }
            //                }
            //            }
            //        }
            //    }
            //}
            //catch (Exception ex)
            //{

            //}
        }

        private void ReadAllPhotoName()
        {
            //try
            //{
            //    IEnumerable<XElement> childList =
            //    from el in doc.Root.Elements()
            //    select el;
            //    foreach (XElement e in childList)
            //    {
            //        if (e.Name.LocalName.ToString().Equals("itemSet", StringComparison.OrdinalIgnoreCase))
            //        {
            //            String strNodeValue = e.ToString();
            //            IEnumerable<XElement> childNoteItemSetList =
            //                from el in e.Elements()
            //                select el;
            //            foreach (XElement eNodeItemSetChild in childNoteItemSetList)
            //            {
            //                if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
            //                {
            //                    IEnumerable<XElement> childNoteNewItemList =
            //                        from el in eNodeItemSetChild.Elements()
            //                        select el;
            //                    foreach (XElement eNodeNewItemChild in childNoteNewItemList)
            //                    {
            //                        if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentSet", StringComparison.OrdinalIgnoreCase))
            //                        {
            //                            IEnumerable<XElement> childNoteContentSetList =
            //                                from el in eNodeNewItemChild.Elements()
            //                                select el;
            //                            foreach (XElement eNodeContentSetChild in childNoteContentSetList)
            //                            {
            //                                String photoSizeType = ""; // thumbnail, baseImage, viewImage
            //                                photoSizeType = eNodeContentSetChild.Attribute("rendition").Value.Replace("rend:", "");
            //                                if (photoSizeType.Equals("thumbnail", StringComparison.OrdinalIgnoreCase))
            //                                    photoInfo.strPhotoSmallPath = eNodeContentSetChild.Attribute("href").Value;
            //                                else if (photoSizeType.Equals("baseImage", StringComparison.OrdinalIgnoreCase))
            //                                {
            //                                    photoInfo.strSize = eNodeContentSetChild.Attribute("size").Value;
            //                                    try
            //                                    {
            //                                        photoInfo.strPhotoWidth = eNodeContentSetChild.Attribute("width").Value;
            //                                        photoInfo.strPhotoHeight = eNodeContentSetChild.Attribute("height").Value; 
            //                                    }
            //                                    catch (Exception ex) { }
            //                                    photoInfo.strPhotoSourcePath = eNodeContentSetChild.Attribute("href").Value;
                                               
            //                                }
            //                                else
            //                                    photoInfo.strPhotoMiddlePath = eNodeContentSetChild.Attribute("href").Value;

            //                                IEnumerable<XElement> childNoteRemoteContentList =
            //                                from el in eNodeContentSetChild.Elements()
            //                                select el;
            //                                foreach (XElement eNodeRemoteContentChild in childNoteRemoteContentList)
            //                                {
            //                                    if (eNodeRemoteContentChild.Name.LocalName.ToString().Equals("altId", StringComparison.OrdinalIgnoreCase))
            //                                    {
            //                                        if (photoSizeType.Equals("thumbnail", StringComparison.OrdinalIgnoreCase))
            //                                            photoInfo.strSmallPhotoName = eNodeRemoteContentChild.Value;
            //                                        else if (photoSizeType.Equals("baseImage", StringComparison.OrdinalIgnoreCase))
            //                                            photoInfo.strSourcePhotoName = eNodeRemoteContentChild.Value;
            //                                        else
            //                                            photoInfo.stMiddlePhotoName = eNodeRemoteContentChild.Value;
            //                                    }
            //                                }
            //                            }
            //                        }
            //                    }
            //                }
            //            }
            //        }
            //    }
            //}
            //catch (Exception ex) { }
        }

        private void ReadNewsCategory()
        {

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

        public void SetStrFilePhyName(String str)
        {
            this.strFilePhyName = str;
        }

        public String GetStrFilePhyName()
        {
            return strFilePhyName;
        }
    }
}