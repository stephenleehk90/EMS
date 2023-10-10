/*jslint vars: true, plusplus: true, devel: true, nomen: true, regexp: true, indent: 4, maxerr: 50 */
/*global $, window, location, CSInterface, SystemPath, themeManager*/

function bMP(){
    var isMP = false;
    return isMP;
};

function bHKET(){
    var bHKET = false;
    return bHKET;
};

var csInterface = new CSInterface();
var web;
var server;
var username;
var password;
var enPassword;
var role;
var _is_login;
var msg;
var action;

(function () {
    'use strict';

    
    function init() {
                
        themeManager.init();
    }
        
    init();
    csInterface.evalScript('loadLib()');

    csInterface.addEventListener("SP_UpdateFinishRecord", function (event){
        var col = event.data.split(",");
        var param = "column_id=" + col[0] + "&column_date=" + col[1] + "&style=" + col[2] + "&computer=";
        Load("SP_UpdateFinishRecord",param,function(){})
    });
    
    csInterface.addEventListener("yesNoCancelDialog", function (layoutdata){
        var layouttext = layoutdata.data.split(",");
        csInterface.requestOpenExtension("founder.Compose.yesNoCancelDialog");
        function layouttextdata(event){
            csInterface.removeEventListener("need layout text", layouttextdata);
            setTimeout( function(){
                var event = new CSEvent("return layout text", "APPLICATION");
                var data = new Object();
                data.label = layouttext[0];
                data.yes = layouttext[1];
                data.no = layouttext[2];
                data.cancel = layouttext[3];
                event.data = data;
                csInterface.dispatchEvent(event);
            },400);
        }
        csInterface.removeEventListener("need layout text", layouttextdata);
        csInterface.addEventListener("need layout text", layouttextdata);

        function yesNoResult(result){
            setTimeout( function(){
                csInterface.removeEventListener("return YesNoCancel Result", yesNoResult);
                var data = result.data + "," + layouttext[4] + "," + layouttext[5] + "," + layouttext[6];
                csInterface.evalScript('yesNoCancelResult("' + data + '")');
            },400);
        }
        csInterface.removeEventListener("return YesNoCancel Result", yesNoResult);
        csInterface.addEventListener("return YesNoCancel Result", yesNoResult);
    });

    csInterface.addEventListener("GetKeyWordsByFileCode", function (event){
        var col = event.data.split(",");
        var tagServer = col[2];
        var str = "strFileCode=" + col[0] + "&strDBUser=" + username + "&strDBPass=" + enPassword;

        ErrorLog("GetKeyWordsByFileCode  " + str ,true ,0);
        if (tagServer.indexOf("http") < 0)
            tagServer = "http://"+tagServer;

        var theUrl = tagServer + "/GetKeyWordsByFileCode_ID";
        theUrl += "?" + str;
        
        var xmlHttp = new XMLHttpRequest();
        
        xmlHttp.addEventListener("load",function complete(){
            var parser = new DOMParser();
            var xmlDoc = parser.parseFromString(xmlHttp.response,"text/xml"); 
            var x = xmlDoc.getElementsByTagName("Keyword");
            var tag = "";
            for (var i = 0; i < x.length ;i++) {
                if (tag != "")
                    tag += "\t"
                tag += x[i].innerHTML;
            }
            csInterface.evalScript('GetKeyWordsByFileCodeSucess("' + tag + ',' + col[1] + ',' + col[0] + '")');
        });
    
        xmlHttp.addEventListener("error",function error(event){
            ErrorLog("Tagging Error (web service error) ---- " + event.target.status,false,5);
            csInterface.evalScript('GetKeyWordsByFileCodeSucess("Tagging Error,' + col[0] + '")');
        });
    
        xmlHttp.open("GET", theUrl);
        xmlHttp.send();
    });
}());

function SetServer(server)
{
    this.server = server;
};

function GetMsg()
{
    return msg;
};

