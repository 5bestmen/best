<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" pageEncoding="UTF-8"%>
<%--<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>--%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
	<head>
		<title>密码修改</title>
		<style type="text/css">
		</style>
		<script type="text/javascript" src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
		<script type="text/javascript" src="<c:url value="/resources/js/common.js"/>"></script>
		<script type="text/javascript">
				function checkPwdModify(){
					with(document.forms[0]){

							if(!password.value){
								alert('密码必须填写！');
								return false;
							}

						if((!password.value ||　!passwordRepeat.value) && password.value.trim()!=passwordRepeat.value.trim()){
							alert('密码和重复密码必须一致！');
							return false;
						}
					}
					document.getElementById('submit').click();
					return true;
				}
		</script>

		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

	</head>
	<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
	<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
	<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
	<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
	<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

	<body onload="if('${msg }'!=''){alert('${msg}');}">

	<%--<div id="wrapper">
		<jsp:include page="meun.jsp"/>


		&lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
		<div id="page-wrapper">--%>
			<div class="row">
				<div class="col-lg-12">
					<h3 class="page-header">密码修改</h3>
				</div>
				<!-- /.col-lg-12 -->
			</div>
			<!-- /.row -->


           <form:form cssClass="form-horizontal" action="pwdModify" method="post" modelAttribute="pwdModifyForm">
		<%--<form align="left" role="form" action="pwdModify.do" method="post">--%>
			<form:hidden path="id" />

			   <div align="left" class="input-group">
				   密码
				   <span style="color:red" title="必填">(*)</span>
				   <span>：</span>
				   <form:password cssClass="form-control" path="password" />
				   <%--<input type="password" id="passwordFirst" class="form-control" property="password" />--%>
			   </div>
			   <div align="left" class="input-group">
				   请再输入一次密码
				   <span style="color:red" title="必填">(*)</span>
				   <span>：</span>
				   <form:password cssClass="form-control" path="passwordRepeat" />
				   <%--<input type="password" id="passwordConform" class="form-control" property="password" />--%>
			   </div>
			   <div style="margin:10px 0;"></div>
			   <form:input path="submit" type="submit" value="保存" cssClass="btn" onclick="return checkPwdModify();"/>
			   <%--<input align="left" type="submit" id="submit" value="保存" property="submit" onclick="return checkPwdModify();"/>--%>
			   <input align="left" type="reset" class="btn" value="重置" />
		    </form:form>
	    <%--</div>
    </div>--%>
	</body>
</html>
