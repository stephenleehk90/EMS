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
    class XHS_XML_Reader:Reader
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

        public XHS_XML_Reader(String strFilePath, Boolean isFromReuters) //strFilePath :can be file path or XML content in String
        {
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
                strFileName = strFilePath.Substring(strFilePath.LastIndexOf('\\') + 1);
            }
            else
            {
                try
                {
                    FileInfo file = new FileInfo(strFilePath);
                    if (file.Extension.Equals(".xml", StringComparison.OrdinalIgnoreCase))
                    {
                        doc = XDocument.Load(strFilePath);
                        strFileName = strFilePath.Substring(strFilePath.LastIndexOf('\\') + 1);
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

        public XHS_XML_Reader(XDocument doc)
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
                    if (e.Name.LocalName.ToString().Equals("Item", StringComparison.OrdinalIgnoreCase))
                    {
                        IEnumerable<XElement> childNoteItem =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeItemChild in childNoteItem)
                        {
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Component", StringComparison.OrdinalIgnoreCase))
                            {
                                String strNodeValue = eNodeItemChild.ToString();
                                IEnumerable<XElement> childNoteComponentList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeComponentChild in childNoteComponentList)
                                {
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("ContentItem", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentItemList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentItemChild in childNoteContentItemList)
                                        {
                                            if (eNodeContentItemChild.Name.LocalName.ToString().Equals("Content", StringComparison.OrdinalIgnoreCase))
                                            {
                                                strContent = eNodeContentItemChild.Value;
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
                    if (e.Name.LocalName.ToString().Equals("Item", StringComparison.OrdinalIgnoreCase))
                    {
                        IEnumerable<XElement> childNoteItem =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeItemChild in childNoteItem)
                        {
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Management", StringComparison.OrdinalIgnoreCase))
                            {
                                String strNodeValue = eNodeItemChild.ToString();
                                IEnumerable<XElement> childNoteManagementList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeManagementChild in childNoteManagementList)
                                {
                                    if (eNodeManagementChild.Name.LocalName.ToString().Equals("Urgency", StringComparison.OrdinalIgnoreCase))
                                    {
                                        String strUrgency = eNodeManagementChild.Value;
                                        try
                                        {
                                            if (Int32.Parse(strUrgency) <= 2)
                                                isUrgent = true;
                                        }
                                        catch (Exception ex) { isUrgent = false; }
                                    }
                                }
                            }
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Component", StringComparison.OrdinalIgnoreCase))
                            {
                                String strNodeValue = eNodeItemChild.ToString();
                                IEnumerable<XElement> childNoteComponentList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeComponentChild in childNoteComponentList)
                                {
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("Lines", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteLinesList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeLinesChild in childNoteLinesList)
                                        {
                                            if (eNodeLinesChild.Name.LocalName.ToString().Equals("HeadLine", StringComparison.OrdinalIgnoreCase))
                                                strHeadLine = eNodeLinesChild.Value.ToString();
                                        }
                                    }
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("DescriptiveMetadata", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteDescriptiveMetadataList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeDescriptiveMetadataChild in childNoteDescriptiveMetadataList)
                                        {
                                            if (eNodeDescriptiveMetadataChild.Name.LocalName.ToString().Equals("SubjectCode", StringComparison.OrdinalIgnoreCase))
                                            {
                                                
                                                IEnumerable<XElement> childNoteSubjectCodeList =
                                                    from el in eNodeDescriptiveMetadataChild.Elements()
                                                    select el;
                                                foreach (XElement eNodeSubjectCodeChild in childNoteSubjectCodeList)
                                                {
                                                    if (eNodeSubjectCodeChild.Name.LocalName.ToString().Equals("DetailCode", StringComparison.OrdinalIgnoreCase))
                                                    try
                                                    {
                                                        lst_Subject.Add(eNodeSubjectCodeChild.Attribute("HowPresent").Value);
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
                    if (e.Name.LocalName.ToString().Equals("Item", StringComparison.OrdinalIgnoreCase))
                    {
                        IEnumerable<XElement> childNoteItem =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeItemChild in childNoteItem)
                        {
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Identification", StringComparison.OrdinalIgnoreCase))
                            {
                                IEnumerable<XElement> childNoteIdentificationList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeIdentificationChild in childNoteIdentificationList)
                                {
                                    String strNodeValue = eNodeIdentificationChild.ToString();
                                    if (eNodeIdentificationChild.Name.LocalName.ToString().Equals("NewsId", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteNewsIdList =
                                            from el in eNodeIdentificationChild.Elements()
                                            select el;
                                        foreach (XElement eNodeNewsIdChild in childNoteNewsIdList)
                                        {
                                            if (eNodeNewsIdChild.Name.LocalName.ToString().Equals("ItemId", StringComparison.OrdinalIgnoreCase) && strId.Length == 0)// capture News ID created by Reuters
                                                strId = eNodeNewsIdChild.Value;
                                        }
                                    }
                                }
                            }
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Management", StringComparison.OrdinalIgnoreCase))
                            {
                                String strNodeValue = eNodeItemChild.ToString();
                                IEnumerable<XElement> childNoteManagementList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeManagementChild in childNoteManagementList)
                                {
                                    if (eNodeManagementChild.Name.LocalName.ToString().Equals("ThisRevisionDate", StringComparison.OrdinalIgnoreCase) && iPriority == 0)// Will be used to idetify the urgency level
                                        try
                                        {
                                            String strNewsDateTime = eNodeManagementChild.Value;
                                            if (strNewsDateTime.LastIndexOf('Z') > 0)
                                                strNewsDateTime = strNewsDateTime.Substring(0, strNewsDateTime.LastIndexOf('Z'));
                                            dtNewsDateTime = DateTime.ParseExact(strNewsDateTime, "yyyyMMddTHHmmss", null, System.Globalization.DateTimeStyles.AssumeLocal);
                                        }
                                        catch (Exception ex) {}
                                    if (eNodeManagementChild.Name.LocalName.ToString().Equals("Importance", StringComparison.OrdinalIgnoreCase) && iPriority == 0)// Will be used to idetify the urgency level
                                        iPriority = Int32.Parse(eNodeManagementChild.Attribute("NormalName").Value);
                                }
                            }
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Component", StringComparison.OrdinalIgnoreCase))
                            {
                                String strNodeValue = eNodeItemChild.ToString();
                                IEnumerable<XElement> childNoteComponentList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeComponentChild in childNoteComponentList)
                                {
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("Lines", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteLinesList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeLinesChild in childNoteLinesList)
                                        {
                                            if (eNodeLinesChild.Name.LocalName.ToString().Equals("KeywordLine", StringComparison.OrdinalIgnoreCase))
                                                strNewsCategory = eNodeLinesChild.Value.ToString();
                                        }
                                    }
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("ContentItem", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentItemList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentItemChild in childNoteContentItemList)
                                        {
                                            if (eNodeContentItemChild.Name.LocalName.ToString().Equals("MediaType", StringComparison.OrdinalIgnoreCase))
                                            {
                                                if (eNodeContentItemChild.Attribute("NormalName").Value == "Photo")
                                                    strType = eNodeContentItemChild.Attribute("NormalName").Value;
                                                if (strType.Length != 0 && dtNewsDateTime != DateTime.MinValue)
                                                {
                                                    return;
                                                }
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
                    if (e.Name.LocalName.ToString().Equals("Item", StringComparison.OrdinalIgnoreCase))
                    {
                        IEnumerable<XElement> childNoteItem =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeItemChild in childNoteItem)
                        {
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Component", StringComparison.OrdinalIgnoreCase))
                            {
                                String strNodeValue = eNodeItemChild.ToString();
                                IEnumerable<XElement> childNoteComponentList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeComponentChild in childNoteComponentList)
                                {
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("Lines", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteLinesList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeLinesChild in childNoteLinesList)
                                        {
                                            if (eNodeLinesChild.Name.LocalName.ToString().Equals("CopyrightLine", StringComparison.OrdinalIgnoreCase))
                                                meta.strCredit = eNodeLinesChild.Value;
                                        }
                                    }
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("CreationMetadata", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteCreationMetadataList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeCreationMetadataChild in childNoteCreationMetadataList)
                                        {
                                            if (eNodeComponentChild.Name.LocalName.ToString().Equals("Creator", StringComparison.OrdinalIgnoreCase))
                                            {
                                                IEnumerable<XElement> childNoteCreatorList =
                                                    from el in eNodeComponentChild.Elements()
                                                    select el;
                                                foreach (XElement eNodeCreatorChild in childNoteCreatorList)
                                                {
                                                    if (eNodeCreatorChild.Name.LocalName.ToString().Equals("Person", StringComparison.OrdinalIgnoreCase))
                                                        meta.strByLine = eNodeCreatorChild.Attribute("NormalName").Value;
                                                }
                                            }
                                        }
                                    }
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("DescriptiveMetadata", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteDescriptiveMetadataList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeDescriptiveMetadataChild in childNoteDescriptiveMetadataList)
                                        {
                                            if (eNodeDescriptiveMetadataChild.Name.LocalName.ToString().Equals("SubjectCode", StringComparison.OrdinalIgnoreCase))
                                            {

                                                IEnumerable<XElement> childNoteSubjectCodeList =
                                                    from el in eNodeDescriptiveMetadataChild.Elements()
                                                    select el;
                                                foreach (XElement eNodeSubjectCodeChild in childNoteSubjectCodeList)
                                                {
                                                    if (eNodeSubjectCodeChild.Name.LocalName.ToString().Equals("DetailCode", StringComparison.OrdinalIgnoreCase))
                                                        try
                                                        {
                                                            //lst_Subject.Add(eNodeSubjectCodeChild.Attribute("HowPresent").Value);
                                                            meta.strSuppCate += eNodeSubjectCodeChild.Attribute("HowPresent").Value + " ";
                                                        }
                                                        catch (Exception ex) { }
                                                }
                                            }
                                            if (eNodeDescriptiveMetadataChild.Name.LocalName.ToString().Equals("Location", StringComparison.OrdinalIgnoreCase))
                                            {

                                                IEnumerable<XElement> childNoteLocationList =
                                                    from el in eNodeDescriptiveMetadataChild.Elements()
                                                    select el;
                                                foreach (XElement eNodeLocationChild in childNoteLocationList)
                                                {
                                                    if (eNodeLocationChild.Name.LocalName.ToString().Equals("Property", StringComparison.OrdinalIgnoreCase))
                                                    {
                                                        if (eNodeLocationChild.Attribute("NormalName").Value == "HappenSpot")
                                                        {
                                                            meta.strCityCode = eNodeLocationChild.Attribute("Value").Value;
                                                            meta.strCity = eNodeLocationChild.Attribute("Value").Value;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("ContentItem", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentItemList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentItemChild in childNoteContentItemList)
                                        {
                                            if (eNodeContentItemChild.Name.LocalName.ToString().Equals("MediaType", StringComparison.OrdinalIgnoreCase))
                                            {
                                                if (eNodeContentItemChild.Attribute("NormalName").Value != "Photo")
                                                    break;
                                            }
                                            if (eNodeContentItemChild.Name.LocalName.ToString().Equals("Content", StringComparison.OrdinalIgnoreCase))
                                            {
                                                strCaption = eNodeContentItemChild.Value.Trim();
                                                strCaption.Trim();
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
                    if (e.Name.LocalName.ToString().Equals("Item", StringComparison.OrdinalIgnoreCase))
                    {
                        IEnumerable<XElement> childNoteItem =
                            from el in e.Elements()
                            select el;
                        foreach (XElement eNodeItemChild in childNoteItem)
                        {
                            if (eNodeItemChild.Name.LocalName.ToString().Equals("Component", StringComparison.OrdinalIgnoreCase))
                            {
                                String strNodeValue = eNodeItemChild.ToString();
                                IEnumerable<XElement> childNoteComponentList =
                                    from el in eNodeItemChild.Elements()
                                    select el;
                                foreach (XElement eNodeComponentChild in childNoteComponentList)
                                {
                                    if (eNodeComponentChild.Name.LocalName.ToString().Equals("ContentItem", StringComparison.OrdinalIgnoreCase))
                                    {
                                        IEnumerable<XElement> childNoteContentItemList =
                                            from el in eNodeComponentChild.Elements()
                                            select el;
                                        foreach (XElement eNodeContentItemChild in childNoteContentItemList)
                                        {
                                            if (eNodeContentItemChild.Name.LocalName.ToString().Equals("MediaType", StringComparison.OrdinalIgnoreCase))
                                            {
                                                if (eNodeContentItemChild.Attribute("NormalName").Value != "Photo")
                                                    break;
                                            }

                                            photoInfo.strPhotoSourcePath = eNodeComponentChild.Attribute("Href").Value;
                                            photoInfo.strSourcePhotoName = eNodeComponentChild.Attribute("Href").Value;
                                            photoInfo.strPhotoMiddlePath = eNodeComponentChild.Attribute("Href").Value;
                                            photoInfo.strMiddlePhotoName = eNodeComponentChild.Attribute("Href").Value;
                                            photoInfo.strPhotoSmallPath = eNodeComponentChild.Attribute("Href").Value;
                                            photoInfo.strSmallPhotoName = eNodeComponentChild.Attribute("Href").Value;

                                            if (eNodeContentItemChild.Name.LocalName.ToString().Equals("Characteristics", StringComparison.OrdinalIgnoreCase))
                                            {
                                                IEnumerable<XElement> childNoteCharacteristicsList =
                                                    from el in eNodeContentItemChild.Elements()
                                                    select el;
                                                foreach (XElement eNodeCharacteristicsChild in childNoteCharacteristicsList)
                                                {
                                                    if (eNodeCharacteristicsChild.Name.LocalName.ToString().Equals("Bytes", StringComparison.OrdinalIgnoreCase))
                                                        photoInfo.strSize = eNodeCharacteristicsChild.Value;
                                                    if (eNodeCharacteristicsChild.Name.LocalName.ToString().Equals("Property", StringComparison.OrdinalIgnoreCase))
                                                    {
                                                        if (eNodeCharacteristicsChild.Attribute("NormalName").Value == "PixelHeight")
                                                            photoInfo.strPhotoHeight = eNodeCharacteristicsChild.Attribute("Value").Value;
                                                        if (eNodeCharacteristicsChild.Attribute("NormalName").Value == "PixelWidth")
                                                            photoInfo.strPhotoWidth = eNodeCharacteristicsChild.Attribute("Value").Value;
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