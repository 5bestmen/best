<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
	<head>
		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

		<title>菜单维护</title>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

		<script src="dist/js/html5shiv.min.js"></script>
		<script src="dist/js/respond.min.js"></script>

		<script type="text/javascript" src="extjs/ext-all.js"></script>
		<script type="text/javascript" src="js/common.js"></script>
		<script type="text/javascript">
			function checkMenuModify(){
				with(document.forms[0]){
					if(isBlank(name.value)){
						alert('菜单名称必须填写！');
						return false;
					}
					if(isNotBlank(orderSeq.value) && (isNaN(orderSeq.value) || !/^[1-9]\d*$/.test(orderSeq.value))){
						alert('菜单顺序必须是正整数！');
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
						<h3 class="page-header">菜单维护</h3>
					</div>
					<!-- /.col-lg-12 -->
				</div>
				<form:form cssClass="form-horizontal" action="menuModify" commandName="menuModifyForm">
				<form:hidden path="id"/>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">菜单名称</label>
							<span style="color:red" title="必填">(*)</span>
						</div>
						<div class="col-xs-3">
							<form:input type="text" cssClass="form-control"  path="name" />
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">菜单路径</label>
						</div>
						<div class="col-xs-3">
							<form:input type="text" cssClass="form-control" path="url"/>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">菜单顺序</label>
						</div>
						<div class="col-xs-3">
							<form:input type="text" cssClass="form-control" path="orderSeq"/>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">上级菜单</label>
						</div>
						<div class="col-xs-3">
						<form:select cssClass="form-control"  path="parentId" itemValue="id" itemLabel="name" items="${menuList}">
						</form:select>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">关联权限</label>
						</div>
						<div class="col-xs-3">
						<form:select cssClass="form-control"  path="actionId" itemValue="id" itemLabel="name" items="${actionList}">
						</form:select>
						</div>
					</div>
					<div class="col-sm-1"></div>
					<input class="btn" type="button" value="返回" onclick="history.go(-1);" />
					<form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkMenuModify();"/>
				</form:form>
			<%--</div>
		</div>--%>
	</body>
</html>
