/*jslint vars: true, plusplus: true, devel: true, nomen: true, regexp: true, indent: 4, maxerr: 50 */
/*global $, Folder*/
function bMP(){   
    var isMP = false;
    return isMP;
};

function bHKET(){   
    var bHKET = false;
    return bHKET;
};

var bLogin = false;
var pretypeset_list = {};
var bRun = false;
var typeset_list = {};
var currentTypeset;
var Setting;
var user;

var bAfterSaveCall = true;

function loadLib() {  
    try {  
        if (!loadLib.xLib) loadLib.xLib = new ExternalObject('lib:PlugPlugExternalObject');  
    } catch (e) {  
        alert('Drat\n' + e);  
    }  
}

function createMenu(){
    //<fragment>
    var mainMenu =  app.menus.item("$ID/Main").submenus.item("組版");
    if ( mainMenu != null && mainMenu.isValid) { 
        if (mainMenu.menuElements.length == 0)
        {
            removeMenu();
            addMenu();
        }
    }
    else
        addMenu();
    //</fragment>
}

function addMenu(){
    //<fragment>
    var mainMenu = app.menus.item("$ID/Main").submenus.add("組版");

    var startComposeAction = app.scriptMenuActions.add("開始組版");
    var startComposeEventListener = startComposeAction.eventListeners.add("onInvoke", startComposeOnInvokeHandler, false);
    var startComposeMenuItem = mainMenu.menuItems.add(startComposeAction);

    var finishComposeAction = app.scriptMenuActions.add("組版完畢");
    var finishComposeEventListener = finishComposeAction.eventListeners.add("onInvoke", finishComposeOnInvokeHandler, false);
    var finishComposeMenuItem = mainMenu.menuItems.add(finishComposeAction);

    var preComposeAction = app.scriptMenuActions.add("預組版");
    var preComposeEventListener = preComposeAction.eventListeners.add("onInvoke", preComposeOnInvokeHandler, false);
    var preComposeMenuItem = mainMenu.menuItems.add(preComposeAction);

    var exportJpgAction = app.scriptMenuActions.add("生成大樣圖片");
    var exportJpgEventListener = exportJpgAction.eventListeners.add("onInvoke", exportJpgOnInvokeHandler, false);
    var exportJpgMenuItem = mainMenu.menuItems.add(exportJpgAction);

    mainMenu.menuSeparators.add();

    var changePasswordAction = app.scriptMenuActions.add("修改個人密碼");
    var changePasswordEventListener = changePasswordAction.eventListeners.add("onInvoke", changePasswordOnInvokeHandler, false);
    var changePasswordMenuItem = mainMenu.menuItems.add(changePasswordAction);

    var changeRoleAction = app.scriptMenuActions.add("改變身份");
    var changeRoleEventListener = changeRoleAction.eventListeners.add("onInvoke", changeRoleOnInvokeHandler, false);
    var changeRoleMenuItem = mainMenu.menuItems.add(changeRoleAction);

    var settingAction = app.scriptMenuActions.add("設置");
    var settingEventListener = settingAction.eventListeners.add("onInvoke", settingOnInvokeHandler, false);
    var settingMenuItem = mainMenu.menuItems.add(settingAction);

    var logoutAction = app.scriptMenuActions.add("登出");
    var logoutEventListener = logoutAction.eventListeners.add("onInvoke", logoutOnInvokeHandler, false);
    var logoutMenuItem = mainMenu.menuItems.add(logoutAction);
    
    mainMenu.menuSeparators.add();

    var aboutAction = app.scriptMenuActions.add("關於採編軟插件");
    var aboutEventListener = aboutAction.eventListeners.add("onInvoke", aboutOnInvokeHandler, false);
    var aboutMenuItem = mainMenu.menuItems.add(aboutAction);

    mainMenu.eventListeners.add("beforeDisplay", myBeforeDisplayHandler, false);
    //</fragment>
}

function myBeforeDisplayHandler(myEvent){
    var startComposeAction = app.scriptMenuActions.item("開始組版");
    var finishComposeAction = app.scriptMenuActions.item("組版完畢");
    var preComposeAction = app.scriptMenuActions.item("預組版");
    var exportJpgAction = app.scriptMenuActions.item("生成大樣圖片");
    var changePasswordAction = app.scriptMenuActions.item("修改個人密碼");
    var changeRoleAction = app.scriptMenuActions.item("改變身份");
    var settingAction = app.scriptMenuActions.item("設置");
    var logoutAction = app.scriptMenuActions.item("登出");
    var aboutAction = app.scriptMenuActions.item("關於採編軟插件");

    finishComposeAction.enabled = false;
    exportJpgAction.enabled = false;
    if (bLogin)
    {
        changeRoleAction.enabled = true;
        logoutAction.enabled = true;

        if (app.documents.length == 0)
            return;
        var doc=app.activeDocument;
        
        if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
            return;
        
        var typeset=typeset_list[doc.fullName.fsName];
        if(typeset!=null){
            finishComposeAction.enabled = true;
            exportJpgAction.enabled = true;
        }
    }
    else
    {
        changeRoleAction.enabled = false;
        logoutAction.enabled = false;
    }
}

function startComposeOnInvokeHandler(myEvent){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "Start Compose";
    eventObj.dispatch();
}

function finishComposeOnInvokeHandler(myEvent){
    ComposeComplete();
}

function preComposeOnInvokeHandler(myEvent){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "PreCompose";
    eventObj.dispatch();
}

function exportJpgOnInvokeHandler(myEvent){
    try{
        if (app.documents.length == 0)
            return;
        var doc=app.activeDocument;
        
        if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
            return;
        
        var typeset=typeset_list[doc.fullName.fsName];
        var jpg_path;
        var jpg_file;
        LoadSettingXML();
        if (bMP())
        {
            var date = FormatDate(Setting.dateFmt, typeset.date);
            if (Setting.subPath == 1)
            {
                jpg_path = Setting.local_temp + "\\" + typeset.layout_code;
                if (date != "")
                    jpg_path += "\\" + date;
            }
            else if (Setting.subPath == 2)
            {
                jpg_path=Setting.local_temp + "\\" + typeset.layout_code;
            }
            else if (Setting.subPath == 3)
            {
                jpg_path=Setting.local_temp;
                if (date != "")
                    jpg_path += "\\" + date;
            }
            else
            {
                jpg_path=Setting.local_temp;
                if (date != "")
                    jpg_path += "\\" + date;
                jpg_path+= "\\" + typeset.layout_code;
            }
        }
        else
        {
            jpg_path=Setting.local_temp + "\\" + typeset.layout_code;
        }
        var filename = app.activeDocument.fullName.name;
        filename = filename.substr(0, filename.lastIndexOf("."))  +".jpg";
        jpg_file = File(jpg_path  + "\\" + filename);
        app.jpegExportPreferences.jpegQuality=[JPEGOptionsQuality.MAXIMUM, JPEGOptionsQuality.HIGH, JPEGOptionsQuality.MEDIUM,JPEGOptionsQuality.LOW][Setting.jpg_quality-1]; 
        app.jpegExportPreferences.exportResolution=Setting.jpg_ppi;
        app.jpegExportPreferences.jpegExportRange = ExportRangeOrAllPages.EXPORT_ALL;
        app.jpegExportPreferences.exportingSpread = true;
        
        try{
            doc.exportFile(ExportFormat.jpg,jpg_file);
            
            loadLib();
            var eventObj = new CSXSEvent();
            eventObj.type = "Export Jpg";
            eventObj.data = jpg_file.fsName + "|exportJpg|" + typeset.PSPath;
            eventObj.dispatch();
        } catch(e) {
            alert("警告，偵測到 InDesign 運行環境出現異常！繼續進行[生成大樣]會有機會出現不可預測的錯誤。強烈建議重啓 InDesign。", "Warning");
        }
    }
    catch(e){ErrorLog(e.message,true,0)}
}

function changePasswordOnInvokeHandler(myEvent){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "Open Change Password Dialog";
    eventObj.dispatch();
}

function changeRoleOnInvokeHandler(myEvent){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "Open Change Role Dialog";
    eventObj.dispatch();
}

function settingOnInvokeHandler(myEvent){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "Open Setting Dialog";
    eventObj.dispatch();
}

function logoutOnInvokeHandler(myEvent){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "Logout";
    eventObj.dispatch();
}

function aboutOnInvokeHandler(myEvent){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "Open About Dialog";
    eventObj.dispatch();
}

function openLoadingDialog(){
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "Open Loading Dialog";
    eventObj.dispatch();
}

function removeMenu(){
    var mainMenu =  app.menus.item("$ID/Main").submenus.item("組版");
    if ( mainMenu != null && mainMenu.isValid) {
        mainMenu.remove();
    }
}

function setLogin(isLogin)
{
    if (isLogin == "true")
    {
        bLogin = true;
    }
    else
    {
        for (var i = 0; i< app.documents.length; i++)
        {
            var doc = app.documents.item(i);
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                continue;
            var t=typeset_list[doc.fullName.fsName];
            var p=pretypeset_list[doc.fullName.fsName];
            if(t!=null || p != null)
            {
                bQuit = false;
                alert("請先關閉所有組版中的版面，再登出。");
                return "false";
            }
        }
        bLogin = false;
        return "true";
    }
}

function getJPEGExportResolution(){
    return app.jpegExportPreferences.exportResolution.toString();
}

function getJPEGQuality(){
    var jpg_quality=4;
    switch(app.jpegExportPreferences.jpegQuality)
    {
        case JPEGOptionsQuality.MAXIMUM: jpg_quality=1; break;
        case JPEGOptionsQuality.HIGH: jpg_quality=2; break;
        case JPEGOptionsQuality.MEDIUM: jpg_quality=3; break;
        case JPEGOptionsQuality.LOW: jpg_quality=4; break;
        default: jpg_quality=4;
    }
    return jpg_quality.toString();
}

function getPdfExportPresets(){
    var ps = app.pdfExportPresets;
    var str = "";
    for (var i=0; i<ps.length;i++)
         str += ps.item(i).name + "_|_";
    return str;
}

function getFonts(){
    var systemFonts = app.fonts.everyItem().getElements();
    var str = "";
    for (var i=0; i<systemFonts.length;i++)
        str += systemFonts[i].name + "_|_";
    return str;
}

var filelock = [];
function lockFile(str) {
    try{
        var inddfile = File(str);
        if(inddfile.exists){
            inddfile.open("a");
            if (inddfile.error != ""){
                ErrorLog(inddfile.error,true,0);
                return "false";
            }
        }

        filelock[str] = File(str + "_lock");
        if(filelock[str].exists){
            if (!filelock[str].remove())
                return "false";
        }

        filelock[str].open("a");
        return "true";
    }
    catch(err) {
        ErrorLog(err.message,true,0);
        return "false";
    }
}

function unlockFile(str) {
    try
    {
        if(filelock[str].exists){
            filelock[str].close();
            filelock[str].remove();
            if (filelock[str].error != ""){
                ErrorLog(filelock[str].error,true,0);
                return "false";
            }
            return "true";
        }
    }
    catch(err) {
        ErrorLog(err.message,true,0);
        return "false";
    }
}

function getDocName() {
    if (app.documents.length == 0)
        return;
    
    var doc = app.activeDocument;
    if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.name == undefined || doc.name == null)
        return;

    return doc.fullName.fsName
}

