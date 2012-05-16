<%

Set CONNn = session("connection")
SET RSss = server.CreateObject("adodb.recordset")

LOGINNAME=Request.QueryString("LOGINNAME")
MRID=REQUEST.QUERYSTRING("MRID")
querystr = "select * from [EMR].[dbo].[mr],[EMR].[dbo].[USER] where ([EMR].[dbo].[MR].Name='" + MRID + "') and ([EMR].[dbo].[MR].Status=1) and ([EMR].[dbo].[MR].ownerid=[EMR].[dbo].[USER].ID) AND ([EMR].[dbo].[USER].LOGINNAME='"+ LOGINNAME +"')"
' response.write(querystr)
SET RSSS = CONNN.EXECUTE(querystr)


if RSSS.EOF then
	response.write("N")
else
	RESPONSE.WRITE("Y")
end if


%>