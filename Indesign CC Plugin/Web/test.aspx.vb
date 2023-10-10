Imports System.Data.SqlClient
Imports System.IO
Imports Newsroom

Partial Class test
    Inherits System.Web.UI.Page

    Protected Sub form1_Load(sender As Object, e As System.EventArgs) Handles form1.Load
        Response.Write(Newsroom.Password.SecretEn64Code("123456"))


        'Dim password As String = "123"
        '        DB.Init(username, password)

        'Response.Write(API.UserGroup("winson"))
        Response.End()
    End Sub

    Public Function WriteToXML(ByVal sqlReader As SqlDataReader, ByVal parentNode As String, ByVal rowNode As String) As String

        Dim xmlBuilder As New StringBuilder()
        Dim fieldCount As Integer = sqlReader.FieldCount - 1

        xmlBuilder.AppendLine("<?xml version=""1.0"" encoding=""utf-8"" ?>")
        xmlBuilder.AppendLine("<" + parentNode + ">")


        While sqlReader.Read()

            xmlBuilder.AppendLine(vbTab + "<" + rowNode + ">")

            For i = 0 To fieldCount
                Dim ColumnName As String = sqlReader.GetName(i).ToString()
                xmlBuilder.Append(vbTab + vbTab + "<" + ColumnName + ">")
                xmlBuilder.Append(sqlReader.Item(i).ToString())
                xmlBuilder.AppendLine("</" + ColumnName + ">")
            Next

            xmlBuilder.AppendLine(vbTab + "</" + rowNode + ">")
        End While

        xmlBuilder.AppendLine("</" + parentNode + ">")

        Return xmlBuilder.ToString

        'Dim sWriter As New StreamWriter(parentNode + ".xml")
        'sWriter.Write(xmlBuilder)
        'sWriter.Flush()
    End Function
End Class
