
Partial Class create_template
    Inherits System.Web.UI.Page

    Protected Sub form1_Load(sender As Object, e As System.EventArgs) Handles form1.Load
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
        com.CommandType = Data.CommandType.StoredProcedure
        Dim company As String = Request("company")
        If (company = "MP") Then
            com.CommandText = String.Format("InsertFinishRecordID")
        Else
            com.CommandText = String.Format("InsertFinishRecord")
        End If


        With com.Parameters.Add("@column_id", Data.SqlDbType.Int)
            .Value = Request("column_id")
            .Direction = Data.ParameterDirection.Input
        End With

        With com.Parameters.Add("@column_date", Data.SqlDbType.NChar)
            .Value = Request("column_date")
            .Direction = Data.ParameterDirection.Input
        End With

        With com.Parameters.Add("@compose_file", Data.SqlDbType.NChar)
            .Value = Request("compose_file")
            .Direction = Data.ParameterDirection.Input
        End With

        With com.Parameters.Add("@type", Data.SqlDbType.Int)
            .Value = Request("type")
            .Direction = Data.ParameterDirection.Input
        End With

        If (company = "MP") Then
            Dim computername As String = Request("computer")
            'Try
            '    If computername Is Nothing Then
            '        computername = Net.Dns.GetHostEntry(Context.Request.UserHostAddress).HostName.Split(".")(0).ToString()
            '    ElseIf computername.Length = 0 Then
            '        computername = Net.Dns.GetHostEntry(Context.Request.UserHostAddress).HostName.Split(".")(0).ToString()
            '    End If
            'Catch ex As Exception
            '    computername = Context.Request.UserHostAddress
            'End Try
            If computername Is Nothing Then
                computername = Context.Request.UserHostAddress
            ElseIf computername.Length = 0 Then
                computername = Context.Request.UserHostAddress
            End If

            With com.Parameters.Add("@hostname", Data.SqlDbType.NChar)
                .Value = computername
                .Direction = Data.ParameterDirection.Input
            End With

            With com.Parameters.Add("@usercode", Data.SqlDbType.NChar)
                .Value = Request("username")
                .Direction = Data.ParameterDirection.Input
            End With
        End If

        With com.Parameters.Add("@out", Data.SqlDbType.SmallInt)
            .Value = 0
            .Direction = Data.ParameterDirection.InputOutput
        End With
        com.ExecuteNonQuery()
        Response.Write(com.Parameters("@out").Value().ToString)
        'Pat news_detail_all crash server test solution=========================================
        'DB.Close()
        '=======================================================================================
        Response.End()
    End Sub
End Class
