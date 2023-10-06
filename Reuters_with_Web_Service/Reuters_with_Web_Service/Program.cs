using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Reuters_with_Web_Service
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
            //Application.Run(new Form1());
            Application.Run(new frmLogin());
            //Application.Run(new frm_Main());
            //Application.Run(new frmSetParameter());
        }
    }
}
