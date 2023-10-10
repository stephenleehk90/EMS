var settingpath = '';
function LoadSettingXML(){

    var SettingData = new Object();
    try {
        settingpath = '';
        var csInterface = new CSInterface();
        var docpath = csInterface.getSystemPath(SystemPath.MY_DOCUMENTS);
        var splitHostApp = docpath.split('/');
        var local_temp = "";
        for (var i = 0; i < splitHostApp.length - 1; i++)
        {
            settingpath += splitHostApp[i] + '/';
        }
        settingpath += "EMS6/";
        local_temp = settingpath;
        var fs = require('fs');
		var fse = require('fs-extra');
        if (!fs.existsSync(settingpath)){
            fse.mkdirSync(settingpath);
        }
        if (!fs.existsSync(settingpath)){
            settingpath = splitHostApp[0] + "EMS6/";
            fse.mkdirSync(settingpath);
        }
        settingpath += "setting.xml";

        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function() {
            SettingData.username="";
            SettingData.server_name="";
            SettingData.local_temp = local_temp;
            SettingData.document_width=300;
            SettingData.document_height=400;
            SettingData.refresh_time=15;
            SettingData.font="PMingLiU\tRegular";
            SettingData.fontDefault = true;
            SettingData.template="";
            SettingData.layoutCode=false;
            
            SettingData.debug=false;
            //enableDebugLog = true; //17-11-2016
            SettingData.normalLog = true; //17-11-2016
            SettingData.systemLog = true; //17-11-2016
            SettingData.composeCompleteLog = true; //17-11-2016
            
            SettingData.save_jpg = true;
            SettingData.complete_jpg = true;
            SettingData.jpg_preview=false
            SettingData.jpg_preview_save=false
            SettingData.jpg_ppi = 72;
            /*SettingData.createServerPDFonly = false;
            SettingData.toleranceSecond = 0;*/
            SettingData.jpg_quality = 3;
                
            if (bMP())
            {
                SettingData.subPath=0;
                SettingData.dateFmt="4YYYYMMDD";
                SettingData.pdf_prefix_ckb = false;
                SettingData.pdf_prefix="";
                SettingData.pdf_suffix_ckb = false;
                SettingData.pdf_suffix="";
                SettingData.role="";
                SettingData.allowDragArticle = true;
                //tagServer = "";
            }
            
            /*if (CONFIG::HKET)
            {
                document_pos = 0;
                
                preview = false;
                preview_width= 300;
                //preview_height=300;
                preview_delay = 1;
            
                template_Pub="";
                other_Pub="";
                typeset_Pub="";
                template_cb = 0;
                msg_cb = 0;
                typeset_name_cb =0;
                delete_chk =false;
                compose_complete_chk =false;
                
                template_rb = "template_rb1";
            }*/

            if (this.status != "404" && fs.existsSync(settingpath)) {
                xmlDoc = xmlhttp.responseXML;
                x = xmlDoc.documentElement.childNodes;
                for (i = 0; i < x.length; i++) {
                    if (x[i].nodeType == 1) {
                        if (x[i].nodeName == "username")
                            SettingData.username = x[i].innerHTML;
                        if (x[i].nodeName == "server_name")
                            SettingData.server_name = x[i].innerHTML;
                        if (x[i].nodeName == "local_temp")
                            SettingData.local_temp = x[i].innerHTML;
                        if (x[i].nodeName == "font"){
                            SettingData.font = x[i].innerHTML;
                            SettingData.font = SettingData.font.replace("\t","\xA0\xA0\xA0");
                        }
                        if (x[i].nodeName == "fontDefault")
                            SettingData.fontDefault = x[i].innerHTML == "true";
                        if (x[i].nodeName == "template")
                            SettingData.template = x[i].innerHTML;
                        if (x[i].nodeName == "pdf")
                            SettingData.pdf = x[i].innerHTML;
                        if (x[i].nodeName == "save_jpg")
                            SettingData.save_jpg = x[i].innerHTML == "true";
                        if (x[i].nodeName == "complete_jpg")
                            SettingData.complete_jpg = x[i].innerHTML == "true";
                        if (x[i].nodeName == "jpg_preview")
                            SettingData.jpg_preview = x[i].innerHTML == "true";
                        if (x[i].nodeName == "jpg_preview_save")
                            SettingData.jpg_preview_save = x[i].innerHTML == "true";
                        if (x[i].nodeName == "jpg_quality")
                            SettingData.jpg_quality = x[i].innerHTML;
                        if (x[i].nodeName == "jpg_resolution")
                            SettingData.jpg_ppi = x[i].innerHTML;
                        if (x[i].nodeName == "document_width")
                            SettingData.document_width = x[i].innerHTML;
                        if (x[i].nodeName == "document_height")
                            SettingData.document_height = x[i].innerHTML;
                        if (x[i].nodeName == "refresh_time")
                            SettingData.refresh_time = x[i].innerHTML;
                        if (x[i].nodeName == "layoutCode")
                            SettingData.layoutCode = x[i].innerHTML == "true";
                        if (x[i].nodeName == "normalLog")
                            SettingData.normalLog = x[i].innerHTML == "true";
                        if (x[i].nodeName == "systemLog")
                            SettingData.systemLog = x[i].innerHTML == "true";
                        if (x[i].nodeName == "composeCompleteLog")
                            SettingData.composeCompleteLog = x[i].innerHTML == "true";
                        /*if (x[i].nodeName == "createServerPDFonly")
                            SettingData.createServerPDFonly = x[i].innerHTML;
                        if (x[i].nodeName == "toleranceSecond")
                            SettingData.toleranceSecond = x[i].innerHTML;*/
                        
                        /*if (bHKET)
                        {
                            x.appendChild(<document_pos>{document_pos}</document_pos>);
                            x.appendChild(<preview>{preview}</preview>);
                            x.appendChild(<preview_width>{preview_width}</preview_width>);
                            //x.appendChild(<preview_height>{preview_height}</preview_height>);
                            x.appendChild(<preview_delay>{preview_delay}</preview_delay>);
                            x.appendChild(<template_cb>{template_cb}</template_cb>);
                            x.appendChild(<msg_cb>{msg_cb}</msg_cb>);
                            x.appendChild(<typeset_name_cb>{typeset_name_cb}</typeset_name_cb>);
                            x.appendChild(<delete_chk>{delete_chk}</delete_chk>);
                            x.appendChild(<compose_complete_chk>{compose_complete_chk}</compose_complete_chk>);
                            x.appendChild(<template_rb>{template_rb}</template_rb>);
                            x.appendChild(<typeset_Pub>{typeset_Pub}</typeset_Pub>);
                            x.appendChild(<other_Pub>{other_Pub}</other_Pub>);
                            x.appendChild(<template_Pub>{template_Pub}</template_Pub>);
                        }*/
                        if (bMP())
                        {
                            if (x[i].nodeName == "subPath")
                                SettingData.subPath = x[i].innerHTML;
                            if (x[i].nodeName == "dateFmt")
                                SettingData.dateFmt = x[i].innerHTML;
                            if (x[i].nodeName == "pdf_prefix_ckb")
                                SettingData.pdf_prefix_ckb = x[i].innerHTML;
                            if (x[i].nodeName == "pdf_prefix")
                                SettingData.pdf_prefix = x[i].innerHTML;
                            if (x[i].nodeName == "pdf_suffix_ckb")
                                SettingData.pdf_suffix_ckb = x[i].innerHTML;
                            if (x[i].nodeName == "pdf_suffix")
                                SettingData.pdf_suffix = x[i].innerHTML;
                            if (x[i].nodeName == "role")
                                SettingData.role = x[i].innerHTML;
                            if (x[i].nodeName == "allowDragArticle")
                                SettingData.allowDragArticle = x[i].innerHTML == "true";
                            //x.appendChild(<tagServer>{tagServer}</tagServer>);
                        }
                        if (x[i].nodeName == "debug")
                            SettingData.debug = x[i].innerHTML;
                    }
                }
            }
            else
                SaveSettingXML(SettingData);
        }
        xmlhttp.open("GET", settingpath, false);
        xmlhttp.send();
    }
    catch(err) {
        ErrorLog(err.message,true,0);
    }
    return SettingData;
};