function preTypesetOpenDoc(template, column_id, local_file, server_file, column_date, username){
    try{
        bAfterSaveCall = true;
        user = username;
        var local = File(local_file);
            
        var mDate;
        var d= new Date();
        var doc;
        
        if(local.exists){
            doc=app.open(local);
            mDate = local.modified;
        }else{
            if(template==""){
                doc=app.documents.add();
            }else{
                doc=app.open(template);
            }
            mDate = d;
        }
        
        var strDate = d.getFullYear() + "-" + ("0"+(d.getMonth() + 1)).slice(-2) + "-" + ("0"+d.getDate()).slice(-2) + " " + ("0"+d.getHours()).slice(-2) + ":" +("0"+d.getMinutes()).slice(-2) + ":" +("0"+d.getSeconds()).slice(-2);
        var strModificationDate = mDate.getFullYear() + "-" + ("0"+(mDate.getMonth() + 1)).slice(-2) + "-" + ("0"+mDate.getDate()).slice(-2) + " " + ("0"+mDate.getHours()).slice(-2) + ":" +("0"+mDate.getMinutes()).slice(-2) + ":" +("0"+mDate.getSeconds()).slice(-2);

        var count = app.activeDocument.extractLabel('count');
        if (count == undefined)
            count = 0;

        app.activeDocument.insertLabel((count+1).toString(), strDate + "," + strModificationDate + "," + user + ",pre-compose");
        app.activeDocument.insertLabel('count',(count+1).toString());
        doc.save(local);
    
        if (local.size!=0)
            local.copy(server_file);
        if (local.size != server_file.size && local.size!=0)
            local.copy(server_file);
        if (server_file.size == 0)
        {
            alert("複製文件到伺服器時出錯，請稍候再試!","Warning");
            return;
        }
        
        var before_save=function(evt){
            
            var doc;
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc=evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.name == undefined || doc.name == null)
                return;
            
            var typeset= pretypeset_list[doc.fullName.fsName];
            if(typeset != undefined && typeset!=null) {
                try
                {
                    var count = Number(app.activeDocument.extractLabel('count'));
                    var d = new Date();
                    strDate = d.getFullYear() + "-" + ("0"+(d.getMonth() + 1)).slice(-2) + "-" + ("0"+d.getDate()).slice(-2) + " " + ("0"+d.getHours()).slice(-2) + ":" +("0"+d.getMinutes()).slice(-2) + ":" +("0"+d.getSeconds()).slice(-2);
                    
                    app.activeDocument.insertLabel((count+1).toString(), strDate + "," + strDate + "," + user + ",pre-compose save");
                    app.activeDocument.insertLabel('count',(count+1).toString());
                }
                catch (e)
                {
                    
                }
            }
        };
        
        if (!bRun)
            app.addEventListener("beforeSave",before_save);
        
        var after_save=function(evt){
            var local = File(local_file);
            var doc;
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc=evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.name == undefined || doc.name == null)
                return;
            
            var typeset= pretypeset_list[doc.fullName.fsName];
            if(typeset != undefined && typeset!=null){
                try
                {
                    if (local.size!=0)
                        local.copy(server_file);
                    if (local.size != server_file.size && local.size!=0)
                        local.copy(server_file);
                    if (server_file.size == 0)
                    {
                        alert("複製文件到伺服器時出錯，請稍候再試!");
                        return;
                    }
                }
                catch (e)
                {
                    alert("警告，偵測到 InDesign 運行環境出現異常！繼續進行[預組版]會有機會出現不可預測的錯誤。強烈建議重啓 InDesign。");
                }
            }
        };
        
        if (!bRun)
            app.addEventListener("afterSave",after_save);
        
        pretypeset_list[doc.fullName.fsName]=column_id+','+column_date;
        
        var after_close=function(evt){
            var doc;
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc=evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.name == undefined || doc.name == null)
                return;
                
            if(pretypeset_list[doc.fullName.fsName]!=undefined && pretypeset_list[doc.fullName.fsName]!=null) {
                try
                {
                    var data = pretypeset_list[doc.fullName.fsName];
                    delete pretypeset_list[doc.fullName.fsName];
                    
                    loadLib();
                    var eventObj = new CSXSEvent();
                    data += ",P"
                    eventObj.type = "SP_UpdateFinishRecord";
                    eventObj.data = data;
                    eventObj.dispatch();
                }
                catch (e)
                {
                    alert("警告，偵測到 InDesign 運行環境出現異常！繼續進行[預組版]會有機會出現不可預測的錯誤。強烈建議重啓 InDesign。");
                }
            }
        };
        
        if (!bRun)
            app.addEventListener("afterClose",after_close);
        
        bRun = true;
    }catch(e)
    {
        ErrorLog(e.message,true,0);
    }
}

function TypesetOpenDoc(template, column_id, local_file, server_file, column_date, filename, username, layout_code, year, month, day, PSPath, PDFPath, PaperPSPath, tagServer){
    try {
        bAfterSaveCall = true;
        user = username;
        var ts =  new Object();
		ts.template = template;
		ts.column_id = column_id;
		ts.local_file = local_file;
		ts.server_file = server_file;
		ts.column_date = column_date;
		ts.filename = filename;
        ts.layout_code = layout_code;
        ts.date = new Object();
		ts.date.year = year;
		ts.date.month = month;
		ts.date.day = day;
		ts.PSPath = PSPath;
        ts.PDFPath = PDFPath;
        ts.PaperPSPath = PaperPSPath;
		ts.tagServer = tagServer;
        var server = File(server_file)
        try
        {
            if (lockFile(server_file) == "false"){
                if (bMP()){
                    alert("版面檔案正在使用中導致無法繼續，請稍候再試。");
                }
                else{
                    alert("該版面正在使用中，請稍候再試。");
                }
                return;
            }
            ErrorLog("Check if the file (" + server_file + ") is in use - lock file (open document)",false,7);
        }
        catch(e)
        {
            ErrorLog("The file (" + server_file + ") is in use - lock file fail (open document)",false,7);
            try{
                ErrorLog("--------------該版面正在使用中，請稍候再試。------------------------",true,0);
                ErrorLog(e.message + " OpenDocument",true,0);
                if(server != null)
                    ErrorLog(server_file,true,0);
                ErrorLog("--------------該版面正在使用中，請稍候再試。------------------------",true,0);
            }catch(e){}
            if (bMP()){
                alert("版面檔案正在使用中導致無法繼續，請稍候再試。");
            }
            else{
                alert("該版面正在使用中，請稍候再試。");
            }
            return;
        }
        var local = File(local_file);
        var d = new Date();
        var mDate;
        var doc;
        
        if(local.exists){
            doc=app.open(local);
            mDate = local.modified;
        }else{
            if(template==""){
                doc=app.documents.add();
            }else{
                doc=app.open(template);
            }
            mDate = d;
        }
        var strDate = d.getFullYear() + "-" + ("0"+(d.getMonth() + 1)).slice(-2) + "-" + ("0"+d.getDate()).slice(-2) + " " + ("0"+d.getHours()).slice(-2) + ":" +("0"+d.getMinutes()).slice(-2) + ":" +("0"+d.getSeconds()).slice(-2);
        var strModificationDate = mDate.getFullYear() + "-" + ("0"+(mDate.getMonth() + 1)).slice(-2) + "-" + ("0"+mDate.getDate()).slice(-2) + " " + ("0"+mDate.getHours()).slice(-2) + ":" +("0"+mDate.getMinutes()).slice(-2) + ":" +("0"+mDate.getSeconds()).slice(-2);
        
        var count = Number(app.activeDocument.extractLabel('count'));
        if (count > 0)
        {
            var label = app.activeDocument.extractLabel(String(count));
            var dateStringArray = label.split(",");
            var d2 = new Date();
            var tempDate =  dateStringArray[1].replace("-", "/");
            tempDate = tempDate.replace("-", "/")
            d2.setTime(Date.parse(tempDate));
            d2.setTime(d2.getTime()+60000);
            var strDate2 = d2.getFullYear() + "-" + ("0"+(d2.getMonth() + 1)).slice(-2) + "-" + ("0"+d2.getDate()).slice(-2) + " " + ("0"+d2.getHours()).slice(-2) + ":" +("0"+d2.getMinutes()).slice(-2) + ":" +("0"+d2.getSeconds()).slice(-2);
            if ((dateStringArray[4] == "pre-compose save" || dateStringArray[4] == "pre-compose") && (strModificationDate > dateStringArray[1] && strModificationDate > strDate2))
            {
                try{
                    ErrorLog("--------------預組版後曾被修改------------------------",true,0);
                    if(local != null)
                        ErrorLog(local_file,true,0);
                    if(server_file != null)
                        ErrorLog(server_file,true,0);
                    ErrorLog("mod time :" + strModificationDate + "-- "  + dateStringArray[4]  + " time:" + dateStringArray[1]  + "-- +10s"  +  strDate2,true,0);
                    ErrorLog(column_id,true,0);
                    ErrorLog("--------------預組版後曾被修改------------------------",true,0);
                } catch (e) {}
                alert("注意：該版面在預組版後曾被修改。\r\n預組版保存時間\t：" + dateStringArray[1] + "\r檔案最後修改時間\t：" + strModificationDate);	
            }
        }
        
        app.activeDocument.insertLabel((count+1).toString(), strDate + "," + strModificationDate + "," + user + ",compose");
        ErrorLog("compose",true,0);
        app.activeDocument.insertLabel('count',(count+1).toString());
        doc.save(local);
        
        try
        {
            unlockFile(server_file);
            ErrorLog("Check if the file (" + server_file + ") is in use - unlock file (open document)",false,7);
        }
        catch (e) {
            ErrorLog("Check if the file (" + server_file + ") is in use - unlock file failed (open document)",false,7);
        }
        try
        {
            var eventObj = new CSXSEvent();
            eventObj.type = "copyFile";
            eventObj.data = local.fsName + "|" + server.fsName + "|opendocument";
            eventObj.dispatch();
        }
        catch(e) {
            try {
                ErrorLog(e.message + " 1",true,0);
                if(local != null)
                    ErrorLog(local.fsName,true,0);
                if(server != null)
                    ErrorLog(server.fsName,true,0);
            }catch(e){}
        }
        ts.modificationDate=local.modified;
        ts.bCompose = false;

        typeset_list[doc.fullName.fsName]=ts;

        var m_storyid;
        var m_path;
        var m_filecode;
        try{
            app.activeDocument.addEventListener("beforePlace",function(evt){
                try{
                    var placeObj = evt.target;
                    if (placeObj.constructor.name == "TextFrame"){
                        m_storyid = placeObj.id;
                        m_path = placeObj.parentStory.label;
                        m_filecode = placeObj.parentStory.extractLabel("filecode");
                    }
                }
                catch(e)
                {
                    alert("beforeQuit: " + e.message)
                }
            });
        }
        catch(e)
        {
            alert("error: " + e.message)
        }

        try{
            app.activeDocument.addEventListener("afterPlace",function(evt){
                try{
                    var placeObj = evt.target;
                    if (placeObj.constructor.name == "TextFrame" && m_storyid != placeObj.id){
                        placeObj.parentStory.label = m_path;
                        placeObj.parentStory.insertLabel("filecode", m_filecode);
                    }
                }
                catch(e)
                {
                    alert("beforeQuit: " + e.message)
                }
            });
        }
        catch(e)
        {
            alert("error: " + e.message)
        }

        loadLib();
        var eventObj = new CSXSEvent();
        eventObj.type = "Open Resource Dialog";
        eventObj.data = doc.fullName.fsName;;
        eventObj.dispatch();

        return doc.fullName.fsName;
    }
    catch (e){
        alert("警告，偵測到 InDesign 運行環境出現異常！繼續進行[組版]會有機會出現不可預測的錯誤。強烈建議重啓 InDesign。");
        try{
            ErrorLog(e.message + " op doc",true,0);
            if(local != null)
                ErrorLog(local_file,true,0);
            if(server_file != null)
                ErrorLog(server_file,true,0);
            ErrorLog(column_id,true,0);
            ErrorLog("label count :" + String(count),true,0);
            for (var i = 1; i <= count; i++)
            {
                var indd_label = app.activeDocument.extractLabel(String(i));
                var indd_dateStringArray = indd_label.split(",");
                
                ErrorLog(indd_dateStringArray[0],true,0);
                ErrorLog(indd_dateStringArray[1],true,0);
                ErrorLog(indd_dateStringArray[2],true,0);
                ErrorLog(indd_dateStringArray[3],true,0);
                ErrorLog(indd_dateStringArray[4],true,0);
            }
        } catch (e) {}
    }
}

