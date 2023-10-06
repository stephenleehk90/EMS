using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Reuters_with_Web_Service.com.reuters.rmb;
/*
 * This is the abstract class of each WebService class
 */
namespace Reuters_with_Web_Service
{
    public abstract class WebServiceHandler
    {
        public abstract void Start(SettingParameter setting);
        public abstract void SetDB(DBHandler DB);
        public abstract bool Connect();
        public abstract String GetError();
        public abstract RmdService GetService();
        public abstract void Start(List<SettingParameter> lst_Setting);
        public abstract List<NewsChannel> GetAllChannels();
        public abstract void SetFrmMain(frm_Main main);
        public abstract void SetInterval(int iInterval);
        public abstract void SetMaxAge(String strMaxAge);
    }
}
