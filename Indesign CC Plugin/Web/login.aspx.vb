Imports System.Data.SqlClient
Imports System.Runtime.InteropServices
Imports System.Net

Partial Class login
    Inherits System.Web.UI.Page

    Enum LoadLibraryFlags As UInteger
        DONT_RESOLVE_DLL_REFERENCES = &H1
        LOAD_IGNORE_CODE_AUTHZ_LEVEL = &H10
        LOAD_LIBRARY_AS_DATAFILE = &H2
        LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE = &H40
        LOAD_LIBRARY_AS_IMAGE_RESOURCE = &H20
        LOAD_WITH_ALTERED_SEARCH_PATH = &H8
    End Enum

    Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal dllToLoad As String) As IntPtr
    Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As IntPtr, ByVal procedureName As String) As IntPtr
    Declare Function FreeLibrary Lib "kernel32" (ByVal hModule As IntPtr) As Boolean
    <DllImport("kernel32.dll")> _
    Private Shared Function LoadLibraryEx(lpFileName As String, hReservedNull As IntPtr, dwFlags As LoadLibraryFlags) As IntPtr
    End Function

    '<DllImport("Dongle.dll",
    'CallingConvention:=CallingConvention.StdCall)> _
    'Private Shared Function GetLicense(ByVal strServerDate As String, ByVal nModule As UShort, ByRef iYear As Integer, ByRef iMonth As Integer, ByRef iDay As Integer) As Integer

    'End Function

    Delegate Function GetLicense(ByVal strServerDate As String, ByVal nModule As UShort, ByRef iYear As Integer, ByRef iMonth As Integer, ByRef iDay As Integer) As Integer

    Protected Sub form1_Load(sender As Object, e As System.EventArgs) Handles form1.Load
        If String.IsNullOrEmpty(Request("username")) Or String.IsNullOrEmpty(Request("password")) Then
            Response.Write("0")
            Response.End()
        End If


        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        If DB.Init(username, password) Then
            Dim sql = "IF EXISTS (SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'news_user' AND COLUMN_NAME = 'isComposeStaff') SELECT 1 hasIsComposeStaff ELSE SELECT 0 hasIsComposeStaff"
            Dim dr = DB.ExecuteReader(sql)
            dr.Read()
            Dim hasIsComposeStaff = dr("hasIsComposeStaff")
            dr.Close()

            If (hasIsComposeStaff.ToString = "1") Then
                sql = String.Format("select CONVERT(char(10), GETDATE(), 20) date")
                dr = DB.ExecuteReader(sql)
                dr.Read()
                Dim ServerDate As String = dr("date")
                dr.Close()

                Dim strPath As String = Server.MapPath("~/bin/Dongle.dll")
                'Dim pDll As IntPtr = LoadLibrary(strPath)
                Dim pDll As IntPtr = LoadLibraryEx(strPath, IntPtr.Zero, LoadLibraryFlags.LOAD_WITH_ALTERED_SEARCH_PATH)
                If pDll = 0 Then
                    pDll = LoadLibraryEx(strPath, IntPtr.Zero, LoadLibraryFlags.LOAD_LIBRARY_AS_DATAFILE)
                    If pDll = 0 Then
                        pDll = LoadLibraryEx(strPath, IntPtr.Zero, LoadLibraryFlags.DONT_RESOLVE_DLL_REFERENCES)
                        If pDll = 0 Then
                            Response.Write(strPath)
                            Response.End()
                        End If
                    End If
                End If
                Dim pAddressOfFunctionToCall As IntPtr = GetProcAddress(pDll, "GetLicense")
                Dim getLicense As GetLicense = DirectCast(Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, GetType(GetLicense)), GetLicense)

                Dim iYear As Integer
                Dim iMonth As Integer
                Dim iDay As Integer
                Dim iGetLicenseCode As Integer = getLicense(ServerDate, 8, iYear, iMonth, iDay)
                Dim test As Boolean = FreeLibrary(pAddressOfFunctionToCall)
                'Dim iGetLicenseCode As Integer = GetLicense(ServerDate, 8, iYear, iMonth, iDay)
                If (iGetLicenseCode <> 0) Then
                    If (iGetLicenseCode = 2) Then
                        Response.Write("查找加密鎖失敗，請確認加密鎖安裝設置正確 !")
                    ElseIf (iGetLicenseCode = 4) Then
                        Response.Write("加密鎖數據不正確，請檢查加密鎖的數據！")
                    ElseIf (iGetLicenseCode = 5) Then
                        Response.Write(String.Format("加密鎖已過期 (過期日: {0}/{1:00}/{2:00}) ！", iYear, iMonth, iDay))
                    End If
                    Response.End()
                End If

                sql = String.Format("select isComposeStaff from news_user where usercode='{0}'", username)
                dr = DB.ExecuteReader(sql)
                dr.Read()
                Dim isComposeStaff = dr("isComposeStaff")
                dr.Close()

                If (isComposeStaff.ToString <> "1") Then
                    Response.Write("登入用戶不是排版員")
                    Response.End()
                End If
            End If

            Dim ip As String
            ip = Context.Request.ServerVariables("HTTP_X_FORWARDED_FOR")
            If ip = String.Empty Then
                ip = Context.Request.ServerVariables("REMOTE_ADDR")
            End If
            Response.Write(1)
            'Dim ipInfo As Net.IPHostEntry = Net.Dns.GetHostEntry(Context.Request.UserHostAddress)
            'Response.Write("<br>" + ipInfo.HostName.Split(".")(0).ToString())
        Else
            Response.Write("0")
        End If
        'Response.Write(password)
        'Response.Write(password)
        Response.End()
        DB.Close()
    End Sub
End Class
