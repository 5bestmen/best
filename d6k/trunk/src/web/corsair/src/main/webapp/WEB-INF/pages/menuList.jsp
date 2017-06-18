<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>

<html lang="zh_CN">
	<head>
		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<meta name="description" content="">
		<meta name="author" content="">

		<title>菜单管理</title>



		<link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
		<link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
		<link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

		<!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
		<!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
		<!--[if lt IE 9]>
		<script src="dist/js/html5shiv.min.js"></script>
		<script src="dist/js/respond.min.js"></script>
		<![endif]-->

		<%--todo 页面自身需要的CSS begin--%>
		<%--页面自身需要的CSS end--%>
	</head>
	<body>
	<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
	<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
	<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
	<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
	<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>
	<script type="application/javascript">
		$(function(){
			$("tbody tr:even").css("background-color","rgb(31, 86, 134)");
			$("tbody tr:even").children().css("color","white");

			$("tbody tr:odd").css("background","#82accb");
			var num = $("tbody tr:odd").children().length;
			for(var i=1;i < num;i++){
				var count =5*i-2;
				$("tbody tr:even").children().eq(count).children(0).children(0).attr("src","/resources/img/Edit_W.png");
				$("tbody tr:odd").children().eq(count).children(0).children(0).attr("src","/resources/img/Edit_B.png");

			}

		});
	</script>
		<%--<div id="wrapper">
			<jsp:include page="meun.jsp"/>
			<div id="page-wrapper">--%>
				<div class="row">
					<div class="col-lg-12">
						<h3 class="page-header">菜单列表</h3>
					</div>
					<!-- /.col-lg-12 -->
				</div>
				<!-- /.row -->
				<%--<form role="form" class="form-inline" action="menuList.do">--%>
             <form:form cssClass="form-inline" action="menuList" commandName="menuListForm">
	               <form:hidden path="deleteIds"/>
	               <form:input type="submit" value="批量删除" cssStyle="display:none" path="del"></form:input>
					<div class="form-group">
						<input class="form-control" placeholder="菜单名称" name="nameQry" autofocus/>
					</div>
					<div class="form-group">
						<input class="form-control" placeholder="菜单路径" name="urlQry"/>
					</div>

					<button type="submit" class="btn btn-primary">
						<i class="glyphicon glyphicon-search"></i>
						查询菜单
					</button>
					<a class="btn btn-primary" href="menuModify">
						<i class="glyphicon glyphicon-plus"></i>
						新增菜单
					</a>
				 	<div style="margin:10px 0;"></div>
					<div class="row">
						<div class="col-lg-12 col-md-12">
							<div class="table-responsive">
								<table class="table table-bordered table-striped">
									<thead>
									<tr id="first">
										<th>
											菜单名称
										</th>
										<th>
											菜单路径
										</th>
										<th>
											上级菜单名称
										</th>
										<th>
											编辑
										</th>
										<th>
											删除
										</th>
									</tr>
									</thead>
									<tbody>
									<c:if test="${not empty menuList}">
										<c:forEach items="${menuList}" var="menu">
											<tr>
												<td>
													<c:out value="${menu.name}"/>
												</td>
												<td>
													<c:out value="${menu.url}"/>
												</td>
												<td>
													<c:out value="${menu.parentName}"/>
												</td>
												<td align="center">
													<a  href="menuModify?id=${menu.id }">
														<%--<i class="glyphicon glyphicon-edit"></i>class="btn btn-primary"
														编辑--%>
															<img /></a>
												</td>
												<td align="center">
													<a
													   href="javascript:if(confirm('是否确定删除这条记录？')){document.forms[0].deleteIds.value='${menu.id}';document.forms[0].del.click();}">
														<%--<i class="glyphicon glyphicon-remove"></i>class="btn btn-warning"
														删除--%>
															<img src="/resources/img/Trash.png"/>
													</a>
												</td>
											</tr>
										</c:forEach>
									</c:if>
									</tbody>
								</table>
							</div>
						</div>
					</div>
					</form:form>

			<%--</div>
		</div>--%>
	</body>
</html>
