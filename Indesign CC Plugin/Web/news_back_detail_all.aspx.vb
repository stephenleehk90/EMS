Imports System.Xml.Linq
Imports System.IO
Imports System.Xml
Partial Class news_back_detail_all
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
        Dim attach_date As Date = New Date(ss(2), ss(0), ss(1))
        attach_date = attach_date.AddDays(-1)
        Dim compose_date As Date = New Date(ss(2), ss(0), ss(1))

        Dim news_column = API.NewsColumn(Request("column_id"))
        Dim nr = API.Newsroom()

        Dim sql = String.Format("(select distinct paper_id,t2.filecode ,layout_file,0 type,txt_title,t2.txt_words,'','',sign_time,sign_usercode,title2,title3,txt_style FROM paper_filelist t1 INNER JOIN text_lib t2 ON t1.filecode = t2.filecode INNER JOIN text_info t3 ON t1.filecode = t3.filecode LEFT OUTER JOIN file_photo_relation t4 ON t2.filecode = t4.filecode LEFT OUTER JOIN deptfile_info t5 ON t2.filecode = t5.filecode,news_column t6, news_paper t7  WHERE t1.islayout = 0 and t1.column_id={0} and t1.column_date='{1}' and sign_time is not NULL  and t1.column_id = t6.column_id and t6.paper_code = t7.paper_code union select paper_id,cast (f2.photo_id as varchar(30)),layoutfile,1 type,f2.photo_title,0,f3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode, '','','' from paper_photolist f2 left outer join file_photo_relation f3 on cast (f2.photo_id as varchar(30))=f3.photocode,news_column t4, news_paper t5 where f2.column_id={0} and f2.column_date='{1}' and islayout =0 and sign_time is not NULL  and f2.column_id = t4.column_id and t4.paper_code = t5.paper_code )order by type", Request("column_id"), Request("date"))
        Dim dt = DB.GetDataTable(sql)

        Dim sw As New StringWriter()
        dt.WriteXml(sw)

        Dim xd As New System.Xml.XmlDocument()
        xd.LoadXml(sw.ToString)
        Dim path As XmlElement
        Dim items = xd.SelectNodes("//item")


        For Each item As XmlNode In items
            Dim paper_id As String = "_" + item.SelectSingleNode("paper_id").InnerText

            Dim message = xd.CreateElement("msg")
            Dim label = xd.CreateElement("label")
            'remove enter
            label.InnerText = String.Format("{0}({1})", item.SelectSingleNode("txt_title").InnerText, item.SelectSingleNode("txt_words").InnerText).Replace(vbCrLf, " ")
            item.PrependChild(label)


            Dim filecode As String = item.SelectSingleNode("filecode").InnerText
            Dim type As Integer = Integer.Parse(item.SelectSingleNode("type").InnerText)

            Dim attach_path As String
            If (nr.Element("files_attach_path" + paper_id) Is Nothing) Then
                attach_path = nr.Element("files_attach_path").Value
            ElseIf (nr.Element("files_attach_path" + paper_id).Value = "") Then
                attach_path = nr.Element("files_attach_path").Value
            Else
                attach_path = nr.Element("files_attach_path" + paper_id).Value
            End If

            'find photo description
            If IsNumeric(item.SelectSingleNode("filecode").InnerText) Then
                Dim p_sql = String.Format("select caption_file from paper_photolist where photo_id={0}", item.SelectSingleNode("filecode").InnerText)
                Dim p_sw As New StringWriter()
                DB.GetDataTable(p_sql).WriteXml(p_sw)
                Dim p_document As New XmlDocument()
                p_document.LoadXml(p_sw.ToString)

                '說明
                Dim p_item As XmlNode = xd.CreateNode(XmlNodeType.Element, "item", Nothing)
                item.AppendChild(p_item)

                label = xd.CreateElement("label")
                label.InnerText = "說明"
                p_item.AppendChild(label)

                Dim x_type = xd.CreateElement("type")
                x_type.InnerText = "0"
                p_item.AppendChild(x_type)


            End If



            Dim attachs As New XmlDocument()
            Dim l_sql = String.Format("select attach_name,type,attach_size,upload_date,username from attachs where filecode='{0}' and del_user is null", filecode)
            Dim l_dt = DB.GetDataTable(l_sql)
            Dim l_sw As New StringWriter()
            l_dt.WriteXml(l_sw)

            'attachment
            attachs.LoadXml(l_sw.ToString())
            For Each attach As XmlNode In attachs.SelectNodes("//item")
                Dim new_attach As XmlNode = xd.CreateNode(XmlNodeType.Element, "item", Nothing)
                new_attach.InnerXml = attach.InnerXml

                message = xd.CreateElement("msg")
                message.InnerText = attach.SelectSingleNode("attach_name").InnerText
                new_attach.PrependChild(message)

                label = xd.CreateElement("label")
                label.InnerText = attach.SelectSingleNode("attach_name").InnerText
                new_attach.PrependChild(label)

                path = xd.CreateElement("path")
                path.InnerText = String.Format("{0}\{1}\{2}{3}", attach_path, attach_date.ToString("yyyyMMdd"), filecode, attach.SelectSingleNode("attach_name").InnerText)
                new_attach.PrependChild(path)

                item.AppendChild(new_attach)
            Next
        Next
        DB.Close()

        For Each item As XmlNode In items
            Dim filecode As String = item.SelectSingleNode("filecode").InnerText
            For Each subitem As XmlNode In xd.SelectNodes(String.Format("//item[Column1='{0}']", filecode))
                'path = xd.CreateElement("path")
                'path.InnerText = subitem.SelectSingleNode("layout_file").InnerText
                'subitem.PrependChild(path)
                item.AppendChild(subitem)
            Next

        Next

        'Dim xd1 As XDocument = XDocument.Parse(xd.InnerXml)
        Response.Write(xd.InnerXml)
        Response.End()
    End Sub
End Class