function addListener(){
    function AFTER_ACTIVATE(doc){
        ReopenResourceDialog(doc.fullName.fsName);
    }

    function BEFORE_SAVE(doc,typeset){
        LoadSettingXML()
        var local = File(typeset.local_file);
        var server = File(typeset.server_file);
        ErrorLog("BEFORE_SAVE --- active doc: " + typeset.modificationDate + " server: " + server.modified + " local: " + local.modified,true,0);
        
       if(typeset.bCompose == false && Setting.save_jpg){
            ErrorLog("export jpeg",true,0);
            var jpg_path;
            var jpg_file;
            if (bMP())
            {
                var date = FormatDate(Setting.dateFmt, typeset.date);
                if (Setting.subPath == 1)
                {
                    jpg_path = Setting.local_temp + "\\" + typeset.layout_code;
                    if (date != "")
                        jpg_path += "\\" + date;
                }
                else if (Setting.subPath == 2)
                {
                    jpg_path=Setting.local_temp + "\\" + typeset.layout_code;
                }
                else if (Setting.subPath == 3)
                {
                    jpg_path=Setting.local_temp;
                    if (date != "")
                        jpg_path += "\\" + date;
                }
                else
                {
                    jpg_path=Setting.local_temp;
                    if (date != "")
                        jpg_path += "\\" + date;
                    jpg_path+= "\\" + typeset.layout_code;
                }
            }
            else
            {
                jpg_path=Setting.local_temp + "\\" + typeset.layout_code;
            }
            var filename = app.activeDocument.fullName.name;
            filename = filename.substr(0, filename.lastIndexOf("."))  +".jpg";
            jpg_file = File(jpg_path  + "\\" + filename);
            app.jpegExportPreferences.jpegQuality=[JPEGOptionsQuality.MAXIMUM, JPEGOptionsQuality.HIGH, JPEGOptionsQuality.MEDIUM,JPEGOptionsQuality.LOW][Setting.jpg_quality-1]; 
            app.jpegExportPreferences.exportResolution=Setting.jpg_ppi;
            app.jpegExportPreferences.jpegExportRange = ExportRangeOrAllPages.EXPORT_ALL;
            app.jpegExportPreferences.exportingSpread = true;

            var psFile = Folder(typeset.PSPath);
            try {
                psFile.create();
            }
            catch (e) {
                alert("生成大樣圖片路徑『"+psFile.fsName+"』錯誤，請檢查設置");
                return;
            }
            
            try{
                doc.exportFile(ExportFormat.jpg,jpg_file);
                var to_file = File(typeset.PSPath + "\\" + filename);
                
                if (Setting.jpg_preview_save) {
                    loadLib();
                    var eventObj = new CSXSEvent();
                    eventObj.type = "Export Jpg";
                    eventObj.data = jpg_file.fsName + "|beforeSave|" + typeset.PSPath;
                    eventObj.dispatch();
                }
                else{
                    ErrorLog("export jpeg complete",true,0);
                    if (jpg_file.size!=0)
                        jpg_file.copy(to_file);
                    if (jpg_file.size != to_file.size && jpg_file.size!=0)
                        jpg_file.copy(to_file);
                    if (to_file.size == 0){
                        alert("複製大樣圖片到伺服器時出錯，請稍候再試!","Warning");
                        return;
                    }
                }
            }
            catch(e)
            {
                try
                {
                    doc.exportFile(ExportFormat.jpg,jpg_file);
                    var to_file = File(typeset.PSPath + "\\" + filename);
                    
                    if (Setting.jpg_preview_save) {
                        loadLib();
                        var eventObj = new CSXSEvent();
                        eventObj.type = "Export Jpg";
                        eventObj.data = jpg_file.fsName + "|beforeSave|" + typeset.PSPath;
                        eventObj.dispatch();
                    }
                    else{
                        ErrorLog("export jpeg complete",true,0);
                        if (jpg_file.size!=0)
                            jpg_file.copy(to_file);
                        if (jpg_file.size != to_file.size && jpg_file.size!=0)
                            jpg_file.copy(to_file);
                        if (to_file.size == 0){
                            alert("複製大樣圖片到伺服器時出錯，請稍候再試!","Warning");
                            return;
                        }
                    }
                }
                catch(e){}
            }
        }
        
        var count = Number(app.activeDocument.extractLabel('count'));
        var d = new Date();
        var strDate = d.getFullYear() + "-" + ("0"+(d.getMonth() + 1)).slice(-2) + "-" + ("0"+d.getDate()).slice(-2) + " " + ("0"+d.getHours()).slice(-2) + ":" +("0"+d.getMinutes()).slice(-2) + ":" +("0"+d.getSeconds()).slice(-2);
        app.activeDocument.insertLabel((count+1).toString(), strDate + "," + strDate + "," + user + ",save");
        ErrorLog("save",true,0);
        app.activeDocument.insertLabel('count',(count+1).toString());
    }

    function AFTER_SAVE(doc,typeset){
        try
        {
            var checkPoint = 0;
            var local = File(typeset.local_file);
            var server = File(typeset.server_file);
            ErrorLog("AFTER_SAVE --- active doc: " + typeset.modificationDate + " server: " + server.modified + " local: " + local.modified,true,0);
            
            var oldModificationDate = typeset.modificationDate;
            typeset.modificationDate = local.modified;
            
            if (local.size == 0)
                doc.save();
            
            var d2 = server.modified;
            d2.setTime(d2.getTime()+60000);

            checkPoint = 1;
            if (oldModificationDate.getTime() >=server.modified.getTime() && oldModificationDate.getTime() <= d2.getTime())
            {
                try
                {
                    unlockFile(typeset.server_file);
                    ErrorLog("Unlock server file (" + server.fsName + ") - After save local - before copy to server",false,7);
                }
                catch (e) {
                    ErrorLog("Unlock server file (" + server.fsName + ") failed - After save local - before copy to server",false,7);
                }
                checkPoint = 2;

                if (bAfterSaveCall==false)
                    bAfterSaveCall=true;
                else
                {
                    try
                    {
                        var eventObj = new CSXSEvent();
                        eventObj.type = "copyFile";
                        eventObj.data = local.fsName + "|" + server.fsName + "|aftersave";
                        eventObj.dispatch();
                    }
                    catch(e) {
                        try {
                            ErrorLog(e.message + " 1",true,0);
                            if(local != null)
                                ErrorLog(local.fsName,true,0);
                            if(server != null)
                                ErrorLog(server.fsName,true,0);
                        }catch(e){}
                    }
                }
            }
            else
            {
                checkPoint = 3;
                try {
                    ErrorLog("-----------------伺服器中的INDD檔被篡改-----------",true,0);
                    if(local != null)
                        ErrorLog(local.fsName,true,0);
                    if(server != null)
                        ErrorLog(server.fsName,true,0);
                    ErrorLog("mod time :" + typeset.modificationDate + "--  old mod time:" + oldModificationDate  + "-- server time:"  + server.modified,true,0);
                    ErrorLog("-----------------伺服器中的INDD檔被篡改-----------",true,0);
                }catch(e){}
               
                var strDate1 = oldModificationDate.getFullYear() + "-" +  ("0"+(oldModificationDate.getMonth() + 1)).slice(-2) + "-" +  ("0"+oldModificationDate.getDate()).slice(-2) + " " + ("0"+oldModificationDate.getHours()).slice(-2) + ":" +("0"+oldModificationDate.getMinutes()).slice(-2) + ":" +("0"+oldModificationDate.getSeconds()).slice(-2);
                var strDate2 = server.modified.getFullYear() + "-" +  ("0"+(server.modified.getMonth() + 1)).slice(-2) + "-" +  ("0"+server.modified.getDate()).slice(-2) + " " + ("0"+server.modified.getHours()).slice(-2) + ":" +("0"+server.modified.getMinutes()).slice(-2) + ":" +("0"+server.modified.getSeconds()).slice(-2);
                currentTypeset = typeset;
                
                var data = "警告：伺服器中的INDD檔被篡改，是否繼續保存？\r\n開始本次組版時的最後修改時間\t:" + strDate1 + "\r現時的最後修改時間\t\t\t:" + strDate2 + ",保存,取消,另存伺服器版本,save,";
                loadLib();
                var eventObj = new CSXSEvent();
                eventObj.type = "yesNoCancelDialog";
                eventObj.data = data;
                eventObj.dispatch();
            }
            checkPoint = 4;
            typeset_list[doc.fullName.fsName] = typeset;
        }
        catch(e) {
            try {alert("警告，偵測到 InDesign 運行環境出現異常！繼續進行會有機會出現不可預測的錯誤。強烈建議重啓 InDesign。");
                
                ErrorLog(e.message + " " + checkPoint,true,0);
                if(local != null)
                    ErrorLog(local.fsName,true,0);
                if(server != null)
                    ErrorLog(server.fsName,true,0);
            }catch(e){
                ErrorLog("write after_save log error",true,0);
            }
        }
    }
    
    function AFTER_CLOSE(doc,typeset){
        bClose = true;
        delete typeset_list[doc.fullName.fsName];
        
        var local = File(typeset.local_file);
        var server = File(typeset.server_file);
        
        if (typeset.bCompose == false && (typeset.modificationDate.getTime() ==local.modified.getTime() || typeset.modificationDate.getTime() ==server.modified.getTime()))
        {
            var data = typeset.column_id + "," + typeset.column_date + ",C";
            loadLib();
            var eventObj = new CSXSEvent();
            eventObj.type = "SP_UpdateFinishRecord";
            eventObj.data = data;
            eventObj.dispatch();
            try
            {
                unlockFile(typeset.server_file);
                ErrorLog("Unlock file (" + server.fsName + ") - after close document",false,7);
            }
            catch (e) {
                ErrorLog("Unlock file (" + server.fsName + ") failed - after close document",false,7);
            }
        }
        else 
        {
            try
            {
                unlockFile(typeset.server_file);
                ErrorLog("Unlock file (" + server.fsName + ") - after complete compose",false,7);
            }
            catch (e) {
                ErrorLog("Unlock file (" + server.fsName + ") failed - after complete compose",false,7);
            }
        }
    }

    function ReopenResourceDialog(docName)
    {
        loadLib();
        var eventObj = new CSXSEvent();
        eventObj.type = "Reopen Resource Dialog";
        eventObj.data = docName;
        eventObj.dispatch();
    }

    function HideAllResourceDialog()
    {
        loadLib();
        var eventObj = new CSXSEvent();
        eventObj.type = "Hide All Resource Dialog";
        eventObj.dispatch();
    }

    app.addEventListener("afterActivate",function(evt){
        try{
            if (app.documents.length == 0){
                HideAllResourceDialog();
                return;
            }

            var doc
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc =evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target.constructor;
            else if(evt.target.constructor.name == "Application")
                doc=app.activeDocument;
            else{
                HideAllResourceDialog();
                return;
            }
     
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
            {
                HideAllResourceDialog();
                return;
            }
          
            var t=typeset_list[doc.fullName.fsName];
            if(t == null)
            {
                HideAllResourceDialog();
                return;
            }
            AFTER_ACTIVATE(doc);
        }
        catch(e)
        {
            alert("afterActivate: " + e.message)
        }
    });

    app.addEventListener("beforeSave",function(evt){
        try{
            if (app.documents.length == 0)
                return;

            var doc
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc =evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                return;
            var typeset=typeset_list[doc.fullName.fsName];
            if(typeset != null)
            {
                BEFORE_SAVE(doc,typeset);
            }
        }
        catch(e)
        {
            alert("beforeSave: " + e.message)
        }
    });
    
    app.addEventListener("afterSave",function(evt){
        try
        {
            if (app.documents.length == 0)
                return;

            var doc
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc =evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                return;
            var typeset=typeset_list[doc.fullName.fsName];
            if(typeset!=null){
                AFTER_SAVE(doc,typeset);
            }
        }
        catch(e)
        {
            alert("afterSave: " + e.message)
        }
    });
    
    app.addEventListener("afterClose",function(evt){
        try{
            if (app.documents.length == 0){
                HideAllResourceDialog();
                return;
            }

            var doc
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc =evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                return;
            
            var typeset=typeset_list[doc.fullName.fsName];
            if(typeset!=null){
                AFTER_CLOSE(doc,typeset);
                
                var active_doc=app.activeDocument;
                if(active_doc!=null){
                    var t=typeset_list[active_doc.name];
                    if(t!=null)
                    {
                        AFTER_ACTIVATE();
                    }
                }
            }
        }
        catch(e)
        {
            alert("afterClose: " + e.message)
        }
    });
    
    app.addEventListener("beforeSaveAs",function(evt){
        try{
            if (app.documents.length == 0)
                return;

            var doc
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc =evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                return;
            var typeset=typeset_list[doc.fullName.fsName];
            if(typeset != null)
            {
                AFTER_CLOSE(doc,typeset);
            }
        }
        catch(e)
        {
            alert("beforeSaveAs: " + e.message)
        }
    });
    
    app.addEventListener("afterSaveAs",function(evt){
        try{
            if (app.documents.length == 0)
                return;

            var doc
            if(evt.target.constructor.name == "LayoutWindow") {
                if(evt.target.parent.constructor.name == "Document")
                    doc =evt.target.parent;
            } else if(evt.target.constructor.name == "Document")
                doc=evt.target;
            
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                return;
            var typeset=typeset_list[doc.fullName.fsName];
            if(typeset!=null){
                
                var active_doc=app.activeDocument;
                if(active_doc!=null){
                    var t=typeset_list[active_doc.name];
                    if(t!=null)
                    {
                        AFTER_ACTIVATE();
                    }
                }
            }
        }
        catch(e)
        {
            alert("afterSaveAs: " + e.message)
        }
    });
    
    app.addEventListener("beforeQuit",function(evt){
        try{
            var bQuit = true;
            for (var i = 0; i< app.documents.length; i++)
            {
                var doc = app.documents.item(i);
                if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                    continue;
                var t=typeset_list[doc.fullName.fsName];
                var p=pretypeset_list[doc.fullName.fsName];
                if(t!=null || p != null)
                {
                    bQuit = false;
                    alert("請先關閉所有組版中的版面，再關閉InDesign。");
                    evt.stopPropagation();  
                    evt.preventDefault();
                    break;
                }
            }
            if (bQuit)
            {
                removeMenu();
            }
        }
        catch(e)
        {
            alert("beforeQuit: " + e.message)
        }
    });

    var onDelete = app.menuActions.itemByID(272); //Invoke after delete
    if (onDelete.eventListeners.length == 0)
    {
        onDelete.addEventListener( "afterInvoke", function(evt){
            try{
                var doc = app.activeDocument;
                if(doc == undefined || doc == null || doc.name == undefined || doc.name == null)
                    return;

                var stories = app.activeDocument.stories.everyItem().getElements();
                var storieslist = "";
                for (var i= 0; i < stories.length; i++)
                {
                    if (stories[i].label != "")
                    {
                        storieslist +=  stories[i].label + "*" + stories[i].id +"|";
                    }
                }
                var graphics = app.activeDocument.allGraphics;
                for (var i= 0; i < graphics.length; i++)
                {
                    if (graphics[i].itemLink != null)
                    {
                        storieslist +=  graphics[i].label + "*" + graphics[i].id +"|";
                    }
                }
                var eventObj = new CSXSEvent();
                eventObj.type = "HandleDeleteText";
                eventObj.data = storieslist;
                eventObj.dispatch();
            }
            catch(e)
            {
                alert("afterDelete: " + e.message)
            }
        });
    }

    var onUndo = app.menuActions.itemByID(265);
    if (onUndo.eventListeners.length == 0)
    {
        onUndo.addEventListener( "afterInvoke", function(evt){
            var eventObj = new CSXSEvent();
            eventObj.type = "UpdateTreeIcon";
            eventObj.dispatch();
        });
    }

    var onRedo = app.menuActions.itemByID (266); //Invoke before undo
    if (onRedo.eventListeners.length == 0)
    {
        onRedo.addEventListener( "afterInvoke", function(evt){
            var eventObj = new CSXSEvent();
            eventObj.type = "UpdateTreeIcon";
            eventObj.dispatch();
        });
    }
    
    var selection = [];
    var onCut = app.menuActions.itemByID(269); //Invoke before cut
    if (onCut.eventListeners.length == 0)
    {
        onCut.addEventListener( "beforeInvoke", function(evt){
            var doc = app.activeDocument;
            var typeset=typeset_list[doc.fullName.fsName];
            if (app.activeDocument!=null && typeset!=null)
            {
                selection = [];
                var select = app.activeDocument.selection;
                for (var i =0; i<select.length; i++ )
                {
                    var selectObj = new Object();
                    if (select[i].constructor.name == "TextFrame")
                    {
                        selectObj.path = select[i].parentStory.label;
                        selectObj.text = select[i].contents;
                    }
                    selection.push(selectObj);
                }
            }
            var eventObj = new CSXSEvent();
            eventObj.type = "UpdateTreeIcon";
            eventObj.dispatch();
        });
    }
    
    var onPaste = app.menuActions.itemByID(271); //Invoke after Paste
    if (onPaste.eventListeners.length == 0)
    {
        onPaste.addEventListener( "afterInvoke", function(evt){
            var doc = app.activeDocument;
            var typeset=typeset_list[doc.fullName.fsName];
            if (app.activeDocument!=null && typeset!=null)
            {
                if (selection.length > 0)
                {
                    var select = app.activeDocument.selection;
                    for (var i =0; i<select.length; i++ )
                    {
                        var bFind = false;
                        for (var j = selection.length - 1; j >= 0; j--)
                        {
                            if (select[i].constructor.name == "TextFrame")
                            {
                                if (select[i].contents == selection[j].text)
                                {
                                    select[i].parentStory.label = selection[j].path;
                                    bFind = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            var eventObj = new CSXSEvent();
            eventObj.type = "UpdateTreeIcon";
            eventObj.dispatch();
        });
    }
}

function yesNoCancelResult(result)
{
    LoadSettingXML();
    var data = result.split(",");
    if (data[0] == "Yes")
    {
        if (data[1] == "replace"){
            try{
                app.doScript(function(){
                    var select = app.activeDocument.selection[0];

                    var path = data[2];
                    var filecode = data[3];
                    var file= new File(path);
                    
                    var font;
                    if (select.constructor.name == "TextFrame")
                        font = select.parentStory.appliedFont;
                    else if (select.constructor.name == "Image"){
                        app.activeDocument.select(select.parent);
                        select = select.parent;
                    }
                    else if (select.constructor.name == "InsertionPoint")
                    {
                        font = select.appliedFont;
                        select.parent.contents = "";
                    }
                    if (font == undefined){
                        if (Setting.fontDefault)
                            font = GetSettingFont();
                        else
                            font = app.activeDocument.allParagraphStyles[1].appliedFont;
                    }
                    
                    var m_iGraphics = app.activeDocument.allGraphics.length;
                    var strExt = path.substr(path.lastIndexOf(".")+1);
                    strExt = strExt.toLowerCase();
                    if (strExt !="xls" && strExt!="xlsx")
                    {
                        if (select.constructor.name == "TextFrame" && select.parentStory != null)
                        {
                            select.parentStory.contents = "";
                        }
                        select.place(file,false,[0,0]);
                        select.fit(FitOptions.CONTENT_TO_FRAME)
                    }
                    else
                    {
                        if (!bMP())
                            app.scriptPreferences.userInteractionLevel = UserInteractionLevels.neverInteract;
                        select.place(file,true,[0,0]);	
                    }
                    select = app.activeDocument.selection[0];
                    if (select.constructor.name == "Rectangle"){
                        select = select.allGraphics[0]
                        select.label = path;
                        select.insertLabel("filecode",filecode);
                    }
                    else if (select.hasOwnProperty("parentStory"))
                    {
                        select.parentStory.label = path;
                        try 
                        {
                            select.parentStory.appliedFont = font;
                            select.parentStory.insertLabel("filecode",filecode);
                        } catch(e){}
                    }
                    else{
                        if (select.constructor.name == "Story")
                            select.appliedFont = font;
                        select.label = path;
                        select.insertLabel("filecode",filecode);
                    }
                }, ScriptLanguage.JAVASCRIPT, undefined, UndoModes.ENTIRE_SCRIPT, "取代稿件");
                var eventObj = new CSXSEvent();
                eventObj.type = "replace article";
                eventObj.dispatch();
            }catch(e){ErrorLog(e.message,true,0);}
        }
        else if (data[1] == "save"){
            var local = File(currentTypeset.local_file);
            var server = File(currentTypeset.server_file);
            
            try
            {
                unlockFile(currentTypeset.server_file);
                ErrorLog("Unlock server file (" + server.fsName + ") - After save document (local and server modification date not same) - continues",false,7);
            }
            catch (e) {
                ErrorLog("Unlock server file (" + server.fsName + ") failed - After save document (local and server modification date not same) - continues",false,7);
            }
            try
            {
                ErrorLog("ok",true,0);
                
                var eventObj = new CSXSEvent();
                eventObj.type = "copyFile";
                eventObj.data = local.fsName + "|" + server.fsName + "|save";
                eventObj.dispatch();
            }
            catch(e) {
                try{
                    ErrorLog(e.message + "3",true,0);
                    if(local != null)
                        ErrorLog(local.fsName,true,0);
                    if(server != null)
                        ErrorLog(server.fsName,true,0);
                }catch(e){}
            }
        }
        else if (data[1] == "ComposeExportFile"){
            ComposeExportFile();
        }
        else if (data[1] == "ComposeExportJpeg"){
            ComposeExportJpeg();
        }
        else if (data[1] == "GetKeyWordsComplete"){
            var eventObj = new CSXSEvent();
            eventObj.type = "Get Typeset Data";
            eventObj.data = "ComposeExportJpeg";
            eventObj.dispatch();
        }
    }
    else if (data[0] == "No")
    {
        var eventObj = new CSXSEvent();
        eventObj.type = "close loading dialog";
        eventObj.dispatch();
        if (data[1] == "GetKeyWordsComplete"){
            ComposeTerminate(data[2]);
        }
    }
    else if (data[0] == "Cancel")
    {
        if (data[1] == "save"){
            var local = File(currentTypeset.local_file);
            var server = File(currentTypeset.server_file);
            
            var saveFile = server.saveDlg();   
            if (saveFile)  
            {
                ErrorLog("saveas: "+ saveFile.fsName,true,0);
                try
                {
                    unlockFile(currentTypeset.server_file);
                    ErrorLog("Unlock server file (" + server.fsName + ") - After save document - before copy to server (local and server modification date not same) - save as",false,7);
                }
                catch (e) {
                    ErrorLog("Unlock server file (" + server.fsName + ") failed - After save document - before copy to server (local and server modification date not same) - save as",false,7);
                }
                
                var eventObj = new CSXSEvent();
                eventObj.type = "copyFile";
                eventObj.data = local.fsName + "|" + server.fsName + "|saveas";
                eventObj.dispatch();
            }
        }
    }
}

function previewResult(result)
{
    try{
        var data = result.split(",");
        if (data[0] == "ok")
        {
            var psFile = Folder(data[3]);
            try {
                psFile.create();
            }
            catch (e) {
                alert("生成大樣圖片路徑『"+psFile.fsName+"』錯誤，請檢查設置");
            }

            var jpg_file = File(data[2]);
            var server_file = File(psFile.fsName + '\\' + jpg_file.name);

            if (data[1] == "ComposeExportJpeg")
                ErrorLog("3 - Start Copy JPG from local to Server",false,5);

            if (jpg_file.size!=0)
                jpg_file.copy(server_file);
            if (jpg_file.size != server_file.size && jpg_file.size!=0)
                jpg_file.copy(server_file);
            if (server_file.size == 0)
            {
                alert("複製大樣圖片到伺服器時出錯，請稍候再試!","Warning");
                return;
            }

            if (data[1] == "exportJpg"){}
            else if (data[1] == "beforeSave"){
                ErrorLog("export jpeg complete",true,0);
            }
            else if (data[1] == "ComposeExportJpeg"){
                ErrorLog("3 - End Copy JPG from local  to Server",false,5);
                var eventObj = new CSXSEvent();
                eventObj.type = "Get Typeset Data";
                eventObj.data = "ComposeExportFile";
                eventObj.dispatch();
            }
        }
        else if (data[0] == "cancel")
        {
            if (data[1] == "ComposeExportJpeg"){
                var data = "大樣圖片沒有生成，是否繼續組版?,是,否,,ComposeExportFile,";
                loadLib();
                var eventObj = new CSXSEvent();
                eventObj.type = "yesNoCancelDialog";
                eventObj.data = data;
                eventObj.dispatch();
            }
        }
        else if (data[0] == "reexport")
        {
            if (app.documents.length == 0)
                return;
            var doc=app.activeDocument;
            
            if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
                return;
            
            var jpg_file = File(data[2]);
            LoadSettingXML();
            app.jpegExportPreferences.jpegQuality=[JPEGOptionsQuality.MAXIMUM, JPEGOptionsQuality.HIGH, JPEGOptionsQuality.MEDIUM,JPEGOptionsQuality.LOW][Setting.jpg_quality-1]; 
            app.jpegExportPreferences.exportResolution=Setting.jpg_ppi;
            app.jpegExportPreferences.jpegExportRange = ExportRangeOrAllPages.EXPORT_ALL;
            app.jpegExportPreferences.exportingSpread = true;
            
            try{
                doc.exportFile(ExportFormat.jpg,jpg_file);
            } catch(e) {
                alert("警告，偵測到 InDesign 運行環境出現異常！繼續進行[生成大樣]會有機會出現不可預測的錯誤。強烈建議重啓 InDesign。", "Warning");
            }
        }
    } catch(e) {
        ErrorLog(e.message,true,0);
    }
}

function GetTypesetData(template, column_id, local_file, server_file, column_date, filename, username, layout_code, year, month, day, PSPath, PDFPath, PaperPSPath, tagServer , type){
    try {
        user = username;
        var ts =  new Object();
		ts.template = template;
		ts.column_id = column_id;
		ts.local_file = local_file;
		ts.server_file = server_file;
		ts.column_date = column_date;
		ts.filename = filename;
        ts.layout_code = layout_code;
        ts.date = new Object();
		ts.date.year = year;
		ts.date.month = month;
		ts.date.day = day;
		ts.PSPath = PSPath;
        ts.PDFPath = PDFPath;
        ts.PaperPSPath = PaperPSPath;
		ts.tagServer = tagServer;
        var local = File(local_file)
        ts.modificationDate = local.modified
        ts.bCompose = false;

        var doc=app.activeDocument;
        typeset_list[doc.fullName.fsName]=ts;

        if (type == "ComposeExportFile")
            ComposeExportFile();
        else if (type == "ComposeExportJpeg"){
            ComposeExportJpeg();
        }
        else if (type == "GetKeyWords")
            GetKeyWords()
    }
    catch(e){
        ErrorLog(e.message,true,0);
    }
}

function getPlacedFile()
{
    var stories = "";
    var storieslist = app.activeDocument.stories.everyItem().getElements()
    for(var i =0; i< storieslist.length; i++) {
        if (storieslist[i].label != "")
            stories +=  storieslist[i].label + "|"
    }

    var graphics = app.activeDocument.allGraphics;
    for (var i = graphics.length -1; i >= 0; i--) {
        if (graphics[i].label != "") {
             stories +=  graphics[i].label + "|";
        }
    }
    return stories;
}

function GetSettingFont()
{
    var settingFont = "";
    var systemFonts= app.fonts.everyItem().getElements();
    for (var j = 0; j < systemFonts.length; j++) {
        if (systemFonts[j].name == Setting.font )
        {
            settingFont = systemFonts[j];
            break;
        }
        else if (systemFonts[j].name =="新細明體\tRegular" || systemFonts[j].name =="PMingLiU\tRegular")
        {
            settingFont = systemFonts[j];
        }
    }
    return settingFont;
}

function PlaceFile(result){
    try
    {
        LoadSettingXML();
        var data = result.split("|");
        var path = data[0];
        var filecode = data[1];
        var select = app.activeDocument.selection;
        for (var i =0; i<select.length; i++ )
		{
            app.activeDocument.select(select,SelectionOptions.REMOVE_FROM);
        }
        app.doScript(function(){
            var strExt = path.substr(path.lastIndexOf(".")+1);
            strExt = strExt.toLowerCase();
            if (strExt =="xls" || strExt=="xlsx")
            {
                app.activeDocument.place(file,true,[0,0]);
            }
            else
            {
                var m_graphics = app.activeDocument.allGraphics;
                var file = new File(path);
                app.activeDocument.place(file,false,[0,0]);
                var stories = app.activeDocument.stories.everyItem().getElements();
                var graphics = app.activeDocument.allGraphics;
                if (m_graphics.length == graphics.length){
                    stories[stories.length - 1].appliedParagraphStyle = app.activeDocument.textDefaults.appliedParagraphStyle;
                    if (Setting.fontDefault)
                        stories[stories.length - 1].appliedFont = GetSettingFont();
                    else if (!bMP())
                        stories[stories.length - 1].appliedFont = app.activeDocument.allParagraphStyles[1].appliedFont;
                    stories[stories.length - 1].label = path;
                    stories[stories.length - 1].insertLabel("filecode",filecode);
                }
                else {
                    graphics[graphics.length - 1].label = path;
                }
            }
        }, ScriptLanguage.JAVASCRIPT, undefined, UndoModes.ENTIRE_SCRIPT, "置入稿件");
    } catch(e){ErrorLog(e.message,true,0);}
}

function file_tree_doubleClickHandler(path,filecode){
    try{
        var select = app.activeDocument.selection[0];
        if (select == null)
            return;
            
        var filename = app.activeDocument.fullName.name;
        var file = new File(path);
        if (!file.exists)
            return;
        
        if (select != null && path != ""){
            if (bMP()){
                yesNoCancelResult("Yes,replace," + path + "," + filecode)
            }
            else{
                path = path.replace(/\\/g,'\\\\');
                var data = "是否取代已選取項目,是,否,,replace," + path + "," + filecode;

                loadLib();
                var eventObj = new CSXSEvent();
                eventObj.type = "yesNoCancelDialog";
                eventObj.data = data;
                eventObj.dispatch();
            }
        }
    }
    catch(e){
        ErrorLog(e.message,true,0);
    }
}

function file_tree_selected(path){
    try{
        var links = app.activeDocument.links;
        for (var i = links.length - 1; i >= 0; i--){
            if(links[i].filePath==path && !(links[i].parent.parent.parent.constructor.name == "PlaceGun" ) ){
                links[i].parent.parent.select();
                return;
            }
        }

        var stories = app.activeDocument.stories;
        for (var i = stories.length - 1; i >= 0; i--){
            if (stories[i].label == path)
            {
                var text = stories[i].textContainers;
                if (text != null)
                {
                    if(text[0].itemLayer != null)
                        text[0].select();
                    for (var k = 1; k < text.length; k++)
                        if (text[k].itemLayer != null)
                            text[k].select(SelectionOptions.ADD_TO);
                }
                break;
            }
        }
    }
    catch(e){
        ErrorLog(e.message,true,0);
    }
}

function RemoveRollback(strBack_list){
    try{
        var back_list = strBack_list.split("|");

        var stories = app.activeDocument.stories;
        var bFind;
        for (var h = back_list.length -1; h >= 0; h--)
        {
            if (back_list[h] == "")
                continue;
            bFind = false;
            for (var i = stories.length -1; i >= 0; i--)
            {
                if (stories[i].label == back_list[h])
                {
                    bFind = true;
                    var text = stories[i].textContainers
                    for (var k = 0; k < text.length; k++)
                        text[k].remove();
                }
            }
            if (bFind)
                back_list.splice(h,1);
        }
        var graphics = app.activeDocument.allGraphics;
        for (var h = back_list.length -1; h >= 0; h--)
        {
            bFind = false;
            for (var i = graphics.length -1; i >= 0; i--)
            {
                if (graphics[i].itemLink != null && graphics[i].itemLink.filePath.toUpperCase() == back_list[h].toUpperCase())
                {
                    bFind = true;
                    graphics[i].parent.remove();
                    for (var x = stories.length -1; x >= 0; x--)
                    {
                        if (stories[x].label == back_list[h])
                        {
                            var text = stories[x].textContainers
                            for (k = 0; k < text.length; k++)
                                text[k].remove();
                        }
                    }
                }
            }
            if (bFind)
                back_list.splice(h,1);
        }
    }
    catch(e){
        ErrorLog(e.message,true,0);
    }
}

function LoadSettingXML(){
    if (Setting == null){
        Setting = new Object();
    }
    var textFile = File(Folder.myDocuments.parent.fsName + "\\EMS6\\setting.xml");
    if (textFile.exists)
    {
        textFile.open("r");
        var xmlString = textFile.read();
        var xml = new XML(xmlString);
        textFile.close();
        Setting.normalLog = (xml.normalLog == "true");
        Setting.systemLog = (xml.systemLog == "true");
        Setting.composeCompleteLog = (xml.composeCompleteLog == "true");
        Setting.debug = (xml.debug == "true");
        Setting.local_temp = xml.local_temp;
        Setting.fontDefault = (xml.fontDefault == "true");
        Setting.font = xml.font;
        Setting.jpg_quality = xml.jpg_quality;
        Setting.jpg_ppi = parseInt(xml.jpg_resolution);
        Setting.jpg_preview_save = xml.jpg_preview_save;
        Setting.subPath = xml.subPath;

        Setting.pdf = xml.pdf;
        Setting.pdf_prefix_ckb = xml.pdf_prefix_ckb;
        Setting.pdf_prefix = xml.pdf_prefix;
        Setting.pdf_suffix_ckb = xml.pdf_suffix_ckb;
        Setting.pdf_suffix = xml.pdf_suffix;
        Setting.save_jpg = (xml.save_jpg != "false");
        Setting.complete_jpg = (xml.complete_jpg != "false")
        Setting.jpg_preview = (xml.jpg_preview == "true")
        Setting.jpg_preview_save = (xml.jpg_preview_save == "true")
        Setting.dateFmt = xml.dateFmt;
    }
    else
    {
        Setting.normalLog = true;
        Setting.systemLog = true;
        Setting.composeCompleteLog = true;
        Setting.debug = true;
        Setting.local_temp = Folder.myDocuments.parent.fsName + "\\EMS6";
        Setting.fontDefault = true;
        Setting.save_jpg = true;
        Setting.jpg_quality = 3;
        Setting.jpg_ppi = parseInt(72);
        Setting.jpg_preview_save = true;
        Setting.subPath = 0;
        Setting.dateFmt = "4YYYYMMDD";
    }
}

function ErrorLog(strLogMsg, bDebug, itype)
{
    LoadSettingXML()
    
    if ((itype == 1 || itype == 2 || itype == 3)  && Setting.normalLog  == false)
        return;
    
    if ((itype == 4) && Setting.systemLog  == false )
        return;
    
    if ((itype == 5) && Setting.composeCompleteLog  == false)
        return;
    
    if ((itype == 6) && Setting.normalLog  == false)
        return;
    
    if ((itype == 7) && Setting.normalLog  == false)
        return;
    
    if (bDebug && itype == 0)
    {
        if (!Setting.debug)
            return;
        
        var loggingStr = ""
        var now = new Date();
        
        var time = now.getFullYear() + "-" + ("0"+(now.getMonth() + 1)).slice(-2) + "-" + ("0"+(now.getDate())).slice(-2) + " " + ("0"+now.getHours()).slice(-2) + ":" +("0"+now.getMinutes()).slice(-2) + ":" +("0"+now.getSeconds()).slice(-2);
        loggingStr = time + " " + strLogMsg;
            
        time = now.getFullYear() + "" + ("0"+(now.getMonth() + 1)).slice(-2) + "" + ("0"+(now.getDate())).slice(-2);
        try
        {
            var file;
            if (bMP())
                file = File(Setting.local_temp+"\\Log\\IDPlugin" + time + ".log");
            else
                file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\IDPlugin" + time + ".log");

            var f = new Folder(file.parent);  
            f.create();
            
            file.open("a");
            file.writeln(loggingStr);
            file.close();
            
        } catch (err){}
    }
    else
    {
        var loggingStr = ""
        var now = new Date();
        var time = now.getFullYear() + "-" + ("0"+(now.getMonth() + 1)).slice(-2) + "-" + ("0"+(now.getDate())).slice(-2) + " " + ("0"+now.getHours()).slice(-2) + ":" +("0"+now.getMinutes()).slice(-2) + ":" +("0"+now.getSeconds()).slice(-2);
        if (strLogMsg.length > 0)
        {
            loggingStr = time + " " + strLogMsg;
        }
        else
            loggingStr = "";
        
        time = now.getFullYear() + "" + ("0"+(now.getMonth() + 1)).slice(-2) + "" + ("0"+(now.getDate())).slice(-2);
        try
        {
            var file;
            if (itype == 0)
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\WebService.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\WebService.log");
            }
            else if (itype == 1)
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\News.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\News.log");
            }
            else if (itype == 2)
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\RollBack.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\RollBack.log");
            }
            else if (itype == 3)
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\Placed.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\Placed.log");
            }
            else if (itype == 4)
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\System.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\System.log");
            }
            else if (itype == 5)
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\Compose.Complete.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\Compose.Complete.log");
            }
            else if (itype == 6)
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\fileDateTime.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\fileDateTimeLog.log");
            }
            else if (itype == 7 )
            {
                if (bMP())
                    file = File(Setting.local_temp + "\\Log\\" + time + "\\fileLock.log");
                else
                    file = File(Folder.myDocuments.parent.fsName + "\\EMS6\\Log\\" + time + "\\fileLock.log");
            }
            
            var f = new Folder(file.parent);  
            f.create();

            file.open("a");
            file.writeln(loggingStr);
            file.close();
        } catch (err){}
    }
}

