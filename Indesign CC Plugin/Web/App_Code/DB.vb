Imports Microsoft.VisualBasic
Imports System.Data.SqlClient

Public Class DB
    Public Shared connection_str As String
    Public Shared conn As SqlConnection

    Public Shared Sub Close()
        conn.Close()
    End Sub

    Public Shared Function Init(ByVal username As String, ByVal password As String) As Boolean
        connection_str = String.Format(System.Configuration.ConfigurationManager.AppSettings("DBConnectionString"), username, password)
        conn = New System.Data.SqlClient.SqlConnection(connection_str)
        Try
            conn.Open()
            Return True
        Catch ex As Exception
            Dim test As Integer = 1

        End Try
        Return False
    End Function

    Public Shared Function Connection() As SqlConnection
        If conn.State = Data.ConnectionState.Closed Then
            conn.Open()
        End If
        Return conn
    End Function

    Public Shared Function ExecuteXmlReader(ByRef sql As String) As System.Xml.XmlReader
        Dim comm As New SqlCommand(sql, Connection)
        Return comm.ExecuteXmlReader()
    End Function

    Public Shared Function ExecuteReader(ByRef sql As String) As SqlDataReader
        Dim comm As New SqlCommand(sql, Connection)
        Return comm.ExecuteReader()
    End Function

    Public Shared Function ExecuteNonQuery(ByRef sql As String) As Integer
        Dim comm As New SqlCommand(sql, Connection)
        Return comm.ExecuteNonQuery()
    End Function

    Public Shared Function Execute(ByRef sql As String) As Object
        Dim comm As New SqlCommand(sql, Connection)
        Return comm.ExecuteScalar(sql)
    End Function

    Public Shared Function GetDataTable(ByRef sql As String) As Data.DataTable
        Dim dt = New Data.DataTable("item")
        Dim da As New Data.SqlClient.SqlDataAdapter(sql, Connection())
        da.Fill(dt)
        For Each r In dt.Rows
            For Each c In dt.Columns
                If Not TypeOf r(c) Is DBNull Then
                    r(c) = Trim(r(c))
                End If
            Next
        Next
        Return dt
    End Function

    'Pat news_detail_all crash server test solution=========================================
    Public Shared connection_str_keep_open As String
    Public Shared conn_keep_open As SqlConnection
    Public Shared last_username As String
    Public Shared last_password As String
    Public Shared Function Init_keep_open(ByVal username As String, ByVal password As String) As Boolean
        Dim conn_keep_open_temp As SqlConnection
        connection_str_keep_open = String.Format(System.Configuration.ConfigurationManager.AppSettings("DBConnectionString"), username, password)
        conn_keep_open_temp = New System.Data.SqlClient.SqlConnection(connection_str_keep_open)
        last_username = username
        last_password = password
        Try
            conn_keep_open_temp.Open()
            conn_keep_open = conn_keep_open_temp
            Return True
        Catch ex As Exception
            Dim test As Integer = 1

        End Try
        Return False
    End Function

    Public Shared Function Connection_keep_open() As SqlConnection
        If Not conn_keep_open.State = Data.ConnectionState.Open Then
            Init_keep_open(last_username, last_password)
        End If
        Return conn_keep_open
    End Function
    Public Shared Function Chk_Connection_keep_open() As Boolean

        If conn_keep_open Is Nothing Then
            Return False
        End If

        If conn_keep_open.State = Data.ConnectionState.Open Then
            Return True
        Else
            Return False
        End If
    End Function
    Public Shared Function GetDataTable_keep_open(ByRef sql As String) As Data.DataTable
        Dim dt = New Data.DataTable("item")
        Dim da As New Data.SqlClient.SqlDataAdapter(sql, Connection_keep_open())
        da.Fill(dt)
        For Each r In dt.Rows
            For Each c In dt.Columns
                If Not TypeOf r(c) Is DBNull Then
                    r(c) = Trim(r(c))
                End If
            Next
        Next
        Return dt
    End Function
    Public Shared Function ChkLogin(ByVal username As String, ByVal password As String) As Boolean
        Dim conn_keep_open_temp As SqlConnection
        connection_str_keep_open = String.Format(System.Configuration.ConfigurationManager.AppSettings("DBConnectionString"), username, password)
        conn_keep_open_temp = New System.Data.SqlClient.SqlConnection(connection_str_keep_open)
        Try
            conn_keep_open_temp.Open()
            conn_keep_open_temp.Close()
            Return True
        Catch ex As Exception

        End Try
        Return False
    End Function
    '=======================================================================================



End Class
