
Partial Class finish_record
    Inherits System.Web.UI.Page

    Protected Sub form1_Load(sender As Object, e As System.EventArgs) Handles form1.Load
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)

        Dim sql As String
        If (Request("company") = "MP") Then
            sql = String.Format("select distinct column_date,isnull(style,'N') as style,period_no from finish_record t1 join news_column t2 on t1.column_id = t2.column_id left join non_paper_period_info t3 on t2.paper_code = t3.non_papername and t3.publication_date = t1.column_date where t1.column_id={0} and column_date>='{1} 00:00:00' and (style = 'P' or style ='Q') order by column_date desc", Request("column_id"), API.SpanOfDay(0))
        Else
            sql = String.Format("select distinct column_date,isnull(style,'N') as style from finish_record where column_id={0} and column_date>='{1} 00:00:00' and (style = 'P' or style ='Q') order by column_date desc", Request("column_id"), API.SpanOfDay(0))
        End If
        Dim dr = DB.ExecuteReader(sql)

        Dim dt As New Data.DataTable("item")
        dt.Columns.Add(New Data.DataColumn("column_date"))
        dt.Columns.Add(New Data.DataColumn("display_date"))
        dt.Columns.Add(New Data.DataColumn("year"))
        dt.Columns.Add(New Data.DataColumn("month"))
        dt.Columns.Add(New Data.DataColumn("day"))
        dt.Columns.Add(New Data.DataColumn("style"))
        If (Request("company") = "MP") Then
            dt.Columns.Add(New Data.DataColumn("period_no"))
        End If

        While dr.Read
            Dim r = dt.NewRow()
            Dim s As Date = dr("column_date")

            r("column_date") = s.ToString("yyyy-MM-dd")
            r("display_date") = s.ToString("yyyyMMdd")
            r("style") = dr("style")
            r("year") = s.ToString("yyyy")
            r("month") = s.ToString("MM")
            r("day") = s.ToString("dd")
            If (Request("company") = "MP") Then
                r("period_no") = dr("period_no")
            End If
            dt.Rows.Add(r)
        End While

        dt.WriteXml(Response.OutputStream)
        DB.Close()

        Response.End()

    End Sub
End Class
