Imports System.Xml.Linq
Imports System.IO
Imports System.Xml
Partial Class other_news_detail_all
    Inherits System.Web.UI.Page

    Protected Sub Page_LoadComplete(sender As Object, e As System.EventArgs) Handles Me.LoadComplete
        If String.IsNullOrEmpty(Request("password")) Then
            Response.End()
            Exit Sub
        End If
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)

        Dim ss As String() = Request("date").Split("-")
        Dim compose_date As Date = New Date(ss(2), ss(0), ss(1))

        Dim news_column = API.NewsColumn(Request("column_id"))
        Dim nr = API.Newsroom()

        Dim test As String = Session("groupid")

        'Dim sql = String.Format("(select  distinct group_id, filecode ,layout_file,0 type,txt_title,txt_words,'','',sign_time,sign_usercode,title2,title3,txt_style from v_textlib  where column_id={0} and column_date='{1}' union select t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type,t2.photo_title,0,t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','' from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode where t2.column_id={0} and t2.column_date='{1}' and islayout =1  )order by type", Request("column_id"), Request("date"))
        'Dim sql = String.Format("(select  distinct column_date,group_id, filecode ,layout_file,0 type,txt_title,txt_words,'','',sign_time,sign_usercode,title2,title3,txt_style from v_textlib where column_date='3-9-2015' union select column_date,t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type,t2.photo_title,0,t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','' from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode where islayout =1 and column_date='3-9-2015')order by type,column_date")
        Dim sql = String.Format("(select  distinct t1.paper_code,paper_id, layout_code,column_code, v1.group_id, filecode ,layout_file,0 type,txt_title,txt_words,'','',sign_time,sign_usercode,title2,title3,txt_style ,'' caption_file from v_textlib v1,news_column t1, news_paper t2, column_permission t3 where v1.column_id <> {0} and column_date='{1}' and v1.column_id = t1.column_id and t1.paper_code = t2.paper_code and t1.column_id=t3.column_id and perm_style='C' and t3.group_id in ({2}) " _
                               & " union select t1.paper_code,paper_id, layout_code,column_code, t3.typeset_group_id, cast (t2.photo_id as varchar(30)),layoutfile,1 type,t2.photo_title,0,t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','', caption_file from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode,news_column t1,news_paper t4,column_permission t5 where t2.column_id<>{0} and t2.column_date='{1}' and islayout =1  and t2.column_id = t1.column_id and t1.paper_code = t4.paper_code and t1.column_id=t5.column_id and perm_style='C' and t5.group_id in ({2}) " _
                               & ")order by paper_id, column_code, type", Request("column_id"), Request("date"), test)
        Dim dt = DB.GetDataTable(sql)

        Dim sw As New StringWriter()
        dt.WriteXml(sw)

        Dim xd As New System.Xml.XmlDocument()
        xd.LoadXml(sw.ToString)
        Dim path As XmlElement
        Dim items = xd.SelectNodes("//item")

        Dim j As Integer = 0
        For Each item As XmlNode In items
            Try
                Dim paper_id As String = "_" + item.SelectSingleNode("paper_id").InnerText

                Dim message = xd.CreateElement("msg")
                message.InnerText = item.SelectSingleNode("layout_file").InnerText
                item.PrependChild(message)

                Dim label = xd.CreateElement("label")
                'remove enter
                label.InnerText = String.Format("{0}({1})", item.SelectSingleNode("txt_title").InnerText, item.SelectSingleNode("txt_words").InnerText).Replace(vbCrLf, " ")
                item.PrependChild(label)


                Dim filecode As String = item.SelectSingleNode("filecode").InnerText
                Dim type As Integer = Integer.Parse(item.SelectSingleNode("type").InnerText)

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

                Dim d = MyDate.Format(compose_date, datefmt)
                Select Case type
                    Case 0
                        Dim t As String = ""
                        Select Case subdir
                            Case "0DATE\CODE"
                                t = d & "\" & item.SelectSingleNode("layout_code").InnerText
                            Case "1CODE\DATE"
                                t = item.SelectSingleNode("layout_code").InnerText & "\" & d
                            Case "2CODE"
                                t = item.SelectSingleNode("layout_code").InnerText
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

                    Case 1
                        path = xd.CreateElement("path")
                        path.InnerText = item.SelectSingleNode("layout_file").InnerText
                        item.PrependChild(path)

                        Dim p_item As XmlNode = xd.CreateNode(XmlNodeType.Element, "item", Nothing)

                        label = xd.CreateElement("label")
                        label.InnerText = "說明"

                        path = xd.CreateElement("path")
                        path.InnerText = item.SelectSingleNode("caption_file").InnerText

                        Dim x_type = xd.CreateElement("type")
                        x_type.InnerText = "0"

                        p_item.AppendChild(label)
                        p_item.AppendChild(path)
                        p_item.AppendChild(x_type)
                        item.AppendChild(p_item)

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


                If type = 0 Then
                    Dim attachs As New XmlDocument()
                    Dim l_sql = String.Format("select attach_name,type,attach_size,upload_date,username from attachs where filecode='{0}' and del_user is null", filecode)
                    Dim l_dt = DB.GetDataTable(l_sql)
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
                            path.InnerText = String.Format("{0}\{1}\{2}{3}", attach_path, attach_date.ToString("yyyyMMdd"), filecode, attach.SelectSingleNode("attach_name").InnerText)

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

            End Try
        Next
        DB.Close()

        Dim i As Integer = 0
        For Each item As XmlNode In items
            Dim filecode As String = item.SelectSingleNode("filecode").InnerText
            For Each subitem As XmlNode In xd.SelectNodes(String.Format("//item[Column1='{0}']", filecode))
                'path = xd.CreateElement("path")
                'path.InnerText = subitem.SelectSingleNode("layout_file").InnerText
                'subitem.PrependChild(path)
                item.AppendChild(subitem)
            Next
            i = i + 1

        Next
        If (i <> items.Count) Then
            Response.Write("error")
        End If

        'Dim xd1 As XDocument = XDocument.Parse(xd.InnerXml)
        Response.Write(xd.InnerXml)
        Response.End()
    End Sub
End Class