function copyFileSucess(file){
    var strFile = file.split("|");
    var sourceFile = File(strFile[0]);
    var targetFile = File(strFile[1]);
    if (targetFile.size == 0)
    {
        alert("複製文件到伺服器時出錯，請稍候再試!");
        return;
    }

    if (app.documents.length == 0){
        return;
    }

    var doc=app.activeDocument;
    if (doc == undefined || doc == null || doc.constructor.name != "Document" || doc.properties['fullName'] == undefined || doc.name == undefined || doc.name == null)
    {
        return;
    }
    
    if(doc.fullName.fsName != sourceFile.fsName)
    {
        ErrorLog(doc.fullName.fsName + " != " + sourceFile.fsName,true,0);
        return;
    }

    try
    {
        if (lockFile(targetFile.fsName) == "false"){
            if (strFile[2] == 'opendocument')
                ErrorLog("Lock file (" + targetFile.fsName + ") fail - open document",false,7);
            if (strFile[2] == 'aftersave')
                ErrorLog("Lock file (" + targetFile.fsName + ") fail - After save document - after copy local to server",false,7);
            if (strFile[2] == 'save')
                ErrorLog("Lock file (" + targetFile.fsName + ") fail - After save document (local and server modification date not same) - continues - after copy local to server",false,7);
            if (strFile[2] == 'saveas')
                ErrorLog("Lock file (" + targetFile.fsName + ") fail - After save document  - before copy to server (local and server modification date not same) - save as - copy local to save as path",false,7);
            try{
                if (strFile[2] == 'opendocument')
                    ErrorLog(e.message + " 1",true,0);
                if (strFile[2] == 'aftersave')
                    ErrorLog(e.message + " 2",true,0);
                if (strFile[2] == 'save')
                    ErrorLog(e.message + " 4",true,0);
                if (strFile[2] == 'saveas')
                    ErrorLog(e.message + " 5",true,0);
                if(sourceFile != null)
                    ErrorLog(sourceFile.fsName,true,0);
                if(server != null)
                    ErrorLog(targetFile.fsName,true,0);
            }catch(e){}
        }
        if (strFile[2] == 'opendocument')
            ErrorLog("Lock server file (" + targetFile.fsName + ") - open document",false,7);
        if (strFile[2] == 'aftersave')
            ErrorLog("Lock server file (" + targetFile.fsName + ") - After save document - after copy local to server",false,7);
        if (strFile[2] == 'save')
            ErrorLog("Lock server file (" + targetFile.fsName + ") - After save document (local and server modification date not same) - continues - after copy local to server",false,7);
        if (strFile[2] == 'saveas')
            ErrorLog("Lock server file (" + targetFile.fsName + ") - After save document  - before copy to server (local and server modification date not same) - save as - copy local to save as path",false,7);
    }                
    catch(e) {
        if (strFile[2] == 'opendocument')
            ErrorLog("Lock file (" + targetFile.fsName + ") fail - open document",false,7);
        if (strFile[2] == 'aftersave')
            ErrorLog("Lock file (" + targetFile.fsName + ") fail - After save document - after copy local to server",false,7);
        if (strFile[2] == 'save')
            ErrorLog("Lock file (" + targetFile.fsName + ") fail - After save document (local and server modification date not same) - continues - after copy local to server",false,7);
        if (strFile[2] == 'saveas')
            ErrorLog("Lock file (" + targetFile.fsName + ") fail - After save document  - before copy to server (local and server modification date not same) - save as - copy local to save as path",false,7);
        try{
            if (strFile[2] == 'opendocument')
                ErrorLog(e.message + " 1",true,0);
            if (strFile[2] == 'aftersave')
                ErrorLog(e.message + " 2",true,0);
            if (strFile[2] == 'save')
                ErrorLog(e.message + " 4",true,0);
            if (strFile[2] == 'saveas')
                ErrorLog(e.message + " 5",true,0);
            if(sourceFile != null)
                ErrorLog(sourceFile.fsName,true,0);
            if(server != null)
                ErrorLog(targetFile.fsName,true,0);
        }catch(e){}
    }
}

