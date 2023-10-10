
Partial Class news_attach
    Inherits System.Web.UI.Page

    Protected Sub Page_LoadComplete(sender As Object, e As System.EventArgs) Handles Me.LoadComplete
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)
        Response.Write(API.NewsAttach(Request("filecode")))
        Response.End()
        DB.Close()
    End Sub
End Class
