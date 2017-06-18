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

    <title>工作票列表</title>

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
        var num = $("tbody tr:odd").children().length;
        for(var i=1;i < num;i++){
            var count =8*i-3;
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
                <h3 class="page-header">工作票列表</h3>
            </div>
            <!-- /.col-lg-12 -->
        </div>
        <!-- /.row -->
        <form:form cssClass="form-inline" action="workTicketList" commandName="workTicketForm" >
            <form:hidden path="deleteIds"/>
            <form:input type="submit" value="批量删除" cssStyle="display: none" path="del"/>
            <form:hidden path="viewId"/>
            <form:input type="submit" value="预览" cssStyle="display:none" path="view"></form:input>

            <div style="margin:10px 0;"></div>
            <div class="row">
                <div class="col-lg-12 col-md-12">
                    <div class="table-responsive">
                        <table class="table table-bordered table-striped">
                            <thead>
                            <tr id="first">
                                <th>
                                    工作票类型
                                </th>
                                <th>
                                    工作责任人
                                </th>
                                <th>
                                    状态
                                </th>
                                <th>
                                    工作票创建时间
                                </th>
                                <th>
                                    增加工作记录
                                </th>
                                <th>
                                    每日工作记录
                                </th>
                                <th>
                                    删除
                                </th>
                                <th>
                                    生成工作票
                                </th>
                            </tr>
                            </thead>
                            <tbody>
                            <c:if test="${not empty workTicketList}">
                                <c:forEach items="${workTicketList}" var="workTicket">
                                    <tr>
                                        <td>
                                            <c:if test="${workTicket.workTicketType  == 1 }">
                                                一级动火工作票
                                            </c:if>
                                            <c:if test="${workTicket.workTicketType  == 2 }">
                                                二级动火工作票
                                            </c:if>
                                            <c:if test="${workTicket.workTicketType  == 3 }">
                                                电气一种工作票
                                            </c:if>
                                            <c:if test="${workTicket.workTicketType  == 4 }">
                                                电气二种工作票
                                            </c:if>
                                        </td>
                                        <td>
                                            <c:out value="${workTicket.newResponsiblePerson }"/>
                                        </td>
                                        <td>
                                            <c:if test="${workTicket.state  == 0 }">
                                                未审核
                                            </c:if>
                                            <c:if test="${workTicket.state  == 1 }">
                                                审核中
                                            </c:if>
                                            <c:if test="${workTicket.state  == 3 }">
                                                审核通过
                                            </c:if>
                                            <c:if test="${workTicket.state  == 4 }">
                                                审核不通过
                                            </c:if>
                                        </td>
                                        <td>
                                            <c:out value="${workTicket.workTicketCreateTime }"/>
                                        </td>
                                        <td>
                                            <a href="workTicketRecord?id=${workTicket.id }">
                                                <i class="glyphicon glyphicon-plus"></i>
                                                <%--新增--%>
                                            </a>
                                        </td>
                                        <td>
                                            <a href="workTicketRecordList?id=${workTicket.id }">
                                                <img/>
                                            </a>
                                        </td>
                                        <c:if test="${workTicket.state == 1}">
                                            <td align="center">
                                                <a href="javascript:alert('审核未完成，无法编辑工作票！')">
                                                    <%--<i class="glyphicon glyphicon-remove"></i>
                                                    删除--%>
                                                        <img src="/resources/img/Trash.png"/>
                                                </a>
                                            </td>
                                            <td align="center">
                                                <a href="javascript:alert('审核未完成，无法预览工作票！')" >
                                                    <i class="glyphicon glyphicon glyphicon-list-alt"></i>
                                                    预览
                                                </a>
                                            </td>
                                        </c:if>
                                        <c:if test="${workTicket.state  == 3 ||  workTicket.state == 4}">
                                            <td align="center">
                                                <a href="javascript:if(confirm('是否确定删除这条记录？')){document.forms[0].deleteIds.value='${workTicket.id}';document.forms[0].del.click();}">
                                                    <%--<i class="glyphicon glyphicon-remove"></i>
                                                    删除--%>
                                                        <img src="/resources/img/Trash.png"/>
                                                </a>
                                            </td>
                                            <td align="center">
                                                <a href="javascript:document.forms[0].viewId.value='${workTicket.id}';document.forms[0].view.click();">
                                                    <i class="glyphicon glyphicon glyphicon-list-alt"></i>
                                                    预览
                                                </a>
                                            </td>
                                        </c:if>
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
    <!-- /#page-wrapper -->

</div>--%>
<!-- /#wrapper -->
</body>

</html>
