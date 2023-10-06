using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reuters_with_Web_Service
{
    interface Reader
    {
        String GetNewsID();
        DateTime GetNewsDateTime();
        String GetStrContent();
        String GetStrHeadLine();
        int GetPriority();
        String GetXMLFileName();
        Boolean Read();
        void SetNewsID(String strId);
        List<String> GetSubjectList();
        //Pat 20160721 add bloomberg-----------------------------------------------
        int GetWordCounts();
        String GetUrgent();
        PhotoInfo GetPhotoInfo();
        String GetStrCaption();
        //-------------------------------------------------------------------------
        String GetErrorMessage();
    }


}