function ComposeComplete(){
    ErrorLog("",false,5);
    ErrorLog("1 - Start Compose Complete ---- " + app.activeDocument.fullName.name,false,5);
    var doc=app.activeDocument;
    
    if (doc == null) return;
    
    if (bHKET()){
        openLoadingDialog();
        ComposeExportJpeg();
    }
    else if (bMP())
    {	
        try
        {
            var stories = app.activeDocument.stories;
            var bFind;
            
            var page = app.activeDocument.pages.item(0);
            var docY1,docX1 ,docY2, docX2 ;
            docY1 = page.bounds[0]; //top
            docX1 = page.bounds[1]; //left
            docY2 = page.bounds[2];
            docX2 = page.bounds[3];
            for (var i = stories.length -1; i >= 0; i--)
            {
                if (stories.item(i).overflows)
                {
                    var text = stories.item(i).textContainers;
                    for (var k = 0; k < text.length; k++)
                    {
                        if (!text[k].itemLayer.visible || text[k].itemLayer.locked)
                            break;
                        
                        var positionY1,positionX1, positionY2,positionX2;
                        var leftTop, rightBottom, IsInsidePageArea;
                        positionY1 = text[k].geometricBounds[0];
                        positionX1 = text[k].geometricBounds[1];
                        positionY2 = text[k].geometricBounds[2];
                        positionX2 = text[k].geometricBounds[3];
                        
                        //inside page detection
                        leftTop = (positionX1 >= docX1 && positionY1 >= docY1); //+ve 
                        rightBottom = (positionX2 <= docX2 && positionY2 <=  docY2); 
                        //inside page detection end
                        
                        if (leftTop && rightBottom)
                        {
                            alert("版面中有文章未能完全顯示，請檢查後再完成組版", "Warning");
                            return;
                        }
                    }
                }
            }
        }
        catch(e)
        {
            ErrorLog("Error text overflows ---- "  + e.message,false,5);
        }
        
        openLoadingDialog();
        var eventObj = new CSXSEvent();
        eventObj.type = "Get Typeset Data";
        eventObj.data = "GetKeyWords";
        eventObj.dispatch();
    }
    else
    {
        openLoadingDialog();
        ErrorLog("ComposeComplete "  ,false,5);
        var eventObj = new CSXSEvent();
        eventObj.type = "LoadBackNewsDetailCallback";
        eventObj.data = "ComposeComplete";
        eventObj.dispatch();
    }
}

