Public Class MyDate

    Public Shared Function Format(d As Date, f As String) As String
        Dim dd As String = d.ToString("dd")
        Dim mm As String = d.ToString("MM")
        Dim yyyy As String = d.ToString("yyyy")
        Select Case f
            Case "0NONE"
                Return ""
            Case "1MM$DD"
                Return mm & "$" & dd
            Case "2MMDDYYYY"
                Return mm & dd & yyyy
            Case "3DDMMYYYY"
                Return dd & mm & yyyy
            Case "4YYYYMMDD"
                Return yyyy & mm & dd
            Case "5DD"
                Return dd
        End Select
        Return ""
    End Function

End Class
