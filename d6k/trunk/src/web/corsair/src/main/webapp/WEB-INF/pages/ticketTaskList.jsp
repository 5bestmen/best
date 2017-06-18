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

    <title>待办任务</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

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
        /*var num = $("tbody tr:odd").children().length;
        for(var i=1;i < num;i++){
            var count =8*i-3;
            $("tbody tr:even").children().eq(count).children(0).children(0).attr("src","/resources/img/Edit_W.png");
            $("tbody tr:odd").children().eq(count).children(0).children(0).attr("src","/resources/img/Edit_B.png");
        }*/
    });

</script>
<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>


    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-12">
                <h3 class="page-header">待办任务列表</h3>
            </div>
            <!-- /.col-lg-12 -->
        </div>
        <!-- /.row -->
        <form:form cssClass="form-inline" action="ticketTaskList" method="get">
            <%--<form:hidden path="deleteIds"/>--%>
            <%--<form:input type="submit" value="批量删除" cssStyle="display: none" path="del"/>--%>
            <%--<form:hidden path="viewId"/>--%>
            <%--<form:input type="submit" value="预览" cssStyle="display:none" path="view"></form:input>--%>
            <%--<a class="btn btn-primary" href="#">--%>
                <%--<i class="glyphicon glyphicon-plus"></i>--%>
                <%--新增流程--%>
            <%--</a>--%>
            <tr>
                <td class="td_table_top" align="left">
                    待办任务：<font color="red">[共:${majorTotal }项]&nbsp;&nbsp;</font>
                </td>
            </tr>
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
                                    流程编号
                                </th>
                                <th>
                                    流程启动时间
                                </th>
                                <th>
                                    任务名称
                                </th>
                                <th>
                                    任务创建时间
                                </th>
                                <th>
                                    查看流程
                                </th>
                                <th>
                                    处理流程
                                </th>
                            </tr>
                            </thead>
                            <tbody>
                            <%--<c:if test="${not empty ticketProcessList}">--%>
                                <c:forEach items="${majorTasks}" var="task">
                                    <tr>
                                        <td>
                                            <c:out value="${task.processName}"/>
                                        </td>
                                        <td>
                                            <c:out value="${task.orderNo}"/>
                                        </td>
                                        <td>
                                            <c:out value="${task.orderCreateTime}"/>
                                        </td>
                                        <td>
                                            <c:out value="${task.taskName}"/>
                                        </td>
                                        <td>
                                            <c:out value="${task.taskCreateTime}"/>
                                        </td>
                                        <td align="center">
                                            <a href="ticketProcessView?processId=${task.processId }&orderId=${task.orderId}">
                                                <i class="glyphicon glyphicon-random"></i>
                                                查看流程
                                            </a>
                                        </td>
                                        <td align="center">
                                            <a href="${task.actionUrl}?processId=${task.processId }&taskId=${task.taskId}&orderId=${task.orderId} ">
                                                <i class="glyphicon glyphicon-hand-up"></i>
                                                处理流程
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
    <%--</div>
    <!-- /#page-wrapper -->

</div>--%>
<!-- /#wrapper -->
</body>

</html>
