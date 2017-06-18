<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" pageEncoding="UTF-8"%>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>

<html>
	<head>

		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">


		<title>权限维护</title>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

		<script src="<c:url value="/resources/extjs/ext-all.js"/>"></script>

		<script type="text/javascript" src="js/common.js"></script>
		<script type="text/javascript">
			function checkActionModify(){
				with(document.forms[0]){
					if(isBlank(name.value)){
						alert('权限名称必须填写！');
						return false;
					}
					if(isBlank(url.value)){
						alert('权限路径必须填写！');
						return false;
					}
				}
				document.getElementById('submit').click();
				return true;
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
					<h2 class="page-header">权限维护</h2>
				</div>
				<!-- /.col-lg-12 -->
			</div>
			<!-- /.row -->

		<form:form cssClass="form-horizontal" action="actionModify" method="post" commandName="actionModifyForm">
			<form:hidden property="id" path="id"/>

			<div class="form-group">
				<div class="col-sm-1">
					<label class="control-label">权限名称</label>
					<span style="color:red" title="必填">(*)</span>
				</div>
				<div class="col-xs-3">
					<form:input cssClass="form-control" property="name" path="name"/>
				</div>
			</div>
			<div class="form-group">
				<div class="col-sm-1">
					<label class="control-label">权限路径</label>
					<span style="color:red" title="必填">(*)</span>
				</div>
				<div class="col-xs-3">
					<form:input cssClass="form-control" property="url" path="url"/>
				</div>
			</div>
			<div class="col-sm-1"></div>
			<input type="button" class="btn" value="返回" onclick="history.go(-1);" />
			<form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkActionModify();"/>
		</form:form>
			<%--</div>
		</div>--%>
	</body>
</html>
