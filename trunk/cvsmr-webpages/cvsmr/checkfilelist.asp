<html>

<head>
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>新建网页 1</title>
</head>
<%
set cvsconn=session("connection")
set cvsrs=Server.CreateObject("adodb.recordset")
set cvsrs=cvsconn.execute("select * from [mrv2].[dbo].[FILELIST]")
%>
<body>

<div align="left">
  <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#C0C0C0" width="525" height="23" id="AutoNumber1">
    <tr>
      <td width="502" height="23">id</td>
      <td width="502" height="23">filename</td>
      <td width="473" height="23">pathname</td>
      <td width="694" height="23">mrid</td>
      <td width="362" height="23">comment</td>
      <td width="502" height="23">lmtime</td>
    </tr>
  <%while not cvsrs.eof%>
    <tr>
      <td width="502" height="23"><%=cvsrs("id")%></td>
      <td width="502" height="23"><%=cvsrs("filename")%></td>
      <td width="473" height="23"><%=cvsrs("pathname")%></td>
      <td width="694" height="23"><%=cvsrs("mrid")%></td>
      <td width="362" height="23"><%=cvsrs("comment")%></td>
      <td width="502" height="23"><%=cvsrs("lmtime")%></td>
    </tr>
    <%cvsrs.movenext 
    wend%>
  </table>
</div>

</body>

</html>