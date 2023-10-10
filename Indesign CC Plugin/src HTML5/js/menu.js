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
var server;
var username = "";
var password;
var enPassword;
var role;
var roleName
var action;

(function () {
    'use strict';

    
    function init() {
                
        themeManager.init();
    }
        
    init();
    csInterface.evalScript('loadLib()');
}());

function onLoaded() {
    csInterface.evalScript('createMenu()');
    csInterface.evalScript('addListener()');

    /*csInterface.addEventListener("applicationBeforeQuit", function (csEvent) {
        csInterface.evalScript('removeMenu()');
    });*/

    csInterface.addEventListener("Start Compose", function(evt) {
        action = "compose";
        if (username == "")
        {
            csInterface.requestOpenExtension("founder.Compose.login", "");
        }
        else
        {
            csInterface.requestOpenExtension("founder.Compose.newsroomDialog", "");
        }
    });
    
    csInterface.addEventListener("PreCompose", function(evt) {
        action = "preCompose";
        if (username == "")
            csInterface.requestOpenExtension("founder.Compose.login", "");
        else
            csInterface.requestOpenExtension("founder.Compose.templateDialog", "");
       
    });

    csInterface.addEventListener("Export Jpg", function(evt) {
        var col = evt.data.split("|");

        csInterface.requestOpenExtension("founder.Compose.preview", "");  
        
        function previewdata(){
            csInterface.removeEventListener("need preview data", previewdata);
            setTimeout( function(){
                var event = new CSEvent("return preview data", "APPLICATION");
                var data = new Object();
                data.path = col[0];
                data.type = col[1];
                data.PSPath = col[2];
                event.data = data;
                csInterface.dispatchEvent(event);
            },400);
        }
        csInterface.removeEventListener("need preview data", previewdata);
        csInterface.addEventListener("need preview data", previewdata);
    });
    
    csInterface.addEventListener("Open Change Password Dialog", function(evt) {
        action = "changePassword";
        if (username == "")
            csInterface.requestOpenExtension("founder.Compose.login", "");
        else
            csInterface.requestOpenExtension("founder.Compose.changePasswordDialog", "");
       
    });
    
    csInterface.addEventListener("Open Change Role Dialog", function(evt) {
        action = "changeRole";
        if (!bMP())
            csInterface.requestOpenExtension("founder.Compose.changeRole", "");
        else
            csInterface.requestOpenExtension("founder.Compose.role", "");
    });

    csInterface.addEventListener("Open Setting Dialog", function(evt) {
        action = "setting";
        if (username == "")
            csInterface.requestOpenExtension("founder.Compose.login", "");
        else
            csInterface.requestOpenExtension("founder.Compose.setting", "");
    });

    csInterface.addEventListener("Logout", function(evt) {
        Logout();
    });

    csInterface.addEventListener("Open About Dialog", function(evt) {
        csInterface.requestOpenExtension("founder.Compose.aboutDialog", "");
    });

	csInterface.addEventListener("logindata", function (event){
        server = event.data.server;
		username = event.data.username;
        password = event.data.password;
        enPassword = event.data.enPassword;
        role = event.data.role;
        roleName = event.data.roleName;
    });
    
    csInterface.addEventListener("need logindata", function (event){
        setTimeout( function()
        {
            var event = new CSEvent("return logindata", "APPLICATION");
            event.data = GetLoginData();
            csInterface.dispatchEvent(event);
        },400);
    });
    
    csInterface.addEventListener("Open Loading Dialog", function(evt) {
        csInterface.requestOpenExtension("founder.Compose.loadingDialog", "");
    });
};

function GetLoginData()
{
    csInterface.evalScript('setLogin("true")');
    var loginData = new Object();
    loginData.server = server;
    loginData.username = username;
    loginData.password = password;
    loginData.enPassword = enPassword;
    loginData.role = role;
    loginData.roleName = roleName;
    loginData.action = action;
    return loginData;
};

function Logout()
{
    csInterface.evalScript('setLogin("false")', function(event){
        if (event == "true"){
            server = "";
            username = "";
            password = "";
            enPassword = "";
            role = "";
            roleName = "";
            action = "";
        }
    });
};