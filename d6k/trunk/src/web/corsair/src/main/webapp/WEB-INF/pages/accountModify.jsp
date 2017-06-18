<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" pageEncoding="UTF-8"%>
<%--<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>--%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
	<head>

		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
		<title>用户维护</title>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

		<script type="text/javascript" src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
		<script type="text/javascript" src="<c:url value="/resources/js/common.js"/>"></script>
		<script type="text/javascript">

			function checkAccountModify(){
				with(document.forms[0]){
					if(!username.value){
						alert('用户登录名必须填写！');
						return false;
					}
					if(!realname.value){
						alert('用户显示名必须填写！');
						return false;
					}
					if(!id.value){
						if(!password.value){
							alert('密码必须填写！');
							return false;
						}
					}
					if((password.value || passwordRepeat.value) && password.value.trim()!= passwordRepeat.value.trim()){
						alert('密码和重复密码必须一致！');
						return false;
					}
					//检查用户登录名和用户显示名是否重复
					if(document.getElementById('usernameError').innerText){
						alert(document.getElementById('usernameError').innerText);
						return false;
					}
				}
				document.getElementById('submit').click();
				return true;
			}

			function checkAccountName(){
				var username = document.forms[0].username.value.trim();
				if(username){
					eval("var params = {username:'"+encodeURIComponent(username)+"',ignoreId:'"+document.forms[0].id.value+"'}");
					Ext.Ajax.request({
						method : 'POST',
						url: '/checkAccountNameServlet',
						success:checkAccountNameAndBack,
						failure:function(){},
						params:params
					});
				}
			}

			function checkAccountNameAndBack(request){
				if(request.responseText){
					var isExist = request.responseText;
					if(isExist=='1'){
						document.getElementById('usernameError').innerText='该用户ID已存在！';
					}
					else if(isExist=='0'){
						document.getElementById('usernameError').innerText='';
					}
				}
			}
		</script>

	</head>

	<body onload="checkAccountName();">

		<%--<div id="wrapper">
			<jsp:include page="meun.jsp"/>

			&lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
			<div id="page-wrapper">--%>
				<div class="row">
					<div class="col-lg-12">
						<h3 class="page-header">用户维护</h3>
					</div>
					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<form:form cssClass="form-horizontal" action="accountModify" commandName="accountModifyForm">
						<form:hidden path="id"/>
						<div class="form-group">
							<div class="col-sm-1">
								<label class="control-label">用户ID</label>
								<span style="color:red" title="必填">(*)</span>
							</div>
							<div class="col-xs-3">
								<form:input path="username" cssClass="form-control" onblur="checkAccountName()"/>
							</div>
							<span id="usernameError" style="color:red"></span>
						</div>
						<div class="form-group">
							<div class="col-sm-1">
								<label class="control-label">用户名</label>
								<span style="color:red" title="必填">(*)</span>
							</div>
							<div class="col-xs-3">
								<form:input cssClass="form-control" path="realname"/>
							</div>
						</div>
						<div class="form-group">
							<div class="col-sm-1">
								<label class="control-label">密码</label>
								<c:if test="${empty requestScope['accountModifyForm'].id}">
									<span style="color:red" title="必填">(*)</span>
								</c:if>
							</div>
							<div class="col-xs-3">
								<form:password path="password" cssClass="form-control" />
							</div>
						</div>
						<div class="form-group">
							<div class="col-sm-1">
								<label class="control-label">重复密码</label>
								<c:if test="${empty requestScope['accountModifyForm'].id}">
									<span style="color:red" title="必填">(*)</span>
								</c:if>
							</div>
							<div class="col-xs-3">
								<form:password path="passwordRepeat" cssClass="form-control" />
							</div>
						</div>
						<div class="form-group">
							<div class="col-sm-1">
								<label class="control-label">角色</label>
							</div>
							<div class="col-xs-3">
							<form:select path="roleId" cssClass="form-control">
								<form:option value="-1" label="--请选择角色"/>
								<form:options items="${roleList}" itemValue="id" itemLabel="roleDesc"/>
							</form:select>
							</div>
						</div>
						<div class="col-sm-1"></div>
						<input type="button" class="btn" value="返回" onclick="history.go(-1);"/>
						<form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkAccountModify();" />
					</form:form>
				<%--</div>
			</div>--%>
	</body>
</html>
