Partial Class role
    Inherits System.Web.UI.Page

    Protected Sub Page_Load(sender As Object, e As System.EventArgs) Handles Me.Load

        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)

        Response.Write(API.UserGroup(username))
        DB.Close()
        Response.End()
    End Sub
End Class
