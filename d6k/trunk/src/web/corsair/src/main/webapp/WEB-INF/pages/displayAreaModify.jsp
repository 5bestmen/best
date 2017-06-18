<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%--<%@ taglib prefix="form" uri="http://struts.apache.org/tags-html" %>--%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html lang="zh_CN">
<%
	session.getAttributeNames();
%>
	<head>

		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

		<title>${sessionScope.APP_NAME}-显示区域编辑</title>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

		<script type="text/javascript" src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
		<script type="text/javascript" src="<c:url value="/resources/js/common.js"/>"></script>

		<script type="text/javascript">

			function checkDisplayAreaModify(){
				with(document.forms[0]){
					if(!name.value){
						alert('区域名必须填写！');
						return false;
					}
					if(!comments.value){
						alert('区域描述必须填写！');
						return false;
					}
				}
				document.getElementById('submit').click();
				return true;
			}

			function checkDisplayAreaName(){
				var name = document.forms[0].name.value.trim();
				if(name){
//					eval("var params = {name:'"+encodeURIComponent(name)+"'"+"}");
//					Ext.Ajax.request({
//						method : 'POST',
//						url: '/api/checkDisplayAreaName',
//						success:checkDisplayAreaNameAndBack,
//						failure:function(){},
//						params:params
//					});
					$.ajax({
						type: 'POST',
						url: '/api/displayArea/checkDisplayAreaName',
						data: {name:name},
						success: function (response) {

							if(response =='1'){
								document.getElementById('nameError').innerText='该区域名已存在，请更换区域名！';
							}
							else if(response =='0'){
								document.getElementById('nameError').innerText='';
							}
						}
					});
				}
			}

//			function checkDisplayAreaNameAndBack(request){
//				if(request.responseText){
//					var isExist = request.responseText;
//					if(isExist=='1'){
//						document.getElementById('nameError').innerText='该区域名已存在，请更换区域名！';
//					}
//					else if(isExist=='0'){
//						document.getElementById('nameError').innerText='';
//					}
//				}
//			}

			function checkDisplayAreaComments(){
				var comments = document.forms[0].comments.value.trim();
				if(comments){
//					eval("var params = {comments:'"+encodeURIComponent(comments)+"'"+"}");
//					Ext.Ajax.request({
//						method : 'POST',
//						url: '/api/displayArea/checkDisplayAreaComments',
//						success:checkDisplayAreaCommentsAndBack,
//						failure:function(){},
//						params:params
//					});

					$.ajax({
						type: 'POST',
						url: '/api/displayArea/checkDisplayAreaComments',
						data: {comments:comments},
						success: function (response) {

							if(response == '1'){
								document.getElementById('commentsError').innerText='该区域描述已存在，请更换描述！';
							}
							else if(response == '0'){
								document.getElementById('commentsError').innerText='';
							}
						}
					});
				}
			}

//			function checkDisplayAreaCommentsAndBack(request){
//				if(request.responseText){
//					var isExist = request.responseText;
//					if(isExist=='1'){
//						document.getElementById('commentsError').innerText='该区域描述已存在，请更换描述！';
//					}
//					else if(isExist=='0'){
//						document.getElementById('commentsError').innerText='';
//					}
//				}
//			}
		</script>

	</head>

	<body>

		<%--<div id="wrapper">
			<jsp:include page="meun.jsp"/>

			&lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
			<div id="page-wrapper">--%>
				<div class="row">
					<div class="col-lg-12">
						<h3 class="page-header">显示区域编辑</h3>
					</div>
					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<form:form cssClass="form-horizontal" action="displayAreaModify" commandName="displayAreaModifyForm">
					<form:hidden path="id"/>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">区域名</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input path="name" cssClass="form-control" onblur="checkDisplayAreaName()"/>
						</div>
						<span id="nameError" style="color:red"></span>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">区域描述</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input path="comments" cssClass="form-control" onblur="checkDisplayAreaComments()"/>
						</div>
						<span id="commentsError" style="color:red"></span>
					</div>
					<%--<div class="col-sm-1"></div>--%>
					<form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkDisplayAreaModify();"/>
					<input class="btn" type="button" value="返回" onclick="history.back();" />
					</form:form>
				<%--</div>
			</div>--%>
	</body>
</html>
