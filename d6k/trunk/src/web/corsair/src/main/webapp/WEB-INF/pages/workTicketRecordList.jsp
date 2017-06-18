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

    <title>系统名-主页</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
    <!--<script src="dist/js/html5shiv.min.js"></script>-->
    <!--<script src="dist/js/respond.min.js"></script>-->
    <%--<![endif]-->--%>

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
    });

</script>


<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>
    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-12">
                <h3 class="page-header">每日工作记录</h3>
            </div>
            <!-- /.col-lg-12 -->
        </div>
        <!-- /.row -->
        <form:form cssClass="form-inline" action="workTicketRecordList" method="post" commandName="workTicketRecordForm" >
            <form:hidden path="id"/>
            <div style="margin:10px 0;"></div>
            <div class="row">
                <div class="col-lg-12 col-md-12">
                    <div class="table-responsive">
                        <table class="table table-bordered table-striped">
                            <thead>
                            <tr id="first">
                                <th>
                                    日期
                                </th>
                                <th>
                                    开工时间
                                </th>
                                <th>
                                    收工时间
                                </th>
                                <th>
                                    工作负责人
                                </th>
                                <th>
                                    工作许可人
                                </th>
                            </tr>
                            </thead>
                            <c:if test="${not empty workTicketRecord}">
                                <c:forEach items="${workTicketRecord}" var="workTicketRecord">
                                    <tr>
                                        <td>
                                            <c:out value="${workTicketRecord.recordDate } "/>
                                        </td>
                                        <td>
                                            <c:out value="${workTicketRecord.startTime } "/>
                                        </td>
                                        <td>
                                            <c:out value="${workTicketRecord.endTime}"/>
                                        </td>
                                        <td>
                                            <c:out value="${workTicketRecord.responsiblePerson }"/>
                                        </td>
                                        <td>
                                            <c:out value="${workTicketRecord.permitPerson }"/>
                                        </td>
                                    </tr>
                                </c:forEach>
                            </c:if>
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
