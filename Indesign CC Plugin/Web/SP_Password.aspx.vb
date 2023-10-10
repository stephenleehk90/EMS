
Partial Class SP_Password
    Inherits System.Web.UI.Page

    Protected Sub Page_Load(sender As Object, e As System.EventArgs) Handles Me.Load
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        'Pat news_detail_all crash server test solution=========================================
        'Check login
        If Not DB.ChkLogin(username, password) Then
            Response.End()
        End If

        'Use same connection
        If Not (DB.Chk_Connection_keep_open) Then 'Use old connection
            If Not DB.Init_keep_open(username, password) Then
                Response.End()
            End If
        End If

        'DB.Init(username, password)
        'Dim com = DB.conn.CreateCommand
        Dim com = DB.conn_keep_open.CreateCommand
        '=======================================================================================

        Dim new_password As String = Newsroom.Password.SecretEn64Code(Request("new_password"))

        com.CommandType = Data.CommandType.StoredProcedure
        com.CommandText = String.Format("sp_password")

        com.Parameters.Add("@old", Data.SqlDbType.NChar).Value = password
        com.Parameters.Add("@new", Data.SqlDbType.NChar).Value = new_password
        com.Parameters.Add("@loginame", Data.SqlDbType.NChar).Value = username

        Dim result = com.ExecuteNonQuery()
        Response.Write(result.ToString)
        'Pat news_detail_all crash server test solution=========================================
        'DB.Close()
        '=======================================================================================
        Response.End()
    End Sub
End Class
