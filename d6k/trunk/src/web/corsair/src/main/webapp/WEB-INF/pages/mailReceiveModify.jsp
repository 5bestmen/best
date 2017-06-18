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

		<script type="text/javascript">
			function checkRoleModify(){
				with(document.forms[0]){
					if(receiverName.value.length==0){
						alert('人名必须填写！');
						return false;
					}
					if(phone.value.length==0)
					{
						alert('请输入手机号码！');
						document.forms[0].phone.focus();
						return false;
					}
					if(phone.value.length!=11)
					{
						alert('请输入有效的手机号码！');
						document.forms[0].phone.focus();
						return false;
					}

					var myphone = /^(((13[0-9]{1})|(15[0-9]{1})|(18[0-9]{1}))+\d{8})$/;
					if(!myphone.test(phone.value))
					{
						alert('请输入有效的手机号码！');
						document.forms[0].phone.focus();
						return false;
					}
					var myreg = /^([a-zA-Z0-9]+[_|\_|\.]?)*[a-zA-Z0-9]+@([a-zA-Z0-9]+[_|\_|\.]?)*[a-zA-Z0-9]+\.[a-zA-Z]{2,3}$/;
					if(!(myreg.test(mailAddress.value))){
						alert("邮箱格式不正确或者邮箱地址为空!");
						document.forms[0].mailAddress.focus();
						return false;
					}
				}
				document.getElementById('submit').click();
				return true;
			}

		</script>

	</head>

	<body onload="getTree()">
		<%--<div id="wrapper">
			<jsp:include page="meun.jsp"/>

			&lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
			<div id="page-wrapper">--%>
				<div class="row">
					<div class="col-lg-12">
						<h3 class="page-header">邮件接收人维护</h3>
					</div>

					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<form:form cssClass="form-horizontal" action="mailReceiveModify" method="post" commandName="mailReceiveModifyForm">
				<form:hidden property="id" path="id"/>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">人名</label>
							<span style="color:red" title="必填">(*)</span>
						</div>

						<div class="col-xs-3">
							<form:input cssClass="form-control" property="receiverName" path="receiverName"/>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
						<label class="control-label">职务</label>
						</div>
						<div class="col-xs-3">
							<form:input cssClass="form-control" property="receiverJob" path="receiverJob" />
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">号码</label>
							<span style="color:red" title="必填">(*)</span>
						</div>
						<div class="col-xs-3">
							<form:input cssClass="form-control" property="phone" path="phone" />
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label">邮箱地址</label>
							<span style="color:red" title="必填">(*)</span>
						</div>
						<div class="col-xs-3">
							<form:input cssClass="form-control" property="mailAddress" path="mailAddress" />
						</div>
					</div>

					<div class="col-sm-1"></div>
					<input type="button" class="btn" value="返回" onclick="history.go(-1);" />
					<form:input type="submit" cssClass="btn"  path="submit" value="保存"  onclick="return checkRoleModify();"/>
			</form:form>
			<%--</div>
            </div>--%>
	</body>
</html>
