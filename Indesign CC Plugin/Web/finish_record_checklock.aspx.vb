
Partial Class finish_record_checklock
    Inherits System.Web.UI.Page

    Protected Sub Page_Load(sender As Object, e As System.EventArgs) Handles Me.Load
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)

        Dim column_id As String = Request("column_id")
        Dim column_date As String = Request("column_date")

        Dim dt = DB.GetDataTable(String.Format("select distinct style,usercode,usermachinename,compose_file from finish_record where column_id={0} and column_date='{1}'", column_id, column_date))
        For Each r In dt.Rows
            Dim compose_file As String = r("compose_file")
            'remame to indd
            Dim f = My.Computer.FileSystem.GetFileInfo(compose_file)
            r("compose_file") = f.Name().Substring(0, f.Name().Length() - f.Extension.Length()) & ".indd"
        Next

        dt.WriteXml(Response.OutputStream)

        Response.End()
    End Sub
End Class
