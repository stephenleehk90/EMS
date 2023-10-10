Imports System.Xml.Linq
Imports System.IO
Imports System.Xml
Partial Class news_detail_all
    Inherits System.Web.UI.Page

    Protected Sub Page_LoadComplete(sender As Object, e As System.EventArgs) Handles Me.LoadComplete
        If String.IsNullOrEmpty(Request("password")) Then
            Response.End()
            Exit Sub
        End If
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))

        'Pat news_detail_all crash server test solution=========================================
        'for stop double loop in refresh
        Session("news_details_all_status") = "start_process"

        'Check login
        If Not DB.ChkLogin(username, password) Then
            Response.End()
        End If

        'Use same connection
        If Not (DB.Chk_Connection_keep_open) Then 'Use old connection
            If Not DB.Init_keep_open(username, password) Then
                Response.End()
            End If
        End If

        'DB.Init(username, password)
        '=======================================================================================

        Dim company As String = Request("company")
        Dim ss As String() = Request("date").Split("-")
        Dim compose_date As Date = New Date(ss(2), ss(0), ss(1))

        Dim news_column As XElement
        If (company = "MP") Then
            news_column = API.NewsColumn_MP_keep_open(Request("column_id"), Request("date"))
        Else
            news_column = API.NewsColumn_keep_open(Request("column_id"))
        End If

        Dim nr = API.Newsroom_keep_open()

        Dim sql As String
        Dim strReplace As String = "REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE("
        Dim strRegex As String = ",CHAR(0),''),CHAR(1),''),CHAR(2),''),CHAR(3),''),CHAR(4),''),CHAR(5),''),CHAR(6),''),CHAR(7),''),CHAR(8),''),CHAR(9),''),CHAR(11),''),CHAR(12),''),CHAR(14),''),CHAR(15),''),CHAR(16),''),CHAR(17),''),CHAR(18),''),CHAR(19),''),CHAR(20),''),CHAR(21),''),CHAR(22),''),CHAR(23),''),CHAR(24),''),CHAR(25),''),CHAR(26),''),CHAR(27),''),CHAR(28),''),CHAR(29),''),CHAR(30),''),CHAR(31),'')"
        If (company = "HKET") Then
            sql = String.Format("(select  distinct paper_id,group_id, filecode ,layout_file,0 type," + strReplace + "txt_title" + strRegex + " txt_title,txt_words,'','' note,sign_time,sign_usercode,title2,title3,txt_style,'' caption_file from v_textlib v1,news_column t1, news_paper t2 where v1.column_id={0} and column_date='{1}' and v1.column_id = t1.column_id and t1.paper_code = t2.paper_code union select paper_id,t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type," + strReplace + "t2.photo_title" + strRegex + ",len(t2.photo_title),t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','',caption_file from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode, news_column t4, news_paper t5 where t2.column_id={0} and t2.column_date='{1}' and islayout =1 and t2.column_id = t4.column_id and t4.paper_code = t5.paper_code )order by txt_title", Request("column_id"), Request("date"))
        ElseIf (company = "MP") Then
            sql = String.Format("(select  distinct paper_id,group_id, filecode ,layout_file,0 type," + strReplace + "txt_title" + strRegex + " txt_title,txt_words,'','' note,sign_time,sign_usercode,title2,title3,txt_style,'' caption_file from v_textlib v1,news_column t1, news_paper t2 where v1.column_id={0} and column_date='{1}' and v1.column_id = t1.column_id and t1.paper_code = t2.paper_code union select paper_id,t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type," + strReplace + "t2.photo_title" + strRegex + ",len(t2.photo_title),t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','',caption_file from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode, news_column t4, news_paper t5 where t2.column_id={0} and t2.column_date='{1}' and islayout =1 and t2.column_id = t4.column_id and t4.paper_code = t5.paper_code )order by type", Request("column_id"), Request("date"))
        Else
            sql = String.Format("(select  distinct paper_id,group_id, filecode ,layout_file,0 type," + strReplace + "txt_title" + strRegex + " txt_title,txt_words,'','' note,sign_time,sign_usercode,title2,title3,txt_style,'' caption_file from v_textlib v1,news_column t1, news_paper t2 where v1.column_id={0} and column_date='{1}' and v1.column_id = t1.column_id and t1.paper_code = t2.paper_code union select paper_id,t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type," + strReplace + "t2.photo_title" + strRegex + ",0,t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','',caption_file from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode, news_column t4, news_paper t5 where t2.column_id={0} and t2.column_date='{1}' and islayout =1 and t2.column_id = t4.column_id and t4.paper_code = t5.paper_code )order by type", Request("column_id"), Request("date"))
        End If
        'Dim sql = String.Format("(select  distinct column_date,group_id, filecode ,layout_file,0 type,txt_title,txt_words,'','',sign_time,sign_usercode,title2,title3,txt_style from v_textlib where column_date='3-9-2015' union select column_date,t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type,t2.photo_title,0,t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','' from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode where islayout =1 and column_date='3-9-2015')order by type,column_date")
        'Pat news_detail_all crash server test solution=========================================
        'Dim dt = DB.GetDataTable(sql)
        Dim dt = DB.GetDataTable_keep_open(sql)
        '=======================================================================================
        Dim sw As New StringWriter()
        dt.WriteXml(sw)

        Dim xd As New System.Xml.XmlDocument()
        xd.LoadXml(sw.ToString)
        Dim path As XmlElement
        Dim items = xd.SelectNodes("//item")

        Dim j As Integer = 0
        'Pat news_detail_all crash server test solution=========================================
        'for stop double loop in refresh
        Session("news_details_all_status") = "in_loop1"
        '=======================================================================================
        For Each item As XmlNode In items
            Try
                'Pat news_detail_all crash server test solution=========================================
                'for stop double loop in refresh
                Dim strStatus As String
                strStatus = Session("news_details_all_status")
                If Not strStatus.Equals("in_loop1") Then
                    Response.End()
                End If
                '=======================================================================================

                Dim paper_id As String = "_" + item.SelectSingleNode("paper_id").InnerText
                Dim type As Integer = Integer.Parse(item.SelectSingleNode("type").InnerText)

                Dim message = xd.CreateElement("msg")
                message.InnerText = item.SelectSingleNode("layout_file").InnerText

                item.PrependChild(message)

                Dim message2 = xd.CreateElement("msg2")
                If (type = 0) Then
                    message2.InnerText = item.SelectSingleNode("layout_file").InnerText
                Else
                    If (item.SelectSingleNode("note") Is Nothing) Then
                        message2.InnerText = ""
                    Else
                        message2.InnerText = item.SelectSingleNode("note").InnerText
                    End If
                End If

                item.PrependChild(message2)

                Dim label = xd.CreateElement("label")
                'remove enter
                If (company = "HKET") Then
                    Dim word As String = item.SelectSingleNode("txt_words").InnerText
                    If (word <> "0.0") Then
                        label.InnerText = String.Format("{0}({1})", item.SelectSingleNode("txt_title").InnerText, word.Substring(0, word.Length() - 2)).Replace(vbCrLf, " ").Replace(vbCr, " ").Replace(vbLf, " ")
                    Else
                        label.InnerText = String.Format("{0}", item.SelectSingleNode("txt_title").InnerText).Replace(vbCrLf, " ").Replace(vbCr, " ").Replace(vbLf, " ")
                    End If
                ElseIf (company = "MP") Then
                    If (type = 1) Then
                        Dim title As String = item.SelectSingleNode("layout_file").InnerText
                        title = title.Substring(title.LastIndexOf("\") + 1)
                        label.InnerText = String.Format("[{0}]{1}", title.Substring(0, title.LastIndexOf(".")), item.SelectSingleNode("txt_title").InnerText).Replace(vbCrLf, " ").Replace(vbCr, " ").Replace(vbLf, " ")
                    Else
                        label.InnerText = String.Format("{0}({1})", item.SelectSingleNode("txt_title").InnerText, item.SelectSingleNode("txt_words").InnerText).Replace(vbCrLf, " ").Replace(vbCr, " ").Replace(vbLf, " ")
                    End If
                Else
                    label.InnerText = String.Format("{0}({1})", item.SelectSingleNode("txt_title").InnerText, item.SelectSingleNode("txt_words").InnerText).Replace(vbCrLf, " ").Replace(vbCr, " ").Replace(vbLf, " ")
                End If

                item.PrependChild(label)


                Dim filecode As String = item.SelectSingleNode("filecode").InnerText

                Dim datefmt As String
                If (nr.Element("paper_lib_datefmt" + paper_id) Is Nothing) Then
                    datefmt = nr.Element("paper_lib_datefmt").Value
                ElseIf (nr.Element("paper_lib_datefmt" + paper_id).Value = "") Then
                    datefmt = nr.Element("paper_lib_datefmt").Value
                Else
                    datefmt = nr.Element("paper_lib_datefmt" + paper_id).Value
                End If

                Dim subdir As String
                If (nr.Element("paper_lib_subdir" + paper_id) Is Nothing) Then
                    subdir = nr.Element("paper_lib_subdir").Value
                ElseIf (nr.Element("paper_lib_subdir" + paper_id).Value = "") Then
                    subdir = nr.Element("paper_lib_subdir").Value
                Else
                    subdir = nr.Element("paper_lib_subdir" + paper_id).Value
                End If

                Dim server As String
                If (nr.Element("paper_server" + paper_id) Is Nothing) Then
                    server = nr.Element("paper_server").Value
                ElseIf (nr.Element("paper_server" + paper_id).Value = "") Then
                    server = nr.Element("paper_server").Value
                Else
                    server = nr.Element("paper_server" + paper_id).Value
                End If

                Dim share As String
                If (nr.Element("paper_share" + paper_id) Is Nothing) Then
                    share = nr.Element("paper_share").Value
                ElseIf (nr.Element("paper_share" + paper_id).Value = "") Then
                    share = nr.Element("paper_share").Value
                Else
                    share = nr.Element("paper_share" + paper_id).Value
                End If

                Dim lib_path As String
                If (nr.Element("paper_lib_path" + paper_id) Is Nothing) Then
                    lib_path = nr.Element("paper_lib_path").Value
                ElseIf (nr.Element("paper_lib_path" + paper_id).Value = "") Then
                    lib_path = nr.Element("paper_lib_path").Value
                Else
                    lib_path = nr.Element("paper_lib_path" + paper_id).Value
                End If

                Dim attach_path As String
                If (nr.Element("files_attach_path" + paper_id) Is Nothing) Then
                    attach_path = nr.Element("files_attach_path").Value
                ElseIf (nr.Element("files_attach_path" + paper_id).Value = "") Then
                    attach_path = nr.Element("files_attach_path").Value
                Else
                    attach_path = nr.Element("files_attach_path" + paper_id).Value
                End If

                Dim code As String
                If (Not String.IsNullOrEmpty(news_column.<is_newspaper>.Value) And news_column.<is_newspaper>.Value = "false") Then
                    code = news_column.<publication_code>.Value & "\" & news_column.<period_no>.Value & "\" & news_column.<layout_code>.Value
                Else
                    code = news_column.<layout_code>.Value
                End If
                Dim d = MyDate.Format(compose_date, datefmt)
                Select Case type
                    Case 0
                        Dim t As String = ""
                        Select Case subdir
                            Case "0DATE\CODE"
                                t = d & "\" & code
                            Case "1CODE\DATE"
                                t = code & "\" & d
                            Case "2CODE"
                                t = code
                            Case "3DATE"
                                t = d
                        End Select
                        'path=App.Newsroom().PaperLibPath(paper_lib_date,news.layout_code,n.layout).resolvePath(n.layout_file).nativePath;
                        If (server.Substring(1, 1) <> ":") Then
                            server = "\\" + server
                        End If


                        Dim p = xd.CreateElement("path")
                        p.InnerText = String.Format("{0}\{1}\{2}\{3}\{4}", server, share, lib_path,
                                                    t, item.SelectSingleNode("layout_file").InnerText)
                        item.PrependChild(p)

                        If (Request("company") = "HKET") Then
                            attach_path = String.Format("{0}\{1}\{2}\{3}", server, share, lib_path, t)
                        End If

                    Case 1
                        path = xd.CreateElement("path")
                        path.InnerText = item.SelectSingleNode("layout_file").InnerText
                        item.PrependChild(path)

                End Select

                'find photo description
                'If IsNumeric(item.SelectSingleNode("filecode").InnerText) Then
                '    'If type = 1 Then
                '    Dim p_sql = String.Format("select caption_file from paper_photolist where photo_id={0}", item.SelectSingleNode("filecode").InnerText)
                '    Dim p_sw As New StringWriter()
                '    DB.GetDataTable(p_sql).WriteXml(p_sw)
                '    Dim p_document As New XmlDocument()
                '    p_document.LoadXml(p_sw.ToString)

                '    '說明
                '    Try
                '        Dim p_item As XmlNode = xd.CreateNode(XmlNodeType.Element, "item", Nothing)

                '        label = xd.CreateElement("label")
                '        label.InnerText = "說明"

                '        path = xd.CreateElement("path")
                '        path.InnerText = p_document.SelectSingleNode("//caption_file").InnerText

                '        Dim x_type = xd.CreateElement("type")
                '        x_type.InnerText = "0"

                '        p_item.AppendChild(label)
                '        p_item.AppendChild(path)
                '        p_item.AppendChild(x_type)
                '        item.AppendChild(p_item)

                '    Catch ex As Exception

                '    End Try


                'End If
                Dim caption_file As String = item.SelectSingleNode("caption_file").InnerText
                If Not String.IsNullOrEmpty(caption_file) Then
                    Dim p_item As XmlNode = xd.CreateNode(XmlNodeType.Element, "item", Nothing)

                    label = xd.CreateElement("label")
                    label.InnerText = "說明"

                    path = xd.CreateElement("path")
                    path.InnerText = caption_file

                    Dim x_type = xd.CreateElement("type")
                    x_type.InnerText = "0"

                    p_item.AppendChild(label)
                    p_item.AppendChild(path)
                    p_item.AppendChild(x_type)
                    item.AppendChild(p_item)
                End If
                item.RemoveChild(item.SelectSingleNode("caption_file"))


                If type = 0 Then
                    Dim attachs As New XmlDocument()
                    ''Dim l_sql = String.Format("select attach_name,type,attach_size,upload_date,username from attachs where filecode='{0}' and del_user is null", filecode)
                    Dim l_sql As String
                    If (Request("company") = "HKET") Then
                        l_sql = String.Format("select attach_name,type,attach_size,upload_date,username from attachs where filecode='{0}' and del_user is null order by attach_name", filecode)
                    Else
                        l_sql = String.Format("select attach_name,type,attach_size,upload_date,username from attachs where filecode='{0}' and del_user is null", filecode)
                    End If
                    'Pat news_detail_all crash server test solution=========================================
                    'Dim l_dt = DB.GetDataTable(l_sql)
                    Dim l_dt = DB.GetDataTable_keep_open(l_sql)
                    '=======================================================================================
                    Dim l_sw As New StringWriter()
                    l_dt.WriteXml(l_sw)

                    'attachment
                    attachs.LoadXml(l_sw.ToString())
                    For Each attach As XmlNode In attachs.SelectNodes("//item")
                        Try
                            Dim new_attach As XmlNode = xd.CreateNode(XmlNodeType.Element, "item", Nothing)
                            new_attach.InnerXml = attach.InnerXml

                            message = xd.CreateElement("msg")
                            message.InnerText = attach.SelectSingleNode("attach_name").InnerText

                            label = xd.CreateElement("label")
                            label.InnerText = attach.SelectSingleNode("attach_name").InnerText

                            ss = (attach.SelectSingleNode("upload_date").InnerText).Split("-")
                            Dim attach_date As Date = New Date(ss(0), ss(1), Left(ss(2), 2))

                            path = xd.CreateElement("path")

                            If (Request("company") = "HKET") Then
                                path.InnerText = String.Format("{0}\{1}_{2}", attach_path, item.SelectSingleNode("layout_file").InnerText.Substring(0, item.SelectSingleNode("layout_file").InnerText.LastIndexOf(".")), attach.SelectSingleNode("attach_name").InnerText)
                            Else
                                path.InnerText = String.Format("{0}\{1}\{2}{3}", attach_path, attach_date.ToString("yyyyMMdd"), filecode, attach.SelectSingleNode("attach_name").InnerText)
                            End If

                            new_attach.PrependChild(message)
                            new_attach.PrependChild(label)
                            new_attach.PrependChild(path)
                            item.AppendChild(new_attach)

                        Catch ex As Exception

                        End Try
                    Next
                End If

                j = j + 1
            Catch ex As Exception
                Dim temp As Integer
                temp = 1
            End Try
        Next
        'Pat news_detail_all crash server test solution=========================================
        'DB.Close()
        '=======================================================================================
        Dim i As Integer = 0
        If (company = "MP") Then
            'Pat news_detail_all crash server test solution=========================================
            'for stop double loop in refresh
            Session("news_details_all_status") = "in_loop2"
            '=======================================================================================
            For Each item As XmlNode In items
                'Pat news_detail_all crash server test solution=========================================
                'for stop double loop in refresh
                Dim strStatus As String
                strStatus = Session("news_details_all_status")
                If Not strStatus.Equals("in_loop2") Then
                    Response.End()
                End If
                '=======================================================================================
                Dim filecode As String = item.SelectSingleNode("filecode").InnerText
                Dim bRelated As Boolean = False
                For Each subitem As XmlNode In xd.SelectNodes(String.Format("//item[Column1='{0}']", filecode))
                    Dim relation As XmlElement = xd.CreateElement("item")
                    If (bRelated = False) Then
                        Dim root As XmlNode = xd.DocumentElement
                        Dim label = xd.CreateElement("label")
                        relation.AppendChild(label)
                        Dim type = xd.CreateElement("type")
                        type.InnerText = "group"
                        relation.AppendChild(type)
                        root.InsertBefore(relation, item)
                        relation.AppendChild(item)
                    End If
                    'path = xd.CreateElement("path")
                    'path.InnerText = subitem.SelectSingleNode("layout_file").InnerText
                    'subitem.PrependChild(path)
                    relation.AppendChild(subitem)
                Next
                i = i + 1

            Next
        Else
            'Pat news_detail_all crash server test solution=========================================
            'for stop double loop in refresh
            Session("news_details_all_status") = "in_loop2"
            '=======================================================================================
            For Each item As XmlNode In items
                'Pat news_detail_all crash server test solution=========================================
                'for stop double loop in refresh
                Dim strStatus As String
                strStatus = Session("news_details_all_status")
                If Not strStatus.Equals("in_loop2") Then
                    Response.End()
                End If
                '=======================================================================================
                Dim filecode As String = item.SelectSingleNode("filecode").InnerText
                For Each subitem As XmlNode In xd.SelectNodes(String.Format("//item[Column1='{0}']", filecode))
                    'path = xd.CreateElement("path")
                    'path.InnerText = subitem.SelectSingleNode("layout_file").InnerText
                    'subitem.PrependChild(path)
                    item.AppendChild(subitem)
                Next
                i = i + 1

            Next
        End If

        If (i <> items.Count) Then
            Response.Write("error")
        End If
        Dim strDate As String
        strDate = DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss")

        'Dim xd1 As XDocument = XDocument.Parse(xd.InnerXml)
        Response.Write(xd.InnerXml)
        Response.End()
    End Sub
End Class
