using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Xml.Linq;
using System.Text.RegularExpressions;
/*
 * This class used to handle all the operation on reading data in XML files
 */
namespace Reuters_with_Web_Service
{
    class XML_Reader:Reader
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

        public XML_Reader(String strFilePath, Boolean isFromReuters) //strFilePath :can be file path or XML content in String
        {
            photoInfo = new PhotoInfo();
            photoInfo.strPhotoWidth = "";
            photoInfo.strPhotoHeight = "";
            photoInfo.lstPhoto = new List<Photo>();
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
            return strId;
        }

        public XML_Reader(XDocument doc)
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
                                    if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentSet",StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentSetList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentSetChild in childNoteContentSetList)
                                        {
                                            if (eNodeContentSetChild.Name.LocalName.ToString().Equals("inlineXML", StringComparison.OrdinalIgnoreCase))
                                            {
                                                IEnumerable<XElement> childNoteInlineXMLList =
                                                   from el in eNodeContentSetChild.Elements()
                                                   select el;
                                                foreach (XElement eNodeInlineXMLChild in childNoteInlineXMLList)
                                                {
                                                    //strContent = eNodeInlineXMLChild.Value.ToString();
                                                    strContent = eNodeInlineXMLChild.ToString();
                                                    strContent = strContent.Substring(strContent.IndexOf("<body>"), strContent.LastIndexOf("</body>") - strContent.IndexOf("<body>"));
                                                    strContent = strContent.Replace("<p>", "");
                                                    strContent = strContent.Replace("<body>", ""); strContent = strContent.Replace("</body>", "");
                                                    strContent = strContent.Replace("\n", "\r\n");
                                                    strContent = strContent.Replace("</p>", "   \r\n");
                                                    strContent = strContent.Replace("<p />", "   \r\n");
                                                    strContent.Trim();
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
            catch (Exception ex)
            {

            }
        }

        public String GetXMLFileName()
        {
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
                                    if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentMeta",StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentMetaList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentMetaChild in childNoteContentMetaList)
                                        {
                                            if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("description", StringComparison.OrdinalIgnoreCase))
                                            {
                                                strHeadLine = eNodeContentMetaChild.Value;
                                            }
                                            if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("urgency", StringComparison.OrdinalIgnoreCase))
                                            {
                                                String strUrgency = eNodeContentMetaChild.Value;
                                                try
                                                {
                                                    if (Int32.Parse(strUrgency) <= 2)
                                                        isUrgent = true;
                                                }catch(Exception ex){isUrgent = false;}
                                            }
                                            if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("subject", StringComparison.OrdinalIgnoreCase))
                                            {
                                                try
                                                {
                                                    lst_Subject.Add(eNodeContentMetaChild.Attribute("qcode").Value);
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
                   if (e.Name.LocalName.ToString().Equals("header", StringComparison.OrdinalIgnoreCase))
                   {
                       String strNodeValue = e.ToString();
                       IEnumerable<XElement> childNoteHeaderList =
                           from el in e.Elements()
                           select el;
                       foreach (XElement eNodeHeaderChild in childNoteHeaderList)
                       {
                           if (eNodeHeaderChild.Name.LocalName.ToString().Equals("transmitId", StringComparison.OrdinalIgnoreCase) && strId.Length == 0)// capture News ID created by Reuters
                               strId = eNodeHeaderChild.Value;
                           if (eNodeHeaderChild.Name.LocalName.ToString().Equals("priority", StringComparison.OrdinalIgnoreCase) && iPriority == 0)// Will be used to idetify the urgency level
                               iPriority = Int32.Parse(eNodeHeaderChild.Value);
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
                                       int iCountMemberOf = 0;
                                       IEnumerable<XElement> childNoteItemMetaList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                       foreach (XElement eNodeItemMetaChild in childNoteItemMetaList)
                                       {
                                           try
                                           {
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("fileName", StringComparison.OrdinalIgnoreCase))
                                                   strFileName = eNodeItemMetaChild.Value.ToString();
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("firstCreated", StringComparison.OrdinalIgnoreCase))
                                                   dtNewsDateTime = DateTime.Parse(eNodeItemMetaChild.Value);
                                               if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("signal", StringComparison.OrdinalIgnoreCase))
                                               {
                                                   if (eNodeItemMetaChild.Attribute("qcode").Value.IndexOf("pmt:") > -1)
                                                       strType = eNodeItemMetaChild.Attribute("qcode").Value.Replace("pmt:", "");
                                               }
                                               try
                                               {
                                                   if (eNodeItemMetaChild.Name.LocalName.ToString().Equals("memberOf", StringComparison.OrdinalIgnoreCase))
                                                   {
                                                       iCountMemberOf++;
                                                       if (iCountMemberOf == 2)
                                                           strNewsCategory = eNodeItemMetaChild.Attribute("literal").Value;
                                                   }
                                               }
                                               catch (Exception ex) { }
                                               if (strType.Length != 0 && dtNewsDateTime != DateTime.MinValue)
                                               {
                                                   return;
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
                            if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
                            {
                                IEnumerable<XElement> childNoteNewItemList =
                                    from el in eNodeItemSetChild.Elements()
                                    select el;
                                foreach (XElement eNodeNewItemChild in childNoteNewItemList)
                                {
                                    if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentMeta", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentMetaList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentMetaChild in childNoteContentMetaList)
                                        {
                                            try
                                            {
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("subject", StringComparison.OrdinalIgnoreCase))
                                                {
                                                    try
                                                    {
                                                        lst_Subject.Add(eNodeContentMetaChild.Attribute("qcode").Value);
                                                        if (eNodeContentMetaChild.Attribute("qcode").Value.IndexOf("N2:") > -1)
                                                        {
                                                            meta.strSuppCate += eNodeContentMetaChild.Attribute("qcode").Value.Replace("N2:", "") + " ";
                                                            continue;
                                                        }

                                                        meta.strCityCode = eNodeContentMetaChild.Attribute("literal").Value;

                                                    }catch(Exception ex){}
                                                }
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("altId", StringComparison.OrdinalIgnoreCase))
                                                {
                                                    if (eNodeContentMetaChild.Attribute("type").Value.IndexOf("idType:IID") > -1)
                                                        meta.strFixidentifier = eNodeContentMetaChild.Value;
                                                    if (eNodeContentMetaChild.Attribute("type").Value.IndexOf("idType:OTR") > -1)
                                                        meta.strOTR = eNodeContentMetaChild.Value;
                                                }
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("by", StringComparison.OrdinalIgnoreCase))
                                                    meta.strByLine = eNodeContentMetaChild.Value;
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("subject", StringComparison.OrdinalIgnoreCase))
                                                {
                                                    try
                                                    {
                                                    if (eNodeContentMetaChild.ToString().IndexOf("literal") > -1)
                                                        meta.strCity = eNodeContentMetaChild.Attribute("literal").Value;
                                                    }
                                                    catch (Exception ex) { }
                                                }
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("infoSource", StringComparison.OrdinalIgnoreCase))
                                                {
                                                    try
                                                    {
                                                        if (eNodeContentMetaChild.Attribute("role").Value.IndexOf("cRole:source") > -1)
                                                        {
                                                            meta.strSource = eNodeContentMetaChild.Attribute("qcode").Value.Replace("photCode:", "") + " ";
                                                            continue;
                                                        }
                                                    }
                                                    catch (Exception ex) { }
                                                }
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("creditline", StringComparison.OrdinalIgnoreCase))
                                                    meta.strCredit = eNodeContentMetaChild.Value;
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("description", StringComparison.OrdinalIgnoreCase))
                                                    strCaption = eNodeContentMetaChild.Value;
                                                if (eNodeContentMetaChild.Name.LocalName.ToString().Equals("contributor", StringComparison.OrdinalIgnoreCase))
                                                {
                                                    try
                                                    {
                                                        if (eNodeContentMetaChild.ToString().IndexOf("literal") > -1)
                                                            meta.strCaptionWriter = eNodeContentMetaChild.Attribute("literal").Value;
                                                    }
                                                    catch (Exception ex) { }
                                                    
                                                }
                                            }
                                            catch (Exception ex)
                                            {
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {

            }
        }

        private void ReadAllPhotoName()
        {
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
                            if (eNodeItemSetChild.Name.LocalName.ToString().Equals("newsItem", StringComparison.OrdinalIgnoreCase))
                            {
                                IEnumerable<XElement> childNoteNewItemList =
                                    from el in eNodeItemSetChild.Elements()
                                    select el;
                                foreach (XElement eNodeNewItemChild in childNoteNewItemList)
                                {
                                    if (eNodeNewItemChild.Name.LocalName.ToString().Equals("contentSet", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentSetList =
                                            from el in eNodeNewItemChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentSetChild in childNoteContentSetList)
                                        {
                                            Photo photo = new Photo();
                                            String photoSizeType = ""; // thumbnail, baseImage, viewImage
                                            photoSizeType = eNodeContentSetChild.Attribute("rendition").Value.Replace("rend:", "");
                                            if (photoSizeType.Equals("thumbnail", StringComparison.OrdinalIgnoreCase))
                                                photoInfo.strPhotoSmallPath = eNodeContentSetChild.Attribute("href").Value;
                                            else if (photoSizeType.Equals("baseImage", StringComparison.OrdinalIgnoreCase))
                                            {
                                                photoInfo.strSize = eNodeContentSetChild.Attribute("size").Value;
                                                try
                                                {
                                                    photoInfo.strPhotoWidth = eNodeContentSetChild.Attribute("width").Value;
                                                    photoInfo.strPhotoHeight = eNodeContentSetChild.Attribute("height").Value; 
                                                }
                                                catch (Exception ex) { }
                                                photoInfo.strPhotoSourcePath = eNodeContentSetChild.Attribute("href").Value;

                                                // Edit 2023-08-15    
                                                if (photo.strPhotoPath == null || photo.strPhotoPath.Trim() == "")
                                                    photo.strPhotoPath = eNodeContentSetChild.Attribute("href").Value;
                                                if (photo.strPhotoSizeType == null || photo.strPhotoSizeType.Trim() == "")
                                                    photo.strPhotoSizeType = photoSizeType;
                                                // Edit 2023-08-15    
                                               
                                            }
                                            else if (photoSizeType.Equals("viewImage", StringComparison.OrdinalIgnoreCase))
                                                photoInfo.strPhotoMiddlePath = eNodeContentSetChild.Attribute("href").Value;
                                            // Edit 2023-08-15    
                                            //else
                                            else if (photoSizeType.Equals("limitedImage", StringComparison.OrdinalIgnoreCase))
                                            // Edit 2023-08-15    
                                            {
                                                photo.strSize = eNodeContentSetChild.Attribute("size").Value;
                                                try
                                                {
                                                    photo.strPhotoWidth = eNodeContentSetChild.Attribute("width").Value;
                                                    photo.strPhotoHeight = eNodeContentSetChild.Attribute("height").Value;
                                                }
                                                catch (Exception ex) { }
//                                                photo.strPhotoPath = eNodeContentSetChild.Attribute("href").Value;
  //                                              photo.strPhotoSizeType = photoSizeType;

                                                // Edit 2023-08-15    
                                                if (photo.strPhotoPath == null || photo.strPhotoPath.Trim() == "")
                                                    photo.strPhotoPath = eNodeContentSetChild.Attribute("href").Value;
                                                if (photo.strPhotoSizeType == null || photo.strPhotoSizeType.Trim() == "")
                                                    photo.strPhotoSizeType = photoSizeType;
                                                // Edit 2023-08-15    

                                                /*IEnumerable<XElement> childNoteRemoteContentListOther =
                                                from el in eNodeContentSetChild.Elements()
                                                select el;
                                                foreach (XElement eNodeRemoteContentChild in childNoteRemoteContentListOther)
                                                {
                                                    if (eNodeRemoteContentChild.Name.LocalName.ToString().Equals("altId", StringComparison.OrdinalIgnoreCase))
                                                    {
                                                        photo.strPhotoName = eNodeRemoteContentChild.Value;
                                                    }
                                                }
                                                photoInfo.lstPhoto.Add(photo);*/
                                            }

                                            IEnumerable<XElement> childNoteRemoteContentList =
                                            from el in eNodeContentSetChild.Elements()
                                            select el;
                                            foreach (XElement eNodeRemoteContentChild in childNoteRemoteContentList)
                                            {
                                                if (eNodeRemoteContentChild.Name.LocalName.ToString().Equals("altId", StringComparison.OrdinalIgnoreCase))
                                                {
                                                    if (photoSizeType.Equals("thumbnail", StringComparison.OrdinalIgnoreCase))
                                                        photoInfo.strSmallPhotoName = eNodeRemoteContentChild.Value;
                                                    else if (photoSizeType.Equals("baseImage", StringComparison.OrdinalIgnoreCase))
                                                        photoInfo.strSourcePhotoName = eNodeRemoteContentChild.Value;
                                                    else if (photoSizeType.Equals("viewImage", StringComparison.OrdinalIgnoreCase))
                                                        photoInfo.strMiddlePhotoName = eNodeRemoteContentChild.Value;
                                                    // Edit 2023-08-15    
                                                    else if (photoSizeType.Equals("limitedImage", StringComparison.OrdinalIgnoreCase))
                                                        photoInfo.strLimitedPhotoName = eNodeRemoteContentChild.Value;
                                                    // Edit 2023-08-15    
                                                    photo.strPhotoName = eNodeRemoteContentChild.Value;
                                                }
                                            }
                                            photoInfo.lstPhoto.Add(photo);
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
    }
}

class PhotoInfo
{
    public String strPixel;
    public String strSize; // in bytes
    public String strPhotoHeight;
    public String strPhotoWidth;
    public String strPhotoSourcePath;
    public String strPhotoMiddlePath; 
    public String strPhotoSmallPath; 
    public String strSourcePhotoName;
    public String strMiddlePhotoName;
    public String strSmallPhotoName;
    public List<Photo> lstPhoto;
    public String strHeadLine;
    public String strSourcePath;
    public String strLimitedPhotoName;
}

class Photo
{
    public String strSize; // in bytes
    public String strPhotoHeight;
    public String strPhotoWidth;
    public String strPhotoPath;
    public String strPhotoName;
    public String strPhotoSizeType;
}

class MediaMeta
{
    public String strCategory;
    public String strSuppCate;
    public String strFixidentifier;
    public String strDateCreate;
    public String strByLine;
    public String strCity;
    public String strCityCode;
    public String strCountryName;
    public String strCountryCode;
    public String strOTR;
    public String strCredit;
    public String strCaptionWriter;
    public String strSource;
}
