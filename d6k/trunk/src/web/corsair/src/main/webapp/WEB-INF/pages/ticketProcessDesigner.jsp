<%@ page contentType="text/html;charset=UTF-8"%>
<%@ include file="/common/taglibs.jsp"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="en">
	<head>
		<title>业务流程图</title>
		<%@ include file="/common/meta.jsp"%>

		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">

		<link href="<c:url value="/resources/css/snaker/style.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/snaker/snaker.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

		<script src="<c:url value="/resources/js/snaker/raphael-min.js"/>"></script>
		<%--<script src="<c:url value="/resources/js/snaker/jquery-ui-1.8.4.custom/js/jquery.min.js"/>"></script>--%>
		<script src="<c:url value="/resources/js/snaker/jquery-ui-1.8.4.custom/js/jquery-ui.min.js"/>"></script>
		<script src="<c:url value="/resources/js/snaker/snaker/snaker.designer.js"/>"></script>
		<script src="<c:url value="/resources/js/snaker/snaker/snaker.model.js"/>"></script>
		<script src="<c:url value="/resources/js/snaker/snaker/snaker.editors.js"/>"></script>

		<script type="text/javascript">
			$(function() {
				var json="${process }";
				var model;
				if(json) {
					//json.replace(new RegExp("@@","gm"), "\"")
					model=eval("(" + json + ")");
				} else {
					model="";
				}
				$('#snakerflow').snakerflow({
					basePath : "/resources/js/snaker/snaker/",
                    ctxPath : "${ctx}",
					restore : model,
                    formPath : "forms/",
					tools : {
						save : {
							onclick : function(data) {
								saveModel(data);
							}
						}
					}
				});
			});
			
			function saveModel(data) {
				alert(data);
				$.ajax({
					type:'POST',
					url:"${ctx}/snaker/process/deployXml",
					data:"model=" + data + "&id=${processId}",
					async: false,
					globle:false,
					error: function(){
						alert('数据处理错误！');
						return false;
					},
					success: function(data){
						if(data == true) {
							window.location.href = "${ctx}/snaker/process/list";
						} else {
							alert('数据处理错误！');
						}
					}
				});
			}
		</script>					
</head>
<body>
<%--<div id="wrapper">
	<jsp:include page="meun.jsp"/>

	&lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
	<div id="page-wrapper">--%>
		<div class="row">
			<div class="col-lg-12">
				<h1 class="page-header">业务流程图</h1>
			</div>
			<!-- /.col-lg-12 -->
		</div>
		<div>
			<table align="center" border="0" cellpadding="0" cellspacing="0">
				<tr>
					<td align="left">
						<input class="btn" type="button" value="返回" onclick="history.back();" />
					</td>
				</tr>
			</table>
		</div>
		<div>
			<table class="properties_all" align="center" border="1" cellpadding="0" cellspacing="0" style="margin-top: 0px">
				<div id="snakerflow" style="border: 1px solid #d2dde2; height:800px; margin-top:10px; margin-left:10px; width:98%;">
				</div>
			</table>
		</div>
	<%--</div>
</div>--%>
</body>
</html>
