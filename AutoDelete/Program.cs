//////////////////////////////////////////////WinForm Version//////////////////////////////////////////////
/**/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace MyNewService
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new AutoDeleteHiddenForm());
        }
    }
}


//////////////////////////////////////////////WinService Version//////////////////////////////////////////////
/*
using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading;

namespace MyNewService
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            ServiceBase[] ServicesToRun;
            ServicesToRun = new ServiceBase[] 
                    { 
                        new AutoDelete() 
                    };
            ServiceBase.Run(ServicesToRun);
        }
    }
}
*/