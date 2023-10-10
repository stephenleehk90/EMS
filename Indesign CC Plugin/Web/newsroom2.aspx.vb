
Partial Class newsroom2
    Inherits System.Web.UI.Page

    Protected Sub Page_Load(sender As Object, e As System.EventArgs) Handles Me.Load
        If String.IsNullOrEmpty(Request("username")) Or String.IsNullOrEmpty(Request("password")) Then
            Exit Sub
        End If

        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))

        DB.Init(username, password)

        Response.Write(API.Newsroom().ToString)
        Response.End()

    End Sub
End Class
