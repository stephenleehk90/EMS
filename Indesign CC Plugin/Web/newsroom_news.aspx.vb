
Partial Class newsroom_news
    Inherits System.Web.UI.Page

    Protected Sub Page_Load(sender As Object, e As System.EventArgs) Handles Me.Load
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)
        If (Request("company") = "MP" And Not String.IsNullOrEmpty(Request("paper_id"))) Then
            Response.Write(API.GetPeriod(Integer.Parse(Request("paper_id"))))
        ElseIf (Request("company") = "MP") Then
            Response.Write(API.NewsroomNews_MP(Integer.Parse(Request("column_id"))))
        ElseIf String.IsNullOrEmpty(Request("column_date")) Then
            Response.Write(API.NewsroomNews(Integer.Parse(Request("column_id"))))
        Else
            Response.Write(API.NewsroomNews(Integer.Parse(Request("column_id")), Request("column_date")))
        End If
        Response.End()
        DB.Close()
    End Sub
End Class
