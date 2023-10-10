
Partial Class news_detail
    Inherits System.Web.UI.Page

    Protected Sub Page_LoadComplete(sender As Object, e As System.EventArgs) Handles Me.LoadComplete
        Dim username As String = Request("username")
        Dim password As String = Newsroom.Password.SecretEn64Code(Request("password"))
        DB.Init(username, password)

        Dim sql = String.Format("(select  distinct group_id, filecode ,layout_file,0 type,txt_title,txt_words,'','',sign_time,sign_usercode,title2,title3,txt_style from v_textlib  where column_id={0} and column_date='{1}' union select t3.typeset_group_id,  cast (t2.photo_id as varchar(30)),layoutfile,1 type,t2.photo_title,0,t3.filecode,photo_note,sign_time,dbo.f_GetNewsUserName(sign_usercode) sign_usercode,  '','','' from paper_photolist t2  left outer join file_photo_relation t3 on cast (t2.photo_id as varchar(30))=t3.photocode where t2.column_id={0} and t2.column_date='{1}' and islayout =1  )order by type", Request("column_id"), Request("date"))
        Dim dt = DB.GetDataTable(sql)
        dt.WriteXml(Response.OutputStream)
        DB.Close()
        Response.End()
    End Sub
End Class
