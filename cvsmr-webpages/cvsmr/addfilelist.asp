<!-- ����Ĺ���ֻ����������

if (��filelist���Ѿ�������ͬ�ļ�¼)
{ 
	�����κ�����;
}else{
	�������ݿ�filelist����;
}

˵��:
	 ������Ѽ��mr�����Ƿ������Ч�Ĵ���assign״̬��mrid���߼�
	Ҳ��������.���������ַ�������뷨��.
	��Ϊ��wincvs�ͻ����ύ��ʱ��, �п����ж��Ƿ������Ч��mrid��
	�������ݿ�filelist���¼��������ͬһ��ʱ���.
	������ʱ�Ƿֱ�д������aspҳ������.
	
	����Ĵ���û�з���ֵ���ظ�http�ͻ���.�Ҵ������һ��
	�����������ֵû��ʲô����.
	�������ܵĴ�����ֵ���߼���:
	if (�������ݿ�����, û����������, ��ʹ���ݿ������Ѿ�����ͬ��¼, ��ֻҪû����)
	{
		���� "Y";
	}else{
		���� "N";
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