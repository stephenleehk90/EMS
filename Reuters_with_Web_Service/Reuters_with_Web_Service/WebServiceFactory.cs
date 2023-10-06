using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
/*
 * 
 *This class is used to control with type of Service will be use based on the EMS version settle by user.
 *Each Class could be really different on insert into target table. 
 */
namespace Reuters_with_Web_Service
{
    class WebServiceFactory
    {
        public WebServiceHandler Create(String strUserName, String strPassword, String strTypeName)
        {
            switch(strTypeName)
            {
                case"EMS6.0":
                    return new WebServiceHandler_TKP(strUserName, strPassword);
                case "EMS4.0":
                    return new WebServiceHandler_EMS4(strUserName, strPassword);
                case "EMSFBS":
                    return new WebServiceHandler_FBS(strUserName, strPassword);
                case "EMS6.5":
                    return new WebServiceHandler_MP(strUserName, strPassword);
                default:
                    return null;
            }
        }
    }
}
