Imports System.IO

Partial Class newsroom_tree
    Inherits System.Web.UI.Page

    Protected Sub Page_LoadComplete(sender As Object, e As System.EventArgs) Handles Me.LoadComplete
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)
        Session("groupid") = Request("group_id")
        If (Request("company") = "MP") Then
            Response.Write(API.NewsroomTree_MP(Request("group_id")))
        ElseIf String.IsNullOrEmpty(Request("type")) Then
            Response.Write(API.NewsroomTree(Request("group_id")))
        Else
            Dim lib_id As String = Request("lib_id")
            Dim paper_id As String = Request("paper_id")
            Dim sql As String
            If String.IsNullOrEmpty(lib_id) And String.IsNullOrEmpty(paper_id) Then
                sql = String.Format("select distinct t1.paper_code, t3.paper_id, t3.typeset_name from news_column t1,column_permission t2,news_paper t3 where t1.column_id=t2.column_id and t1.paper_code=t3.paper_code " _
                            & " and t2.group_id in ({0}) and perm_style='C' order by t1.paper_code", Request("group_id"))
            ElseIf String.IsNullOrEmpty(lib_id) Then
                sql = String.Format("select distinct t4.lib_id,t4.lib_name from news_column t1,column_permission t2,news_paper t3,news_lib t4 where t1.column_id=t2.column_id and t1.paper_code=t3.paper_code and t1.lib_id = t4.lib_id " _
                            & " and t2.group_id in ({0}) and paper_id={1} and perm_style='C' order by t4.lib_id", Request("group_id"), paper_id)
            Else
                sql = String.Format("select column_code,layout_code,t1.column_id from news_column t1,column_permission t2 where t1.column_id=t2.column_id " _
                            & " and t2.group_id in ({0}) and lib_id={1} and paper_code = N'{2}' and perm_style='C' order by ", Request("group_id"), lib_id, Request("paper_code"))
                Dim ColSort As String
                ColSort = API.Newsroom().Element("ColumnSortMethod").Value
                If (ColSort = "1") Then
                    sql += "column_code"
                Else
                    sql += "layout_code"
                End If
            End If
            Dim dt = DB.GetDataTable(sql)
            Dim sw As New StringWriter()
            dt.WriteXml(sw)
            Dim xd As New System.Xml.XmlDocument()
            xd.LoadXml(sw.ToString)
            Response.Write(xd.InnerXml.ToString)
            Response.End()
        End If

        DB.Close()
        Response.End()
    End Sub
End Class
