Partial Class decode_password
    Inherits System.Web.UI.Page

    Protected Sub Page_Load(sender As Object, e As System.EventArgs) Handles Me.Load
        'Response.Write(API.SecretEn64Code(Request("password")))
        'Dim n As New Newsroom.Password

        Response.Write(Newsroom.Password.SecretEn64Code(Request("password")))
        Response.End()
        'Response.Write(Newsroom.Password.SecretEn64Code("123"))
    End Sub
End Class