function SaveSettingXML(SettingData)
{
    var data;
    try
    {
        var parser = new DOMParser();
        var doc = parser.parseFromString('<xml/>', 'text/xml');
        var root = doc.documentElement

        var nodeName = doc.createElement("username");
        var nodeValue = doc.createTextNode(SettingData.username);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("server_name");
        nodeValue = doc.createTextNode(SettingData.server_name);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("local_temp");
        nodeValue = doc.createTextNode(SettingData.local_temp);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("font");
        SettingData.font = SettingData.font.replace("\xA0\xA0\xA0","\t");
        nodeValue = doc.createTextNode(SettingData.font);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("fontDefault");
        nodeValue = doc.createTextNode(SettingData.fontDefault);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("template");
        nodeValue = doc.createTextNode(SettingData.template);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("pdf");
        nodeValue = doc.createTextNode(SettingData.pdf);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("save_jpg");
        nodeValue = doc.createTextNode(SettingData.save_jpg);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("complete_jpg");
        nodeValue = doc.createTextNode(SettingData.complete_jpg);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("jpg_preview");
        nodeValue = doc.createTextNode(SettingData.jpg_preview);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("jpg_preview_save");
        nodeValue = doc.createTextNode(SettingData.jpg_preview_save);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("jpg_quality");
        nodeValue = doc.createTextNode(SettingData.jpg_quality);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("jpg_resolution");
        nodeValue = doc.createTextNode(SettingData.jpg_ppi);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("document_width");
        nodeValue = doc.createTextNode(SettingData.document_width);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("document_height");
        nodeValue = doc.createTextNode(SettingData.document_height);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("refresh_time");
        nodeValue = doc.createTextNode(SettingData.refresh_time);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("layoutCode");
        nodeValue = doc.createTextNode(SettingData.layoutCode);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("normalLog");
        nodeValue = doc.createTextNode(SettingData.normalLog);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("systemLog");
        nodeValue = doc.createTextNode(SettingData.systemLog);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("composeCompleteLog");
        nodeValue = doc.createTextNode(SettingData.composeCompleteLog);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        /*nodeName = doc.createElement("createServerPDFonly");
        nodeValue = doc.createTextNode(SettingData.createServerPDFonly);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        nodeName = doc.createElement("toleranceSecond");
        nodeValue = doc.createTextNode(SettingData.toleranceSecond);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);*/
        /*if (CONFIG::HKET)
			{
				x.appendChild(<document_pos>{document_pos}</document_pos>
				x.appendChild(<preview>{preview}</preview>
				x.appendChild(<preview_width>{preview_width}</preview_width>
				//x.appendChild(<preview_height>{preview_height}</preview_height>
				x.appendChild(<preview_delay>{preview_delay}</preview_delay>
				x.appendChild(<template_cb>{template_cb}</template_cb>
				x.appendChild(<msg_cb>{msg_cb}</msg_cb>
				x.appendChild(<typeset_name_cb>{typeset_name_cb}</typeset_name_cb>
				x.appendChild(<delete_chk>{delete_chk}</delete_chk>
				x.appendChild(<compose_complete_chk>{compose_complete_chk}</compose_complete_chk>
				x.appendChild(<template_rb>{template_rb}</template_rb>
				x.appendChild(<typeset_Pub>{typeset_Pub}</typeset_Pub>
				x.appendChild(<other_Pub>{other_Pub}</other_Pub>
				x.appendChild(<template_Pub>{template_Pub}</template_Pub>
			}*/
        if (bMP())
        {
            nodeName = doc.createElement("subPath");
            nodeValue = doc.createTextNode(SettingData.subPath);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            nodeName = doc.createElement("dateFmt");
            nodeValue = doc.createTextNode(SettingData.dateFmt);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            nodeName = doc.createElement("pdf_prefix_ckb");
            nodeValue = doc.createTextNode(SettingData.pdf_prefix_ckb);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            nodeName = doc.createElement("pdf_prefix");
            nodeValue = doc.createTextNode(SettingData.pdf_prefix);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            nodeName = doc.createElement("pdf_suffix_ckb");
            nodeValue = doc.createTextNode(SettingData.pdf_suffix_ckb);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            nodeName = doc.createElement("pdf_suffix");
            nodeValue = doc.createTextNode(SettingData.pdf_suffix);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            nodeName = doc.createElement("role");
            nodeValue = doc.createTextNode(SettingData.role);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            nodeName = doc.createElement("allowDragArticle");
            nodeValue = doc.createTextNode(SettingData.allowDragArticle);
            nodeName.appendChild(nodeValue);
            root.appendChild(nodeName);
            //x.appendChild(<tagServer>{tagServer}</tagServer>
        }
        nodeName = doc.createElement("debug");
        nodeValue = doc.createTextNode(SettingData.debug);
        nodeName.appendChild(nodeValue);
        root.appendChild(nodeName);
        
        data = (new XMLSerializer()).serializeToString(doc);
        var reg = new RegExp('></', 'g')
        data = data.replace(reg,'>\t</');
        data = data.replace(/></g,'>\n  <');
        reg = new RegExp('>\t</', 'g')
        data = data.replace(reg,'></');
        data = data.replace('></xml>','>\n</xml>');
        var fs = require('fs');
        fs.writeFileSync(settingpath, data);
    }
    catch(err) {
        ErrorLog(err.message,true,0);
    }
};