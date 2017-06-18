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

		<title>${sessionScope.APP_NAME}-显示模板数据编辑</title>

		<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
		<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
		<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
		<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
		<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

		<script type="text/javascript" src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
		<script type="text/javascript" src="<c:url value="/resources/js/common.js"/>"></script>

		<script type="text/javascript">

			function checkDisplayTemplateDataModify(){
				with(document.forms[0]){
					if(!index.value){
						alert('数据索引必须填写');
						return false;
					}
				}
				document.getElementById('submit').click();
				return true;
			}

			function checkDisplayTemplateDataIndex(){
				confirmDeviceTemplate();
				var index = document.forms[0].index.value.trim();
				var displayTemplate = document.forms[0].displayTemplate.value.trim();
				if(index){
//					eval("var params = {index:'"+encodeURIComponent(index)+"',displayTemplate:'"+encodeURIComponent(displayTemplate)+"'}");
//					Ext.Ajax.request({
//						method : 'POST',
//						url: '/api/checkDisplayTemplateDataIndex',
//						success:checkDisplayTemplateDataIndexAndBack,
//						failure:function(){},
//						params:params
//					});
					$.ajax({
						type: 'POST',
						url: '/api/displayTemplate/checkDisplayTemplateDataIndex',
						data: {index:index, displayTemplate:displayTemplate},
						success: function (response) {

							if(response == 'indexNotNumber'){
								document.getElementById('indexError').innerText='数据索引必须为数字，请重新输入！';
							}else if(response == 'isExist'){
								document.getElementById('indexError').innerText='该数据索引已被占用，请重新输入！';
							} else if(response == 'ok'){
								document.getElementById('indexError').innerText='';
							}
						}
					});
				}
			}

//			function checkDisplayTemplateDataIndexAndBack(request){
//
//				document.getElementById('indexError').innerText='';
//
//				if(request.responseText){
//					var state = request.responseText;
//					if(state == 'indexNotNumber'){
//						document.getElementById('indexError').innerText='数据索引必须为数字，请重新输入！';
//					}else if(state == 'isExist'){
//						document.getElementById('indexError').innerText='该数据索引已被占用，请重新输入！';
//					} else if(state == 'ok'){
//						document.getElementById('indexError').innerText='';
//					}
//				}
//			}

			function confirmDeviceTemplate(){
				var displayTemplate = document.forms[0].displayTemplate.value.trim();
				if(displayTemplate){
//					eval("var params = {displayTemplate:'"+encodeURIComponent(displayTemplate)+"'"+"}");
//					Ext.Ajax.request({
//						method : 'POST',
//						url: '/api/confirmDeviceTemplate',
//						success:confirmDeviceTemplateAndBack,
//						failure:function(){},
//						params:params
//					});
					$.ajax({
						type: 'POST',
						url: '/api/displayTemplate/confirmDeviceTemplate',
						data: { displayTemplate:displayTemplate},
						success: function (response) {
							document.getElementById('confirmTemplate').innerText='';
							if(response != 'error'){
								var state = "请对应选择前缀为：'" + response + "'的模板数据！";
								document.getElementById('confirmTemplate').innerText = state;
							}else {
								document.getElementById('confirmTemplate').innerText='找不到显示模板对应的设备模板！';
							}
						}
					});
				}
			}

//			function confirmDeviceTemplateAndBack(request){
//
//				document.getElementById('confirmTemplate').innerText='';
//
//				if(request.responseText){
//					var state = request.responseText;
//					if(state != 'error'){
//						state = "请对应选择前缀为：'" + state + "'的模板数据！";
//						document.getElementById('confirmTemplate').innerText= state;
//					}else {
//						document.getElementById('confirmTemplate').innerText='找不到显示模板对应的设备模板！';
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
						<h3 class="page-header">显示模板数据编辑</h3>
					</div>
					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<form:form cssClass="form-horizontal" action="displayTemplateDataModify" commandName="displayTemplateDataModifyForm">
					<form:hidden path="id"/>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">所属显示模板</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:select cssClass="form-control"  path="displayTemplate" items="${displayTemplates}" onblur="checkDisplayTemplateDataIndex()"/>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">数据索引</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input type="text" cssClass="form-control" path="index" onblur="checkDisplayTemplateDataIndex()"/>
						</div>
						<span id="indexError" style="color:red"></span>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">关联设备模板数据</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:select cssClass="form-control"  path="deviceTemplateData" items="${deviceTemplateDatas}"/>
						</div>
						<span id="confirmTemplate" style="color:blue"></span>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">图标</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input type="text" cssClass="form-control" path="icon"/>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">显示样式1</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input type="text" cssClass="form-control" path="extraString1" />
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">显示样式2</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input type="text" cssClass="form-control" path="extraString2" />
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-2 col-lg-2">
							<label class="control-label">显示样式3</label>
						</div>
						<div class="col-xs-8 col-lg-3">
							<form:input type="text" cssClass="form-control" path="extraString3" />
						</div>
					</div>
					<form:input type="submit" cssClass="btn"  path="submit" value="保存" onclick="return checkDisplayTemplateDataModify();"/>
					<input class="btn" type="button" value="返回" onclick="history.back();" />
					</form:form>
				<%--</div>
			</div>--%>
	</body>
</html>
