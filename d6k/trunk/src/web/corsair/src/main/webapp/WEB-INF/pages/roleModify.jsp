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
					if(roleName.value.length==0){
						alert('角色名称必须填写！');
						return false;
					}

					menuIdArray = [];
					getChecked($('#menuTree').treeview('getNode', 0), menuIdArray);
                    
					menuIds.value = menuIdArray.join(',');
				}
				document.getElementById('submit').click();
				return true;
			}

			function getChecked(node, menuIdArray) {
				if(node.id!=0 && $('#menuTree').treeview('getNode', node.nodeId).state.checked == true) {
					menuIdArray.push(node.id);
				}
				if(node.leaf!=true) {
					for (var i=0; i<node.nodes.length; i++) {
						getChecked(node.nodes[i], menuIdArray);
					}
				}

			}

			function getTree() {

				$('#menuTree').treeview({
					data: [{id:0,text:"菜单",expanded: true,state: {checked: true},nodes:${menuJson}}],
					showIcon: false,
					showCheckbox: true
				});

				$('#menuTree').treeview('expandAll');

				// 选中子节点同时选中父节点
				$('#menuTree').on('nodeChecked', function(event, data) {

					if(data.nodeId==0) {
						return;
					}
					var parentNode = $('#menuTree').treeview('getParent', data.nodeId);
					while(typeof(parentNode.nodeId)=="number") {
						$('#menuTree').treeview('checkNode', [ parentNode, { silent: true } ]);
						parentNode = $('#menuTree').treeview('getParent', parentNode.nodeId);
					}
				});

				// 取消父节点同时取消子节点
				$('#menuTree').on('nodeUnchecked', function(event, data) {

					if(!data.leaf || data.nodes) {

						for (var i=0; i<data.nodes.length; i++) {
							$('#menuTree').treeview('uncheckNode', [ data.nodes[i].nodeId, { silent: false } ]);
						}
					}
				});
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
						<h3 class="page-header" >角色维护</h3>
					</div>

					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<form:form cssClass="form-horizontal" action="roleModify" method="post" commandName="roleModifyForm">
				<form:hidden property="id" path="id"/>
					<div class="form-group">
						<div class="col-sm-1">
							<label class="control-label" >角色名称</label>
							<span style="color:red" title="必填">(*)</span>
						</div>

						<div class="col-xs-3">
							<form:input cssClass="form-control" property="roleName" path="roleName"/>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-1">
						<label class="control-label" >角色描述</label>
						</div>
						<div class="col-xs-3">
							<form:input cssClass="form-control" property="roleDesc" path="roleDesc" />
						</div>
					</div>
					<div class="form-group">
						<div id="menuTree" class="col-sm-offset-1 col-xs-3"></div>
						<form:hidden property="menuIds" path="menuIds"/>
					</div>

					<div class="col-sm-1"></div>
					<input type="button" class="btn" value="返回" onclick="history.go(-1);" />
					<form:input type="submit" cssClass="btn"  path="submit" value="保存"  onclick="return checkRoleModify();"/>
			</form:form>
			<%--</div>
          </div>--%>
	</body>
</html>