function SetLoginData(loginData)
{
    this.server = loginData.server;
    this.username = loginData.username;
    this.password = loginData.password;
    this.enPassword = loginData.enPassword;
    this.role = loginData.role;
    this.action = loginData.action;
};

function GetLoginData()
{
    var loginData = new Object();
    loginData.server = server;
    loginData.username = username;
    loginData.password = password;
    loginData.enPassword = enPassword;
    loginData.role = role;
    loginData.action = action;
    return loginData;
};

function IsLogin()
{
    return _is_login;
};

function Login(username,password,callback){
    web = this;
    web.username=username;
    web.password=password;
    
    Load("login",null,function(text){
        msg = text;
        _is_login=(msg=="1");
        if(!_is_login){
            web.username="";
            web.password="";
            callback();
        }
        else
        {
            Load("decode_password",null,function(result){
                result = result.substr(0, result.indexOf('\r\n'));
                web.enPassword=result;
                callback();
            });
        }
    });
};

function Load(path,source,callback){
    var theUrl = 'http://' + server + '/' + path + '.aspx?username=' + username + '&password=' + password;
    if (source != null)
        theUrl += "&" + source;
    if (bMP())
        theUrl += "&company=MP"
    
    var xmlHttp = new XMLHttpRequest();
    
    xmlHttp.addEventListener("load",function complete(){
        callback(xmlHttp.responseText);
    });

    xmlHttp.addEventListener("error",function io_error(event){
        alert(xmlHttp.responseText);
    });

    xmlHttp.open("GET", theUrl);
    xmlHttp.send();
};

function SubDir(subdir, date, code){
    switch(subdir){
        case "0DATE\\CODE":
            return date + "\\" + code;
            break;
        case "1CODE\\DATE":
            return code + "\\" + date;
            break;
        case "2CODE":
            return code;
            break;
        case "3DATE":
            return date;
            break;
    }
    return "";
}

function FormatDate(fmt, date){
    var dd = date.day;
    var mm = date.month;
    var yyyy = date.year;
    switch (fmt){
        case "0NONE":
            return "";
            break;
        case "1MM$DD":
            return mm+"$"+dd; 					
            break;
        case "2MMDDYYYY":
            return mm+dd+yyyy;
            break;
        case "3DDMMYYYY":
            return dd+mm+yyyy;
            break;
        case "4YYYYMMDD":
            return yyyy+mm+dd;
            break;
        case "5DD":
            return dd;
            break;
    }
    return "";
}

function TemplateFilesPath(paper_id, xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];
    var f;
    
    var bx_server;
    if (x.getElementsByTagName("bx_server_" + paper_id).length > 0)
        bx_server = x.getElementsByTagName("bx_server_" + paper_id)[0].innerHTML;
    else
        bx_server = x.getElementsByTagName("bx_server")[0].innerHTML;
    if (bx_server.toString().substr(1,1)==":" )
    {
        f = bx_server;
    }
    else
    {
        f = "\\\\"+bx_server;
    }
    var bx_share;
    if (x.getElementsByTagName("bx_share_" + paper_id).length > 0)
        bx_share = x.getElementsByTagName("bx_share_" + paper_id)[0].innerHTML;
    else
        bx_share = x.getElementsByTagName("bx_share")[0].innerHTML;
    var bx_path;
    if (x.getElementsByTagName("bx_path_" + paper_id).length > 0)
        bx_path = x.getElementsByTagName("bx_path_" + paper_id)[0].innerHTML;
    else
        bx_path = x.getElementsByTagName("bx_path")[0].innerHTML;
    
    return f + "\\" + bx_share+ "\\" + bx_path;
}

