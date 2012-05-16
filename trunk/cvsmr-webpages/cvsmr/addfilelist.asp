<!-- 这里的功能只包含两部分

if (表filelist里已经存在相同的记录)
{ 
	不作任何事情;
}else{
	插入数据库filelist表中;
}

说明:
	 本来想把检查mr里面是否存在有效的处于assign状态的mrid的逻辑
	也放在这里.不过后来又放弃这个想法了.
	因为在wincvs客户端提交的时候, 有可能判断是否存在有效的mrid和
	插入数据库filelist表记录并不是在同一个时间点.
	所以暂时是分别写在两个asp页面里面.
	
	这里的代码没有返回值返回给http客户端.我大概想了一下
	觉得这个返回值没有什么意义.
	将来可能的处理返回值的逻辑是:
	if (插入数据库正常, 没有致命错误, 即使数据库里面已经有相同记录, 但只要没出错)
	{
		返回 "Y";
	}else{
		返回 "N";
	}
	
 // -->


<%
Set CONNn = session("connection")
SET RSss = server.CreateObject("adodb.recordset")

REV=TRIM(REQUEST.QUERYSTRING("REV"))
TAG=TRIM(REQUEST.QUERYSTRING("TAG"))
ID=REQUEST.QUERYSTRING("ID")
FILENAME=REQUEST.QUERYSTRING("FILENAME")
PATHNAME=REQUEST.QUERYSTRING("PATHNAME")
MRID=REQUEST.QUERYSTRING("MRID")
COMMENT=REQUEST.QUERYSTRING("COMMENT")
LMTIME=REQUEST.QUERYSTRING("LMTIME")
RESERVED1=REQUEST.QUERYSTRING("RESERVED1")
RESERVED2=REQUEST.QUERYSTRING("RESERVED2")
RESERVED3=REQUEST.QUERYSTRING("RESERVED3")
USERNAME=request.querystring("USERNAME")
nowtime= date + time
LMTIME=cstr(nowtime)

for each item in request.querystring("FILENAME")
	PATHNAME=TRIM(REQUEST.QUERYSTRING("PATHNAME"))+"/"+TRIM(item)
	CVS_QUERYSTRING="INSERT INTO [EMR].[dbo].[FILELIST] VALUES(1, '"+ item +"', '"+PATHNAME+"','"+MRID+"', '" + COMMENT+"', '"+LMTIME+"','"+REV+"','"+TAG+"','')"
	set RSss=session("connection").EXECUTE(CVS_QUERYSTRING)
	response.write("Y")
next 

%>