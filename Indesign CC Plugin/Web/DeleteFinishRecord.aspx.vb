Partial Class DeleteFinishRecord
    Inherits System.Web.UI.Page

    Protected Sub Page_Load(sender As Object, e As System.EventArgs) Handles Me.Load

        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))

        DB.Init(username, password)
        Dim sql = String.Format("delete finish_record where column_id={0} and column_date='{1} 00:00:00'", Request("column_id"), Request("column_date"))
        Dim dt = DB.ExecuteNonQuery(sql)
        DB.Close()
    End Sub
End Class