function PaperPS(paper_id, xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];
    var f;

    var paper_ps_server;
    
    if (x.getElementsByTagName("paper_ps_server_" + paper_id).length > 0)
        paper_ps_server = x.getElementsByTagName("paper_ps_server_" + paper_id)[0].innerHTML;
    else
        paper_ps_server = x.getElementsByTagName("paper_ps_server")[0].innerHTML;
    if (paper_ps_server.toString().substr(1,1)==":" )
    {
        f = paper_ps_server;
    }
    else
    {
        f = "\\\\"+paper_ps_server;
    }
    var paper_ps_share;
    if (x.getElementsByTagName("paper_ps_share_" + paper_id).length > 0)
        paper_ps_share = x.getElementsByTagName("paper_ps_share_" + paper_id)[0].innerHTML;
    else
        paper_ps_share = x.getElementsByTagName("paper_ps_share")[0].innerHTML;
    var paper_ps_path;
    if (x.getElementsByTagName("paper_ps_path_" + paper_id).length > 0)
        paper_ps_path = x.getElementsByTagName("paper_ps_path_" + paper_id)[0].innerHTML;
    else
        paper_ps_path = x.getElementsByTagName("paper_ps_path")[0].innerHTML;
    return f + "\\" + paper_ps_share + "\\" + paper_ps_path;
}

function PaperPSPath(date, code, paper_id, xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];

    var paper_ps_subdir;
    if (x.getElementsByTagName("paper_ps_subdir_" + paper_id).length > 0)
        paper_ps_subdir = x.getElementsByTagName("paper_ps_subdir_" + paper_id)[0].innerHTML;
    else
        paper_ps_subdir = x.getElementsByTagName("paper_ps_subdir")[0].innerHTML;
    var paper_ps_datefmt;
    if (x.getElementsByTagName("paper_ps_datefmt_" + paper_id).length > 0)
        paper_ps_datefmt = x.getElementsByTagName("paper_ps_datefmt_" + paper_id)[0].innerHTML;
    else
        paper_ps_datefmt = x.getElementsByTagName("paper_ps_datefmt")[0].innerHTML;

    var subdir = SubDir(paper_ps_subdir, FormatDate(paper_ps_datefmt, date), code);
    return PaperPS(paper_id, xmlDoc) + "\\" + subdir;
}

function PS(paper_id, xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];
    var f;
    var ps_server;

    if (x.getElementsByTagName("ps_server_" + paper_id).length > 0)
        ps_server = x.getElementsByTagName("ps_server_" + paper_id)[0].innerHTML;
    else
        ps_server = x.getElementsByTagName("ps_server")[0].innerHTML;
    if (ps_server.toString().substr(1,1)==":" )
    {
        f = ps_server;
    }
    else
    {
        f = "\\\\"+ps_server;
    }
    
    var ps_share;
    if (x.getElementsByTagName("ps_share_" + paper_id).length > 0) 
        ps_share = x.getElementsByTagName("ps_share_" + paper_id)[0].innerHTML;
    else
        ps_share = x.getElementsByTagName("ps_share")[0].innerHTML;
    var ps_path;
    if (x.getElementsByTagName("ps_path_" + paper_id).length > 0)
        ps_path = x.getElementsByTagName("ps_path_" + paper_id)[0].innerHTML;
    else
        ps_path = x.getElementsByTagName("ps_path")[0].innerHTML;
    return f + "\\" + ps_share + "\\" + ps_path;
}

function PSPath(date, code, paper_id, xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];

    var ps_subpath;
    if (x.getElementsByTagName("ps_subpath_" + paper_id).length > 0)
        ps_subpath = x.getElementsByTagName("ps_subpath_" + paper_id)[0].innerHTML;
    else
        ps_subpath = x.getElementsByTagName("ps_subpath")[0].innerHTML;
    var ps_datefmt;
    if (x.getElementsByTagName("ps_datefmt_" + paper_id).length > 0)
        ps_datefmt = x.getElementsByTagName("ps_datefmt_" + paper_id)[0].innerHTML;
    else
        ps_datefmt = x.getElementsByTagName("ps_datefmt")[0].innerHTML;
    var subdir = SubDir(ps_subpath, FormatDate(ps_datefmt, date), code);
    return PS(paper_id, xmlDoc) + "\\" + subdir;
}

