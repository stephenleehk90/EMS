using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Reuters_with_Web_Service.com.reuters.rmb;
using System.Net;
using System.Xml.Linq;

namespace Reuters_with_Web_Service
{
    public partial class Form1 : Form
    {
        RmdService service;

        public Form1()
        {
            InitializeComponent();
            try 
            {
                String strAuth = "89G/7WtVWIoDPrW+vNr2Jq9Jowzsq7CoyfbfQ+HdRaQ="; //GetReutersLoginAuth();
                service = new RmdService();
                service.Url = "http://rmb.reuters.com/rmd/soap/xml/?token=" + strAuth;
                CheckServiceLoginTimeout();
                String[] str = { "" };
                results res = new results();
                availableChannels channels = new availableChannels();
                channels = service.getChannels(new String[] { "" }, new String[] { "TXT" , "PIC", "NGS", ""});
                List<channelInformation> channelInfo = channels.channelInformation.ToList();
                res = service.getItems("2013.08.20.00.00-2013.08.21.00.00", "", new String[] { "Cbl069" }, new String[] { "T" }, 300, true, new String[] { "TXT" }, "default", 100, true, false, true, null);
                //res = service.getItems("2013.08.01-2013.08.09", "7D", 
                res = service.getItems(null, null, new String[] { "xbe144" }, new String[] { "P" }, 300, true, new String[] { "PIC" }, "default", 100, true, false, true, null);
                result re = new result();
                String str1 = service.getItem(res.result[0].id, res.result[0].channel[0].ToString(), null, null);
            }catch(Exception ex)
            {

            }
            ReadPath();
        }

        private bool CheckServiceLoginTimeout()
        {
            String response;
            try
            {
                using (WebClient wc = new WebClient())
                {
                    response = wc.DownloadString(service.Url);
                }
                if (response.Length > 0)
                {
                    XDocument doc = XDocument.Parse(response);
                    response = doc.Element("authToken").Value;
                }
            }
            catch (Exception ex)
            {
            }
            return true;
        }

        private String GetReutersLoginAuth()
        {
            String response;
            //"https://commerce.reuters.com/rmd/rest/xml/login?username=" + "takungpao_rccd" + "&password=" + "Rtrt4Kun6PA0";
            using (WebClient wc = new WebClient())
            {
                response = wc.DownloadString("https://commerce.reuters.com/rmd/rest/xml/login?username=takungpao_rccd&password=Rtrt4Kun6PA0");
            }
            if (response.Length > 0)
            {
                XDocument doc = XDocument.Parse(response);
                response = doc.Element("authToken").Value;
            }

            return response;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ReadPath();
        }

        private void ReadPath()
        {
            List<String> lstFileName = new List<String>();
            //String strPath = "D:\\Work\\New_Reuters\\Reuters-NewsML-G2\\Asian Business News";
            String strPath = "D:\\Work\\New_Reuters\\Reuters-NewsML-G2\\English Language News Graphics Service";
            DirectoryInfo dirInfo = new DirectoryInfo(strPath);
            for (int i = 0; i < dirInfo.GetFiles().Count(); i++)
                lstFileName.Add(dirInfo.GetFiles()[i].FullName);

            for (int i = 0; i < lstFileName.Count; i++)
            {
                XML_Reader reader = new XML_Reader(lstFileName[i].ToString(), false);
                reader.Read();
            }
        }
    }
}
