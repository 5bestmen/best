<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%--<%@ taglib prefix="form" uri="http://struts.apache.org/tags-html" %>--%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>业务流程管理</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
    <link href="resources/css/style.css" rel="stylesheet" type="text/css">

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
            var count =4*i-2;
            $("tbody tr:even").children().eq(count).children(0).children(0).attr("src","/resources/img/Edit_W.png");
            $("tbody tr:odd").children().eq(count).children(0).children(0).attr("src","/resources/img/Edit_B.png");
        }
    });

</script>
<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>


    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-12">
                <h3 class="page-header">业务流程管理</h3>
            </div>
            <!-- /.col-lg-12 -->
        </div>
        <!-- /.row -->
        <form:form cssClass="form-inline" action="ticketProcessList" method="get" >
            <%--<form:hidden path="deleteIds"/>--%>
            <%--<form:input type="submit" value="批量删除" cssStyle="display: none" path="del"/>--%>
            <%--<form:hidden path="viewId"/>--%>
            <%--<form:input type="submit" value="预览" cssStyle="display:none" path="view"></form:input>--%>
            <div style="margin:10px 0;"></div>
            <div class="row">
                <div class="col-lg-12 col-md-12">
                    <div class="table-responsive">
                        <table class="table table-bordered table-striped">
                            <thead>
                            <tr id="first">
                                <th>
                                    流程名称
                                </th>
                                <th>
                                    流程描述
                                </th>
                                <th>
                                    流程状态
                                </th>
                                <th>
                                    启动流程
                                </th>
                                <th>
                                    流程图
                                </th>
                            </tr>
                            </thead>
                            <tbody>
                            <%--<c:if test="${not empty ticketProcessList}">--%>
                                <c:forEach items="${page.result}" var="list">
                                    <tr>
                                        <td>
                                            <c:out value="${list.name}"/>
                                        </td>
                                        <td>
                                            <c:out value="${list.displayName}"/>
                                        </td>
                                        <td align="center">
                                            <c:out value="${list.state  == 1 ? '启用' : '禁用' }"/>
                                        </td>
                                        <td align="center">
                                            <c:if test="${list.name  == 'workTicket' }">
                                                <a href="workTicketModify?processId=${list.id }&processName=${list.name }">
                                                    <i class="glyphicon glyphicon-play"></i>
                                                    启动流程
                                                </a>
                                            </c:if>
                                            <c:if test="${list.name  == 'workTicketProcess' }">
                                                <a href="workTicketModify?processId=${list.id }&processName=${list.name }">
                                                    <i class="glyphicon glyphicon-play"></i>
                                                    启动流程
                                                </a>
                                            </c:if>
                                            <c:if test="${list.name  == 'operateTicket' }">
                                                <a href="operateTicketEdit">
                                                    <i class="glyphicon glyphicon-play"></i>
                                                    启动流程
                                                </a>
                                            </c:if>
                                        </td>
                                        <td align="center">
                                            <a href="ticketProcessDesigner?processId=${list.id }">
                                                <i class="glyphicon glyphicon-random"></i>
                                                流程图
                                            </a>
                                        </td>
                                    </tr>
                                </c:forEach>
                            <%--</c:if>--%>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </form:form>
   <%-- </div>
    <!-- /#page-wrapper -->

</div>--%>
<!-- /#wrapper -->
</body>

</html>