function PDF(paper_id, xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];
    var f;
    var ps_pdf_server;

    if (x.getElementsByTagName("ps_pdf_server_" + paper_id).length > 0)
    ps_pdf_server = x.getElementsByTagName("ps_pdf_server_" + paper_id)[0].innerHTML;
    else
    ps_pdf_server = x.getElementsByTagName("ps_pdf_server")[0].innerHTML;
    if (ps_pdf_server.toString().substr(1,1)==":" )
    {
        f = ps_pdf_server;
    }
    else
    {
        f = "\\\\"+ps_pdf_server;
    }

    ErrorLog(ps_pdf_server,true,0);
    var ps_pdf_share;
    if (x.getElementsByTagName("ps_pdf_share_" + paper_id).length > 0) 
    ps_pdf_share = x.getElementsByTagName("ps_pdf_share_" + paper_id)[0].innerHTML;
    else
    ps_pdf_share = x.getElementsByTagName("ps_pdf_share")[0].innerHTML;
    var ps_pdf_path;
    if (x.getElementsByTagName("ps_pdf_path_" + paper_id).length > 0)
    ps_pdf_path = x.getElementsByTagName("ps_pdf_path_" + paper_id)[0].innerHTML;
    else
    ps_pdf_path = x.getElementsByTagName("ps_pdf_path")[0].innerHTML;
    return f + "\\" + ps_pdf_share + "\\" + ps_pdf_path;
}

function PDFPath(date,code,paper_id, xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];
    var ps_same_setting = 1;
    if (x.getElementsByTagName("ps_same_setting").length > 0)
        ps_same_setting = x.getElementsByTagName("ps_same_setting")[0].innerHTML;

    var ps_pdf_subpath;
    var ps_pdf_datefmt;
    var subdir;
    if (ps_same_setting == "0")
    {
        if (x.getElementsByTagName("ps_pdf_subpath_" + paper_id).length > 0)
            ps_pdf_subpath = x.getElementsByTagName("ps_pdf_subpath_" + paper_id)[0].innerHTML;
        else
            ps_pdf_subpath = x.getElementsByTagName("ps_pdf_subpath")[0].innerHTML;
        if (x.getElementsByTagName("ps_pdf_datefmt_" + paper_id).length > 0)
            ps_pdf_datefmt = x.getElementsByTagName("ps_pdf_datefmt_" + paper_id)[0].innerHTML;
        else
            ps_pdf_datefmt = x.getElementsByTagName("ps_pdf_datefmt")[0].innerHTML;
        subdir = SubDir(ps_pdf_subpath, FormatDate(ps_pdf_datefmt, date), code);
        return PDF(paper_id, xmlDoc) + "\\" + subdir;
    }
    else
    {
        if (x.getElementsByTagName("ps_subpath_" + paper_id).length > 0)
            ps_pdf_subpath = x.getElementsByTagName("ps_subpath_" + paper_id)[0].innerHTML;
        else
            ps_pdf_subpath = x.getElementsByTagName("ps_subpath")[0].innerHTML;
        if (x.getElementsByTagName("ps_datefmt_" + paper_id).length > 0)
            ps_pdf_datefmt = x.getElementsByTagName("ps_datefmt_" + paper_id)[0].innerHTML;
        else
            ps_pdf_datefmt = x.getElementsByTagName("ps_datefmt")[0].innerHTML;
        subdir = SubDir(ps_pdf_subpath, FormatDate(ps_pdf_datefmt, date), code);
        return PS(paper_id, xmlDoc) + "\\" + subdir;
    }
}

function TagWebService(xmlDoc){
    var x = xmlDoc.getElementsByTagName("newsroom")[0];
    var tagWebService = "";
    if (x.getElementsByTagName("TagWebServiceURL").length > 0)
        tagWebService = x.getElementsByTagName("TagWebServiceURL")[0].innerHTML;
    return tagWebService;
}

function ErrorLog(strLogMsg, bDebug, itype){
    csInterface.evalScript("ErrorLog('" + strLogMsg + "', '" + bDebug + "', '" + itype + "')");
}