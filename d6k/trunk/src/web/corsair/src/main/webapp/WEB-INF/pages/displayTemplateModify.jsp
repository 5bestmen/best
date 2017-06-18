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
		<link href="resources/css/style.css" rel="stylesheet" type="text/css">

		<title>${sessionScope.APP_NAME}-显示模板编辑</title>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

		<script type="text/javascript" src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
		<script type="text/javascript" src="<c:url value="/resources/js/common.js"/>"></script>

		<script type="text/javascript">

			function checkDisplayTemplateModify(){
				with(document.forms[0]){
					if(!name.value){
						alert('模板名必须填写！');
						return false;
					}
					if(!desc.value){
						alert('描述必须填写！');
						return false;
					}
				}
				document.getElementById('submit').click();
				return true;
			}

			function checkDisplayTemplateName(){
				var name = document.forms[0].name.value.trim();
				if(name){
//					eval("var params = {name:'"+encodeURIComponent(name)+"'"+"}");
//					Ext.Ajax.request({
//						method : 'POST',
//						url: '/api/checkDisplayTemplateName',
//						success:checkDeviceTemplateNameAndBack,
//						failure:function(){},
//						params:params
//					});
					$.ajax({
						type: 'POST',
						url: '/api/displayTemplate/checkDisplayTemplateName',
						data: {name:name},
						success: function (response) {

							if(response == '1'){
								document.getElementById('nameError').innerText='该模板名已存在，请更换模板名！';
							}
							else if(response == '0'){
								document.getElementById('nameError').innerText='';
							}
						}
					});
				}
			}

//			function checkDeviceTemplateNameAndBack(request){
//				if(request.responseText){
//					var isExist = request.responseText;
//					if(isExist=='1'){
//						document.getElementById('nameError').innerText='该模板名已存在，请更换模板名！';
//					}
//					else if(isExist=='0'){
//						document.getElementById('nameError').innerText='';
//					}
//				}
//			}

			function checkDisplayTemplateDesc(){
				var desc = document.forms[0].desc.value.trim();
				if(desc){
//					eval("var params = {desc:'"+encodeURIComponent(desc)+"'"+"}");
//					Ext.Ajax.request({
//						method : 'POST',
//						url: '/api/checkDisplayTemplateDesc',
//						success:checkDeviceTemplateDescAndBack,
//						failure:function(){},
//						params:params
//					});
					$.ajax({
						type: 'POST',
						url: '/api/displayTemplate/checkDisplayTemplateDesc',
						data: {desc:desc},
						success: function (response) {

							if(response == '1'){
								document.getElementById('descError').innerText='该模板描述已存在，请更换描述！';
							}
							else if(response == '0'){
								document.getElementById('descError').innerText='';
							}
						}
					});
				}
			}

//			function checkDeviceTemplateDescAndBack(request){
//				if(request.responseText){
//					var isExist = request.responseText;
//					if(isExist=='1'){
//						document.getElementById('descError').innerText='该模板描述已存在，请更换描述！';
//					}
//					else if(isExist=='0'){
//						document.getElementById('descError').innerText='';
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
						<h3 class="page-header">显示模板编辑</h3>
					</div>
					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<form:form cssClass="form-horizontal" action="displayTemplateModify" commandName="displayTemplateModifyForm">
					<form:hidden path="id"/>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">模板名</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input path="name" cssClass="form-control" onblur="checkDisplayTemplateName()"/>
						</div>
						<span id="nameError" style="color:red"></span>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">描述</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input type="text" cssClass="form-control" path="desc" onblur="checkDisplayTemplateDesc()"/>
						</div>
						<span id="descError" style="color:red"></span>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">是否需要容器</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:select cssClass="form-control"  path="needContainer" items="${needFlags}"></form:select>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">关联设备模板</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:select cssClass="form-control"  path="deviceTemplate" items="${deviceTemplates}"></form:select>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">显示区域</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:select cssClass="form-control"  path="displayArea" items="${displayAreas}"></form:select>
						</div>
					</div>
					<%--<div class="col-sm-1"></div>--%>
					<form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkDisplayTemplateModify();"/>
					<input class="btn" type="button" value="返回" onclick="history.back();" />
					</form:form>
				<%--</div>
			</div>--%>
	</body>
</html>