function LoadBackNewsDetailSucess(results){
    var result = results.split(",");
    var back_list = result[0].split("|");
    for (var h = back_list.length -1; h >= 0; h--){
        if (back_list[h] == "")
            back_list.splice(h,1);
    }
    if (result[1] == "ComposeComplete"){
        if (parseInt(back_list.length) > 0)
        {
            var data = "該版面有"+(back_list.length)+"篇稿件被回調，是否繼續組版?,是,否,,ComposeExportJpeg";

            loadLib();
            var eventObj = new CSXSEvent();
            eventObj.type = "yesNoCancelDialog";
            eventObj.data = data;
            eventObj.dispatch();
        }
        else{
            ComposeExportJpeg();
        }
    }
    else if (result[1] == "GetKeyWordsComplete") {
        var objCount = 0;
        var OutsideCount = 0;
        ErrorLog("End LoadBackNewsDetail",false,5);
        try {
            if (back_list.length > 0) {
                ErrorLog("Have BackNews",false,5);
                
                var positionY1,positionX1, positionY2,positionX2;
                var leftTop, rightBottom, IsInsidePageArea;
                
                var stories = app.activeDocument.stories;
                var bFind;
                
                var haveWarning, allOutside;
                var page = app.activeDocument.pages.item(0);
                var docY1,docX1 ,docY2, docX2 ;
                docY1 = page.bounds[0]; //top
                docX1 = page.bounds[1]; //left
                docY2 = page.bounds[2];
                docX2 = page.bounds[3];
                
                try{
                    for (var h = back_list.length -1; h >= 0; h--){
                        bFind = false;
                        haveWarning = false;
                        allOutside = true;
                        for (var i = stories.length -1; i >= 0; i--){
                            if (bFind)
                                break;
                            if (stories.item(i).label == back_list[h]) {
                                var text = stories.item(i).textContainers
                                
                                for (var k = 0; k < text.length; k++) {
                                    if (!text[k].itemLayer.visible) {
                                        back_list.splice(h,1);
                                        bFind = true;
                                        break;
                                    }
                                    
                                    /////Doc Geometry Test
                                    positionY1 = text[k].geometricBounds[0];
                                    positionX1 = text[k].geometricBounds[1];
                                    positionY2 = text[k].geometricBounds[2];
                                    positionX2 = text[k].geometricBounds[3];
                                    /////Doc Geometry End
                                    
                                    //inside page detection
                                    leftTop = (positionX1 >= docX1 && positionY1 >= docY1); //+ve 
                                    rightBottom = (positionX2 <= docX2 && positionY2 <= docY2); 
                                    //inside page detection end
                                    
                                    IsInsidePageArea = (leftTop && rightBottom);
                                    
                                    if (text[k].extractLabel("warnAlready") == "warnAlready" || IsInsidePageArea == false ){
                                        haveWarning = true;
                                        if (IsInsidePageArea){
                                            allOutside = false;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if (haveWarning){
                            objCount ++;
                            if (allOutside){
                                OutsideCount ++;
                            }
                        }
                    }
                }
                catch(e){
                    ErrorLog("Count BackNews Text Error "  + e.message,false,5);
                }
                
                var graphics = app.activeDocument.allGraphics;
                
                try {
                    for (var h = back_list.length -1; h >= 0; h--) {
                        haveWarning = false;
                        allOutside = true;
                        for (i = graphics.length -1; i >= 0; i--) {
                            if (graphics[i].itemLink != null && graphics[i].itemLink.filePath.toUpperCase() == back_list[h].toUpperCase()) {
                                if (!graphics[i].parent.itemLayer.visible || graphics[i].parent.itemLayer.locked) {
                                    back_list.splice(h,1);
                                    break;
                                }
                                
                                /////Doc Geometry Test
                                positionY1 = graphics[i].parent.geometricBounds[0];
                                positionX1 = graphics[i].parent.geometricBounds[1];
                                positionY2 = graphics[i].parent.geometricBounds[2];
                                positionX2 = graphics[i].parent.geometricBounds[3];
                                /////Doc Geometry End
                                
                                //inside page detection
                                leftTop = (positionX1 >= docX1 && positionY1 >= docY1); //+ve 
                                rightBottom = (positionX2 <= docX2 && positionY2 <= docY2); 
                                //inside page detection end
                                
                                IsInsidePageArea = (leftTop && rightBottom);
                                
                                if(graphics[i].parent.extractLabel("warnAlready") == "warnAlready"  || IsInsidePageArea == false ) {
                                    haveWarning = true;
                                    if (IsInsidePageArea) {
                                        allOutside = false;
                                        break;
                                    }
                                }
                            }
                        }
                        if (haveWarning) {
                            objCount ++;
                            if (allOutside)
                            {
                                OutsideCount ++;
                            }
                        }
                    }
                }
                catch(e)
                {
                    ErrorLog("Count BackNews Pic Error "  + e.message,false,5);
                }
                
                try
                {
                    ErrorLog(objCount + "    " + back_list.length + "    " + OutsideCount,true,0);
                    if ((objCount != back_list.length || (OutsideCount != objCount) ) && back_list.length > OutsideCount)
                    {
                        ErrorLog("BackNews Dialog",false,5);
                        result[0] = result[0].replace(/\\/g,'\\\\');
                        var data = "該版面有"+(back_list.length - OutsideCount)+"篇稿件被回調，是否繼續組版?,是,否,,GetKeyWordsComplete," + result[0];
                        loadLib();
                        var eventObj = new CSXSEvent();
                        eventObj.type = "yesNoCancelDialog";
                        eventObj.data = data;
                        eventObj.dispatch();
                    }
                    else
                    {
                        ErrorLog("Hide BackNews Dialog",false,5);
                        ComposeTerminate(result[0]);
                        ComposeExportJpeg();
                    }
                }
                catch(e)
                {
                    ErrorLog("BackNews ExportJpeg Error "  + e.message,false,5);
                    ComposeExportJpeg();
                }
            }
            else{
                ComposeExportJpeg();
            }
        }
        catch(e)
        {
            ErrorLog("After LoadBackNewsDetail Error "  + e.message,false,5);
            ComposeExportJpeg();
        }
    }
}

function ComposeTerminate(strBack_list)
{
    ErrorLog(strBack_list,true,0);
    var back_list = strBack_list.split("|");
    try
    {
        for (var h = back_list.length -1; h >= 0; h--){
            if (back_list[h] == "")
                back_list.splice(h,1);
        }
        var targetLayer;
        try
        {
            var activeLayer = app.activeDocument.activeLayer;
            var allLayer = app.activeDocument.layers;
        }
        catch(e)
        {
            ErrorLog("ComposeTerminate get active Layer Error "  + e.message,false,5);
        }
        
        try
        {
            for (var w = 0; w < allLayer.length; w++)
            {
                var extLayer = allLayer.item(w);
                if (extLayer.label == "warnLayer")
                {
                    if (extLayer.layerColor[0]==255 && extLayer.layerColor[1]==0 && extLayer.layerColor[2]==0)
                    {
                        targetLayer = extLayer;
                        break;
                    }
                }
            }
        }
        catch(e)
        {
            ErrorLog("ComposeTerminate get warnLayer Error "  + e.message,false,5);
        }
        
        ////////////////// Create layer if not exist
        if (targetLayer == null)
        {
            var layer;
            layer = app.activeDocument.layers.add();
            layer.label = "warnLayer";
            layer.name = "warnLayer";
            layer.layerColor = new Array(255, 0, 0);  //red color
            targetLayer = layer;
        }
        
        var  page = app.activeDocument.pages.item(0);
        var positionY1,positionX1, positionY2,positionX2;
        var docY1,docX1 ,docY2, docX2 ;
        docY1 = page.bounds[0]; //top
        docX1 = page.bounds[1]; //left
        docY2 = page.bounds[2];
        docX2 = page.bounds[3];
        var leftTop, rightBottom, IsInsidePageArea;
        
        //////////////////////// Find obj if obj is textObject
        var stories = app.activeDocument.stories;
        try
        {
            for (var h = back_list.length -1; h >= 0; h--)
            {
                for (var i = stories.length -1; i >= 0; i--)
                {
                    if (stories.item(i).label == back_list[h])
                    {
                        var text = stories.item(i).textContainers
                        for (var k = 0; k < text.length; k++)
                        {
                            /////Doc Geometry Test
                            positionY1 = text[k].geometricBounds[0];
                            positionX1 = text[k].geometricBounds[1];
                            positionY2 = text[k].geometricBounds[2];
                            positionX2 = text[k].geometricBounds[3];
                            /////Doc Geometry End
                            
                            //inside page detection
                            leftTop = (positionX1 >= docX1 && positionY1 >= docY1); //+ve 
                            rightBottom = (positionX2 <= docX2 && positionY2 <= docY2); 
                            
                            text[k].insertLabel("warnAlready","warnAlready");
                            if (text[k].itemLayer.visible && !text[k].itemLayer.locked && !text[k].locked)
                            {
                                if (text[k].parent.constructor.name === 'Group')
                                {
                                    var group = text[k].parent;
                                    group.ungroup();
                                }
                                text[k].itemLayer = targetLayer;
                                text[k].itemLayer.visible =true;
                            }
                        }
                    }
                }
            }
        }
        catch(e)
        {
            ErrorLog("ComposeTerminate set text label Error "  + e.message,false,5);
        }

        var graphics = app.activeDocument.allGraphics;
        try
        {
            ErrorLog(back_list.length,true,0);
            for (h = back_list.length -1; h >= 0; h--)
            {
                for (i = graphics.length -1; i >= 0; i--)
                {
                    ErrorLog(graphics[i].itemLink.filePath,true,0);
                    ErrorLog(back_list[h],true,0);
                    ErrorLog(graphics[i].itemLink.filePath.toUpperCase() == back_list[h].toUpperCase(),true,0);
                    if (graphics[i].itemLink != null && graphics[i].itemLink.filePath.toUpperCase() == back_list[h].toUpperCase())
                    {
                        /////Doc Geometry Test
                        positionY1 = graphics[i].parent.geometricBounds[0];
                        positionX1 = graphics[i].parent.geometricBounds[1];
                        positionY2 = graphics[i].parent.geometricBounds[2];
                        positionX2 = graphics[i].parent.geometricBounds[3];
                        /////Doc Geometry End
                        
                        //inside page detection
                        leftTop = (positionX1 >= docX1 && positionY1 >= docY1); //+ve 
                        rightBottom = (positionX2 <= docX2 && positionY2 <= docY2); 
                        //inside page detection end
                        
                        if (!leftTop || !rightBottom)
                            continue;
                        
                        graphics[i].parent.insertLabel("warnAlready","warnAlready");
                        
                        if (graphics[i].parent.itemLayer.visible && !graphics[i].parent.itemLayer.locked && !graphics[i].parent.locked)
                        {
                            if (graphics[i].parent.parent.constructor.name === 'Group')
                            {
                                group = graphics[i].parent.parent;
                                group.ungroup();
                            }
                            graphics[i].parent.itemLayer = targetLayer;
                            graphics[i].parent.itemLayer.visible= true;
                        }
                        
                    }
                }
            }
        }
        catch(e)
        {
            ErrorLog("ComposeTerminate set pic label Error "  + e.message,false,5);
        }
        
        try
        {
            app.activeDocument.selection = null; //release any focus on item layer
            var doc = app.activeDocument;
            doc.activeLayer = activeLayer;
        }
        catch(e)
        {
            ErrorLog("ComposeTerminate set back active Layer Error "  + e.message,false,5);
        }
    }
    catch(e)
    {
        ErrorLog("ComposeTerminate Error "  + e.message,false,5);
    }
}

function FormatDate(fmt, date){
    var dd = date.day;
    var mm = date.month;
    var yyyy = date.year;
    if (fmt == "0NONE"){
        return "";
    }
    else if (fmt == "1MM$DD"){
        return mm+"$"+dd;
    }
    else if (fmt == "2MMDDYYYY"){
        return mm+dd+yyyy;
    }
    else if (fmt == "3DDMMYYYY"){
        return dd+mm+yyyy;
    }
    else if (fmt == "4YYYYMMDD"){
        return yyyy+mm+dd;
    }
    else if (fmt == "5DD"){
        return dd;
    }
    return "";
}

function ComposeExportJpeg(){
    try{
        var doc=app.activeDocument;
        if(Setting.complete_jpg && Setting.jpg_preview){
            ErrorLog("export jpeg", true, 0);
            var typeset=typeset_list[doc.fullName.fsName];
            var jpg_path;
            var jpg_file;
            if (bMP())
            {
                var date = FormatDate(Setting.dateFmt, typeset.date);
                if (Setting.subPath == 1)
                {
                    jpg_path = Setting.local_temp + "\\" + typeset.layout_code;
                    if (date != "")
                        jpg_path += "\\" + date;
                }
                else if (Setting.subPath == 2)
                {
                    jpg_path=Setting.local_temp + "\\" + typeset.layout_code;
                }
                else if (Setting.subPath == 3)
                {
                    jpg_path=Setting.local_temp;
                    if (date != "")
                        jpg_path += "\\" + date;
                }
                else
                {
                    jpg_path=Setting.local_temp;
                    if (date != "")
                        jpg_path += "\\" + date;
                    jpg_path+= "\\" + typeset.layout_code;
                }
            }
            else
            {
                jpg_path=Setting.local_temp + "\\" + typeset.layout_code;
            }
            var filename = app.activeDocument.fullName.name;
            filename = filename.substr(0, filename.lastIndexOf("."))  +".jpg";
            jpg_file = File(jpg_path  + "\\" + filename);
            app.jpegExportPreferences.jpegQuality=[JPEGOptionsQuality.MAXIMUM, JPEGOptionsQuality.HIGH, JPEGOptionsQuality.MEDIUM,JPEGOptionsQuality.LOW][Setting.jpg_quality-1]; 
            app.jpegExportPreferences.exportResolution=Setting.jpg_ppi;
            app.jpegExportPreferences.jpegExportRange = ExportRangeOrAllPages.EXPORT_ALL;
            app.jpegExportPreferences.exportingSpread = true;
            
            try{
                var eventObj = new CSXSEvent();
                eventObj.type = "close loading dialog";
                eventObj.dispatch();
                ErrorLog("2 - Start Export JPG to local",false,5);
                doc.exportFile(ExportFormat.jpg,jpg_file);
                ErrorLog("2 - End Export JPG to local",false,5);

                loadLib();
                var eventObj = new CSXSEvent();
                eventObj.type = "Export Jpg";
                eventObj.data = jpg_file.fsName + "|ComposeExportJpeg|" + typeset.PSPath;
                eventObj.dispatch();
            }
            catch (e) {
                try{
                    ErrorLog("2 - Start Export JPG to local",false,5);
                    doc.exportFile(ExportFormat.jpg,jpg_file);
                    ErrorLog("2 - End Export JPG to local",false,5);

                    loadLib();
                    var eventObj = new CSXSEvent();
                    eventObj.type = "Export Jpg";
                    eventObj.data = jpg_file.fsName + "|ComposeExportJpeg|" + typeset.PSPath;
                    eventObj.dispatch();
                } catch(e) {
                    alert("生成大樣圖片失敗（本地），請重新『生成大樣圖片』。", "Warning");
                }
            }
        }
        else
        {
            ComposeExportFile();
        }
    }
    catch(e){ErrorLog(e.message,true,0);}
}

function PDFPreset(){
    var pdf=Setting.pdf;
    var ps=app.pdfExportPresets;
    for(var i=0;i<ps.length;i++){
        if(ps.item(i).name == pdf){
            return ps.item(i);
        }
    }
    return null;
}

function ComposeExportFile(){
    
    if (app.documents.length == 0)
        return;
    var doc=app.activeDocument;
    if (doc == undefined || doc == null) return;

    var typeset=typeset_list[doc.fullName.fsName];
    typeset.bCompose = true;
    
    var local;
    var localTemp;
    if (bMP())
    {
        var date = FormatDate(Setting.dateFmt, typeset.date);
        if (Setting.subPath == 1)
        {
            localTemp = Setting.local_temp + "\\" + typeset.layout_code;
            if (date != "")
                localTemp += "\\" + date;
        }
        else if (Setting.subPath == 2)
        {
            localTemp=Setting.local_temp + "\\" + typeset.layout_code;
        }
        else if (Setting.subPath == 3)
        {
            localTemp=Setting.local_temp;
            if (date != "")
                localTemp += "\\" + date;
        }
        else
        {
            localTemp=Setting.local_temp;
            if (date != "")
                localTemp += "\\" + date;
            localTemp+= "\\" + typeset.layout_code;
        }
    }
    else
    {
        localTemp=Setting.local_temp + "\\" + typeset.layout_code;
    }
    var filename = app.activeDocument.fullName.name;
    filename = filename.substr(0, filename.lastIndexOf("."));
    local = File(localTemp + "\\" + app.activeDocument.fullName.name);
    
    if(Setting.complete_jpg && !Setting.jpg_preview){
        var jpg_file = File(localTemp + "\\" + filename  + ".jpg");
        app.jpegExportPreferences.jpegQuality=[JPEGOptionsQuality.MAXIMUM, JPEGOptionsQuality.HIGH, JPEGOptionsQuality.MEDIUM,JPEGOptionsQuality.LOW][Setting.jpg_quality-1]; 
        app.jpegExportPreferences.exportResolution=Setting.jpg_ppi;
        app.jpegExportPreferences.jpegExportRange = ExportRangeOrAllPages.EXPORT_ALL;
        app.jpegExportPreferences.exportingSpread = true;
        
        try {
            
            ErrorLog("2 - Start Export jpg to local",false,5);
            doc.exportFile(ExportFormat.jpg,jpg_file);
            ErrorLog("2 - End Export jpg to local",false,5);
            ErrorLog("3 - Start Copy jpg from local to Server",false,5);
            var to_file = typeset.PSPath + "\\" + filename  + ".jpg";
            if (jpg_file.size!=0)
                jpg_file.copy(to_file);
            if (jpg_file.size != to_file.size && jpg_file.size!=0)
                jpg_file.copy(to_file);
            if (to_file.size == 0)
            {
                alert("複製大樣圖片到伺服器時出錯，請稍候再試!","Warning");
                return;
            }
            ErrorLog("3 - End Copy jpg from local  to Server",false,5);
        } catch (e) {
            try {
                ErrorLog("2 - Start Export jpg to Server",false,5);
                doc.exportFile(ExportFormat.jpg,jpg_file);
                ErrorLog("2 - End Export jpg to Server",false,5);
                ErrorLog("3 - Start Copy jpg from local to Server",false,5);
                to_file = typeset.PSPath + "\\" + filename+".jpg";
                if (jpg_file.size!=0)
                    jpg_file.copy(to_file);
                if (jpg_file.size != to_file.size && jpg_file.size!=0)
                    jpg_file.copy(to_file);
                if (to_file.size == 0)
                {
                    alert("複製大樣圖片到伺服器時出錯，請稍候再試!","Warning");
                    return;
                }
                ErrorLog("3 - End Copy jpg from local  to Server",false,5);
            } catch (e) {
                alert("生成大樣圖片失敗（本地），請重新『生成大樣圖片』。", "Warning");
            }
        }
    }
    
    ErrorLog("4 - Start Save indd file to local",false,5);

    bAfterSaveCall = false;
    doc.save();
    if (local.size == 0)
        doc.save();
    bAfterSaveCall = true;
    ErrorLog("4 - End Save indd file to local",false,5);
    //pdf
    ErrorLog("export pdf",true,0);
    var local_pdf;
    var pdf_filename = filename;
    if (bMP())
    {
        if (Setting.pdf_prefix_ckb && Setting.pdf_prefix != "")
            pdf_filename = Setting.pdf_prefix + pdf_filename;
        if (Setting.pdf_suffix_ckb && Setting.pdf_suffix != "")
            pdf_filename += Setting.pdf_suffix;
        local_pdf = File(localTemp + "\\" + filename  + ".pdf");
    }
    else
        local_pdf = File(localTemp + "\\" + filename  + "_Print.pdf");
    
    ErrorLog("5 - Start Export pdf to local",false,5);
    try{
        var PDFExportPreset = PDFPreset();
        if (PDFExportPreset != null)
            doc.exportFile(ExportFormat.pdfType,local_pdf,false,PDFExportPreset);
        else
            doc.exportFile(ExportFormat.pdfType,local_pdf,false);

    }
    catch(e) {
        try{
            var PDFExportPreset = PDFPreset();
            if (PDFExportPreset != null)
                doc.exportFile(ExportFormat.pdfType,local_pdf,false,PDFExportPreset);
            else
                doc.exportFile(ExportFormat.pdfType,local_pdf,false);
        }
        catch(e) {
            alert("生成大樣pdf失敗（本地），請稍候再試或聯絡系統管理員。", "Warning");
        }
    }
    ErrorLog("5 - End Export pdf to local",false,5);
    
    
    var pdf_file;
    if (bMP())
    {
        pdf_file = File(typeset.PDFPath + "\\" + pdf_filename + ".pdf");
    }
    else
        pdf_file= File(typeset.PDFPath + "\\" + pdf_filename+"_Print.pdf");
    
    var pdfFile = Folder(typeset.PDFPath);
    if(!pdfFile.exists){
        try {
            pdfFile.create();
        }
        catch (e) {
            alert("生成大樣pdf路徑『"+pdfFile.fsName+"』錯誤，請檢查設置", "Warning");
        }
    }
    
    ErrorLog("6 - Start Copy pdf from local to Server",false,5);
    if (local_pdf.size!=0)
        local_pdf.copy(pdf_file);
    if (local_pdf.size != pdf_file.size && local_pdf.size!=0)
        local_pdf.copy(pdf_file);
    if (pdf_file.size == 0)
    {
        alert("複製大樣pdf到伺服器時出錯，請稍候再試!","Warning");
        return;
    }
    ErrorLog("6 - End Copy pdf from local to Server",false,5);
    ErrorLog("export pdf complete",true,0);
    
    var server=Folder(typeset.PaperPSPath);
    if(!server.exists){
        
        try {
            server.create();
        }
        catch (e) {
            alert("生成大樣檔案(indd)路徑『"+server.fsName+"』錯誤，請檢查設置", "Warning");
        }
    }
    var server_file=File(server.fsName + "\\" + app.activeDocument.fullName.name);

    // 2022-12-01
        try
        {
            if(server_file.exists)
            {
                var server_date = server_file.modified;

                var server_d = server_date.getFullYear() + ("0"+ (server_date.getMonth()+1).toString()).slice(-2) + ("0"+server_date.getDate()).slice(-2) + ("0"+server_date.getHours()).slice(-2) + ("0"+server_date.getMinutes()).slice(-2) + ("0"+server_date.getSeconds()).slice(-2);
                var server_pos = server_file.fsName.lastIndexOf('.indd'); 
                var server_backup = server_file.fsName.slice(0,server_pos); 
                server_backup = server_backup + "_" + server_d + ".indd";
                if (server_file.size!=0)
                    server_file.copy(server_backup);
            }
        }
        catch(e) {
            try {
            }catch(e){}
        }
    // 2022-12-01
    

    doc.close(SaveOptions.NO);
    try
    {
        unlockFile(server_file.fsName);
        ErrorLog("Unlock file (" + server_file.fsName + ") - ComposeExportFile",false,7);
    }
    catch (e) {
        ErrorLog("Unlock file (" + server_file.fsName + ") failed - ComposeExportFile",false,7);
    }
    ErrorLog("7  - Start Copy indd file from local to Server",false,5);
    if (local_pdf.size!=0)
        local.copy(server_file);
    if (local.size != server_file.size && local.size!=0)
        local.copy(server_file);
    if (server_file.size == 0)
    {
        alert("複製文件到伺服器時出錯，請稍候再試!","Warning");
        return;
    }
    ErrorLog("7 - End Copy indd file from local to Server",false,5);
    doc=null;
    
    ErrorLog("8 - Start Update DB Compose Status",false,5);
    
    var eventObj = new CSXSEvent();
    eventObj.type = "close loading dialog";
    eventObj.dispatch();

    var data = typeset.column_id + "," + typeset.column_date + ",F";
    loadLib();
    var eventObj = new CSXSEvent();
    eventObj.type = "SP_UpdateFinishRecord";
    eventObj.data = data;
    eventObj.dispatch();
    ErrorLog("8 - End Update DB Compose Status",false,5)
    ErrorLog("1 - End Compose Complete",false,5);
}

var count = 0;
var count2 = 0;
var error = 0;
function GetKeyWords(){
    try
    {
        if (app.documents.length == 0)
            return;
        var doc=app.activeDocument;
        if (doc == undefined || doc == null) return;
        var typeset=typeset_list[doc.fullName.fsName];
        if (typeset.tagServer.length == 0){
            GetKeyWordsComplete();
            return;
        }
        var stories = app.activeDocument.stories;
        if (stories.length == 0){
            GetKeyWordsComplete();
            return;
        }
        
        count = 0;
        count2 = 0;
        error = 0;
        for (var i = 0; i<stories.length; i++) {
            if (stories.item(i).extractLabel("filecode") != "") {
                stories.item(i).insertLabel("orgText",stories.item(i).contents);
                count++;
            }
        }
        
        if (count == 0) {
            ErrorLog("No actical from Editroom",false,5);
            GetKeyWordsComplete();
            return;
        }
        
        ErrorLog("Start Tagging Web service",false,5);

        for (var j = 0; j<stories.length; j++)
        {
            var filecode = stories.item(j).extractLabel("filecode");
            if (filecode != "")
            {
                var eventObj = new CSXSEvent();
                eventObj.type = "GetKeyWordsByFileCode";
                eventObj.data = filecode + "," + j + "," + typeset.tagServer;
                eventObj.dispatch();
            }
        }
    }
    catch (e)
    {
        ErrorLog(e.message,true,0);
        ErrorLog("Tagging Error ---- " + e.message,false,5);
        GetKeyWordsComplete();
    }
}

function GetKeyWordsByFileCodeSucess(str){
    try{
        var col = str.split(",");
        if(col[0] == "Tagging Error") {
            ErrorLog(col[1] + " Tagging Error (After web service)---- " + e.message,false,5);
            count2++;
            error++
            if (count2 == count) {
                ErrorLog("End Tagging Web service",false,5);
                GetKeyWordsComplete();
            }
        } else{
            var stories = app.activeDocument.stories;
            stories.item(parseInt(col[1])).insertLabel("tag",col[0]);
            count2++;
            ErrorLog(col[2] + " Tagging: " + col[0],false,5);
            if (count2 == count) {
                ErrorLog("End Tagging Web service",false,5);
                GetKeyWordsComplete();
            }
        }
    }
    catch(e){
        ErrorLog("Tagging Error " + e.message,false,5);
        count2++;
        error++
        if (count2 == count) {
            ErrorLog("End Tagging Web service",false,5);
            GetKeyWordsComplete();
        }
    }
}

function GetKeyWordsComplete(){
    ErrorLog("Start LoadBackNewsDetail",false,5);
    var eventObj = new CSXSEvent();
    ErrorLog("GetKeyWordsComplete ",false,5);
    eventObj.type = "LoadBackNewsDetailCallback";
    eventObj.data = "GetKeyWordsComplete";
    eventObj.dispatch();
}