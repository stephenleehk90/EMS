Imports Microsoft.VisualBasic
Imports System.Data.SqlClient
Imports System.Xml
Imports System.IO
Imports System.Web

Public Class API

    Private Shared _session As String

    Private Shared Property Session(p1 As String) As String
        Get
            Return _session
        End Get
        Set(value As String)
            _session = value
        End Set
    End Property

    Public Shared Function NewsAttach(ByVal filecode As String) As String
        Dim sql = String.Format("select attach_name,type,attach_size,upload_date,username from attachs where filecode='{0}' and del_user is null", filecode)
        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("items")
        xml.AppendChild(root)

        Dim dr = DB.ExecuteReader(sql)

        Return API.WriteToXML(dr, "items", "item")
    End Function

    Public Shared Function UserGroup(ByRef usercode As String) As String
        Dim sql = String.Format("select  news_group.group_id, lib_name, group_name from user_group , news_group , news_user , news_lib where  user_group.usercode ='{0}'  and user_group.group_id = news_group.group_id  and news_user.usercode = user_group.usercode  and news_group.lib_id = news_lib.lib_id and (datepart(yy,perm_end )=1900 or perm_end>getdate())", usercode)

        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("groups")
        xml.AppendChild(root)

        Dim dr = DB.ExecuteReader(sql)

        Return API.WriteToXML(dr, "groups", "group")

    End Function

    Public Shared Function NewsDetail(ByVal column_id As String) As String
        Dim d As String = SpanOfDay(1)
        Dim sql = String.Format("(select  distinct group_id, filecode ,layout_file,0 type,txt_title,txt_words,'','',sign_time,sign_usercode,title2,title3,txt_style from v_textlib  where column_id={0} and column_date='{1}' union select t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type,t2.photo_title,0,t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','' from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode where t2.column_id={0} and t2.column_date='{1}' and islayout =1  )order by type", column_id, d)
        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("itemslist")
        Dim dr = DB.ExecuteReader(sql)
        Return API.WriteToXML(dr, "itemslist", "item")
    End Function

    Public Shared Function NewsroomNews(ByVal column_id As Integer) As String
        Dim sql = String.Format("(select distinct column_date ,'N' as style from paper_filelist where column_id={0} and column_date>='{1} 00:00:00' and islayout=1 union select distinct column_date ,'N' from paper_photolist where column_id={0} and column_date>='{1} 00:00:00' and islayout=1 union select distinct column_date ,style from finish_record where column_id={0} and column_date>='{1} 00:00:00') order by column_date desc", column_id.ToString, SpanOfDay(0))
        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("itemslist")
        Dim dr = DB.ExecuteReader(sql)
        'grouping column date
        Dim result As New SortedDictionary(Of String, String)

        While dr.Read
            Dim d As Date = dr.Item("column_date")
            Dim s_date = d.ToString("yyyy-MM-dd")
            If result.ContainsKey(s_date) Then
                If result(s_date).Equals("N") Then
                    result(s_date) = dr.Item("style")
                End If
            Else
                result(s_date) = dr.Item("style")
            End If
        End While

        Dim table As New Data.DataTable("item")
        table.Columns.Add(New Data.DataColumn("column_date"))
        table.Columns.Add(New Data.DataColumn("display_date"))
        table.Columns.Add(New Data.DataColumn("style"))

        For Each k In result.Keys
            Dim r = table.NewRow()
            r("column_date") = k
            r("display_date") = String.Format("{0}{1}{2}", k.Split("-"))
            r("style") = result(k)
            table.Rows.Add(r)
        Next

        Dim sw As New IO.StringWriter()
        sw.WriteLine("<?xml version=""1.0"" encoding=""utf-8"" ?>")
        table.WriteXml(sw)
        Return sw.ToString()
    End Function

    Public Shared Function NewsroomNews(ByVal column_id As Integer, ByVal column_date As String) As String
        Dim dt = DateTime.ParseExact(column_date, "M-d-yyyy", Nothing)

        Dim sql = String.Format("(select distinct column_date ,'N' as style from paper_filelist where column_id={0} and column_date='{1} 00:00:00' and islayout=1 union select distinct column_date ,'N' from paper_photolist where column_id={0} and column_date='{1} 00:00:00' and islayout=1 union select distinct column_date ,style from finish_record where column_id={0} and column_date='{1} 00:00:00') order by column_date desc", column_id.ToString, dt.ToString("M-d-yyyy"))
        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("itemslist")
        Dim dr = DB.ExecuteReader(sql)
        'grouping column date
        Dim result As New SortedDictionary(Of String, String)

        While dr.Read
            Dim d As Date = dr.Item("column_date")
            Dim s_date = d.ToString("yyyy-MM-dd")
            If result.ContainsKey(s_date) Then
                If result(s_date).Equals("N") Then
                    result(s_date) = dr.Item("style")
                End If
            Else
                result(s_date) = dr.Item("style")
            End If
        End While

        Dim table As New Data.DataTable("item")
        table.Columns.Add(New Data.DataColumn("column_date"))
        table.Columns.Add(New Data.DataColumn("display_date"))
        table.Columns.Add(New Data.DataColumn("style"))

        For Each k In result.Keys
            Dim r = table.NewRow()
            r("column_date") = k
            r("display_date") = String.Format("{0}{1}{2}", k.Split("-"))
            r("style") = result(k)
            table.Rows.Add(r)
        Next

        Dim sw As New IO.StringWriter()
        sw.WriteLine("<?xml version=""1.0"" encoding=""utf-8"" ?>")
        table.WriteXml(sw)
        Return sw.ToString()
    End Function

    Public Shared Function Newsroom() As XElement
        Dim sql = String.Format("SELECT entry,value from sys_newsroom")
        Dim new_t = New Data.DataTable("newsroom")
        Dim dt = DB.GetDataTable(sql)
        For Each r In dt.Rows
            Dim c As New Data.DataColumn(r("entry"))
            new_t.Columns.Add(c)
        Next

        Dim new_r = new_t.NewRow
        For Each r In dt.Rows
            new_r(r("entry")) = r("value")
        Next
        new_t.Rows.Add(new_r)

        Dim sw As New StringWriter()
        new_t.WriteXml(sw)
        Newsroom = XElement.Parse(sw.ToString)
        Return Newsroom.Element("newsroom")
    End Function

    Public Shared Function NewsroomNews_MP(ByVal column_id As Integer) As String
        Dim sql = String.Format("(select distinct column_date ,'N' as style ,period_id from paper_filelist where column_id={0} and column_date>='{1} 00:00:00' and islayout=1 union select distinct column_date ,'N' ,period_id from paper_photolist where column_id={0} and column_date>='{1} 00:00:00' and islayout=1 union select distinct column_date ,style ,period_no from finish_record t1 join news_column t2 on t1.column_id = t2.column_id left join non_paper_period_info t3 on t2.paper_code = t3.non_papername and t3.publication_date = t1.column_date where t1.column_id={0} and column_date>='{1} 00:00:00') order by column_date desc", column_id.ToString, SpanOfDay(0))
        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("itemslist")
        Dim dr = DB.ExecuteReader(sql)
        'grouping column date
        Dim result As New SortedDictionary(Of String, String)
        Dim period As New SortedDictionary(Of String, String)

        While dr.Read
            Dim d As Date = dr.Item("column_date")
            Dim s_date = d.ToString("yyyy-MM-dd")
            If result.ContainsKey(s_date) Then
                If result(s_date).Equals("N") Then
                    result(s_date) = dr.Item("style")
                    If (IsDBNull(dr.Item("period_id"))) Then
                        period(s_date) = ""
                    Else
                        period(s_date) = dr.Item("period_id")
                    End If
                End If

            Else
                result(s_date) = dr.Item("style")
                If (IsDBNull(dr.Item("period_id"))) Then
                    period(s_date) = ""
                Else
                    period(s_date) = dr.Item("period_id")
                End If
            End If
        End While

        Dim table As New Data.DataTable("item")
        table.Columns.Add(New Data.DataColumn("column_date"))
        table.Columns.Add(New Data.DataColumn("display_date"))
        table.Columns.Add(New Data.DataColumn("style"))
        table.Columns.Add(New Data.DataColumn("period_id"))

        For Each k In result.Keys
            Dim r = table.NewRow()
            r("column_date") = k
            r("display_date") = String.Format("{0}{1}{2}", k.Split("-"))
            r("style") = result(k)
            r("period_id") = period(k)
            table.Rows.Add(r)
        Next

        Dim sw As New IO.StringWriter()
        sw.WriteLine("<?xml version=""1.0"" encoding=""utf-8"" ?>")
        table.WriteXml(sw)
        Return sw.ToString()
    End Function

    Public Shared Function GetPeriod(ByVal paper_id As Integer) As String
        Dim sql = String.Format("  select distinct period_no,CONVERT(char(10), publication_date, 20) as publication_date from non_paper_period_info,news_paper where non_papername=paper_code and paper_id={0} and publication_date>='{1} 00:00:00' order by period_no", paper_id, API.SpanOfDay(0))
        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("itemslist")
        Dim dr = DB.ExecuteReader(sql)
        'grouping column date

        Dim table As New Data.DataTable("item")
        table.Columns.Add(New Data.DataColumn("period_no"))
        table.Columns.Add(New Data.DataColumn("publication_date"))

        While dr.Read
            Dim r = table.NewRow()
            r("period_no") = dr("period_no")
            r("publication_date") = dr("publication_date")
            table.Rows.Add(r)
        End While

        Dim sw As New IO.StringWriter()
        sw.WriteLine("<?xml version=""1.0"" encoding=""utf-8"" ?>")
        table.WriteXml(sw)
        Return sw.ToString()
    End Function

    Public Shared Function NewsroomTree(ByRef group_id As String) As String
        Dim ColSort As String
        ColSort = Newsroom().Element("ColumnSortMethod").Value
        Dim sql = String.Format("select distinct t1.paper_code ,t3.paper_id,column_code,layout_code,t1.column_id from news_column t1,column_permission t2,news_paper t3 where t1.column_id=t2.column_id and t1.paper_code=t3.paper_code and t2.group_id in ({0}) and perm_style='C' order by t1.paper_code,column_code", group_id)
        'Dim sql = String.Format("select t1.paper_code ,t3.paper_id,column_code,layout_code,t1.column_id from news_column t1,column_permission t2,news_paper t3 where t1.column_id=t2.column_id and t1.paper_code=t3.paper_code and t2.group_id={0} and perm_style='C' order by paper_code, ", group_id)
        'If (ColSort = "1") Then
        '    sql += "column_code"
        'Else
        '    sql += "layout_code"
        'End If

        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("newslist")
        Dim dr = DB.ExecuteReader(sql)
        Return API.WriteToXML(dr, "newslist", "news")
    End Function

    Public Shared Function NewsroomTree_MP(ByRef group_id As String) As String
        Dim ColSort As String
        ColSort = Newsroom().Element("ColumnSortMethod").Value
        Dim sql = String.Format("select distinct t1.paper_code ,t3.paper_id,column_code,layout_code,t1.column_id,is_newspaper from news_column t1,column_permission t2,news_paper t3 where t1.column_id=t2.column_id and t1.paper_code=t3.paper_code and t2.group_id in ({0}) and perm_style='C' order by t1.paper_code,column_code", group_id)
        'Dim sql = String.Format("select t1.paper_code ,t3.paper_id,column_code,layout_code,t1.column_id,is_newspaper from news_column t1,column_permission t2,news_paper t3 where t1.column_id=t2.column_id and t1.paper_code=t3.paper_code and t2.group_id={0} and perm_style='C' order by paper_code, ", group_id)
        'If (ColSort = "1") Then
        '    sql += "column_code"
        'Else
        '    sql += "layout_code"
        'End If

        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("newslist")
        Dim dr = DB.ExecuteReader(sql)
        Return API.WriteToXML(dr, "newslist", "news")
    End Function

    Public Shared Function Xml2String(ByRef xmlDoc As System.Xml.XmlDocument) As String
        Dim sw As New IO.StringWriter
        Dim xw As New XmlTextWriter(sw)
        xmlDoc.WriteTo(xw)
        Return xw.ToString()
    End Function

    Public Shared Function TableToXML(ByVal table As Data.DataTable, ByVal parentNode As String, ByVal rowNode As String) As String
        Dim xmlBuilder As New StringBuilder()
        Dim fieldCount As Integer = table.Columns.Count

        xmlBuilder.AppendLine("<?xml version=""1.0"" encoding=""utf-8"" ?>")
        xmlBuilder.AppendLine("<" + parentNode + ">")

        For Each r As Data.DataRow In table.Rows
            xmlBuilder.AppendLine(vbTab + "<" + rowNode + ">")

            For Each c As Data.DataColumn In table.Columns
                xmlBuilder.Append(vbTab + vbTab + "<" + c.Caption + ">")
                xmlBuilder.Append(Trim(r(c.Caption)))
                xmlBuilder.AppendLine("</" + c.Caption + ">")
            Next
            xmlBuilder.AppendLine(vbTab + "</" + rowNode + ">")
        Next
        xmlBuilder.AppendLine("</" + parentNode + ">")

        Return xmlBuilder.ToString
    End Function

    Public Shared Function WriteToXML(ByVal sqlReader As SqlDataReader, ByVal parentNode As String, ByVal rowNode As String) As String

        Dim xmlBuilder As New StringBuilder()
        Dim fieldCount As Integer = sqlReader.FieldCount - 1

        xmlBuilder.AppendLine("<?xml version=""1.0"" encoding=""utf-8"" ?>")
        xmlBuilder.AppendLine("<" + parentNode + ">")


        While sqlReader.Read()

            xmlBuilder.AppendLine(vbTab + "<" + rowNode + ">")

            For i = 0 To fieldCount
                Dim ColumnName As String = sqlReader.GetName(i).ToString()
                If String.IsNullOrEmpty(ColumnName) Then
                    Continue For
                End If
                xmlBuilder.Append(vbTab + vbTab + "<" + ColumnName + ">")
                xmlBuilder.Append(Trim(sqlReader.Item(i).ToString()))
                xmlBuilder.AppendLine("</" + ColumnName + ">")
            Next

            xmlBuilder.AppendLine(vbTab + "</" + rowNode + ">")
        End While

        xmlBuilder.AppendLine("</" + parentNode + ">")

        Return xmlBuilder.ToString

        'Dim sWriter As New StreamWriter(parentNode + ".xml")
        'sWriter.Write(xmlBuilder)
        'sWriter.Flush()
    End Function

    Public Shared Function FinishRecord(ByVal column_id As String) As String
        Dim sql = String.Format("select distinct column_date ,isnull(style,'N') from finish_record where column_id={0} and column_date>='{1} 00:00:00' and (style = 'P' or style ='Q') order by column_date desc", column_id, SpanOfDay(0))
        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("items")
        Dim dr = DB.ExecuteReader(sql)
        Return API.WriteToXML(dr, "items", "item")
    End Function

    Public Shared Function NewsColumn(ByVal column_id As String) As XElement
        Dim sql = String.Format("SELECT * from news_column where column_id={0}", column_id)
        Dim t = New Data.DataTable("news_column")
        Dim dt = DB.GetDataTable(sql)
        Dim sw As New StringWriter()
        dt.WriteXml(sw)
        Return XElement.Parse(sw.ToString).Element("item")
    End Function

    Public Shared Function NewsColumn_MP(ByVal column_id As String, ByVal column_date As String) As XElement
        Dim sql = String.Format("SELECT * from news_column t1, news_paper t2 left join non_paper_period_info on non_papername=paper_code and publication_date='{1} 00:00:00' where t1.paper_code=t2.paper_code and column_id = {0}", column_id, column_date)
        Dim t = New Data.DataTable("news_column")
        Dim dt = DB.GetDataTable(sql)
        Dim sw As New StringWriter()
        dt.WriteXml(sw)
        Return XElement.Parse(sw.ToString).Element("item")
    End Function
    Public Shared Function SpanOfDay(ByVal day As Integer) As String
        Dim sql = String.Format("SELECT value from sys_newsroom where entry = 'workdaychangetime'")
        Dim dr = DB.ExecuteReader(sql)
        dr.Read()
        Dim spanTime = dr("value")
        dr.Close()
        Dim dt = DateTime.ParseExact(spanTime, "HH:mm:ss", Nothing)
        Dim test As String
        If dt > Now Then
            test = dt.AddDays(-1).AddDays(day).ToString("M-d-yyyy")
        Else
            test = Now.AddDays(day).ToString("M-d-yyyy")
        End If
        Return test
    End Function

    Public Shared Function ComputerName() As String
        Dim sql = String.Format("select  host_name() name")

        Dim xml As New System.Xml.XmlDocument
        Dim root As System.Xml.XmlElement = xml.CreateElement("computer")
        xml.AppendChild(root)

        Dim dr = DB.ExecuteReader(sql)

        Return API.WriteToXML(dr, "computer", "name")

    End Function
    'Pat news_detail_all crash server test solution=========================================
    Public Shared Function NewsColumn_keep_open(ByVal column_id As String) As XElement
        Dim sql = String.Format("SELECT * from news_column where column_id={0}", column_id)
        Dim t = New Data.DataTable("news_column")
        Dim dt = DB.GetDataTable_keep_open(sql)
        Dim sw As New StringWriter()
        dt.WriteXml(sw)
        Return XElement.Parse(sw.ToString).Element("item")
    End Function

    Public Shared Function NewsColumn_MP_keep_open(ByVal column_id As String, ByVal column_date As String) As XElement
        Dim sql = String.Format("SELECT * from news_column t1, news_paper t2 left join non_paper_period_info on non_papername=paper_code and publication_date='{1} 00:00:00' where t1.paper_code=t2.paper_code and column_id = {0}", column_id, column_date)
        Dim t = New Data.DataTable("news_column")
        Dim dt = DB.GetDataTable_keep_open(sql)
        Dim sw As New StringWriter()
        dt.WriteXml(sw)
        Return XElement.Parse(sw.ToString).Element("item")
    End Function
    Public Shared Function Newsroom_keep_open() As XElement
        Dim sql = String.Format("SELECT entry,value from sys_newsroom")
        Dim new_t = New Data.DataTable("newsroom")
        Dim dt = DB.GetDataTable_keep_open(sql)
        For Each r In dt.Rows
            Dim c As New Data.DataColumn(r("entry"))
            new_t.Columns.Add(c)
        Next

        Dim new_r = new_t.NewRow
        For Each r In dt.Rows
            new_r(r("entry")) = r("value")
        Next
        new_t.Rows.Add(new_r)

        Dim sw As New StringWriter()
        new_t.WriteXml(sw)
        Newsroom_keep_open = XElement.Parse(sw.ToString)
        Return Newsroom_keep_open.Element("newsroom")
    End Function
    '=======================================================================================
End Class
