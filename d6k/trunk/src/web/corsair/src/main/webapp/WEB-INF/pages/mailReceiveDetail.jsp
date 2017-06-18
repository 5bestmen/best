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
		<link href="<c:url value="/resources/extjs/resources/css/ext-all.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

		<title>角色维护</title>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootstrap-treeview.min.js"/>"></script>

		<script src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/html5shiv.min.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/respond.min.js"/>"></script>
		<script src="<c:url value="/resources/js/common.js"/>"></script>

	</head>

	<body>
		<%--<div id="wrapper">
			<jsp:include page="meun.jsp"/>

			&lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
			<div id="page-wrapper">--%>
				<div class="row">
					<div class="col-lg-12">
						<h2 class="page-header">收件人查看</h2>
					</div>

					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<form:form cssClass="form-horizontal" action="mailReceiveModify" method="get" commandName="mailReceiveModifyForm">
				<form:hidden property="id" path="id"/>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">人名</label>
						</div>

						<div class="col-xs-3">
							<form:input cssClass="form-control" property="receiverName" path="receiverName" disabled="true"/>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
						<label class="control-label">职务</label>
						</div>
						<div class="col-xs-3">
							<form:input cssClass="form-control" property="receiverJob" path="receiverJob" disabled="true"/>
						</div>
					</div>
					<div class="form-group">
					<div class="col-sm-1">
						<label class="control-label">号码</label>
					</div>
					<div class="col-xs-3">
						<form:input cssClass="form-control" property="phone" path="phone" disabled="true"/>
					</div>
				   </div>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">邮箱地址</label>
						</div>
						<div class="col-xs-3">
							<form:input cssClass="form-control" property="mailAddress" path="mailAddress" disabled="true"/>
						</div>
					</div>

					<div class="col-sm-1"></div>
					<input type="button" class="btn" value="返回" onclick="history.go(-1);" />
			</form:form>
			<%--</div>
	</div>--%>

	</body>
</html>
