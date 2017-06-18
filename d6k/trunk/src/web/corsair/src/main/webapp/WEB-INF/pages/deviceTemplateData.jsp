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

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>${sessionScope.APP_NAME}-设备模板数据</title>

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
    });

</script>


<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>


    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-12">
                <h1 class="page-header">${titleDesc}模板数据</h1>
            </div>
            <!-- /.col-lg-12 -->
        </div>
        <!-- /.row -->

        <form:form cssClass="form-inline" action="deviceTemplateData" method="post" commandName="deviceTemplateDataForm">
            <form:hidden path="deleteIds" />
            <form:input type="submit" value="批量删除" cssStyle="display: none" path="del" />
            <div class="form-group">
                <input class="form-control" placeholder="数据描述" name="desc" autofocus/>
            </div>

            <button type="submit" class="btn btn-primary">
                <i class="glyphicon glyphicon-search"></i>
                查询数据
            </button>
            <a class="btn btn-primary" href="deviceTemplateDataModify">
                <i class="glyphicon glyphicon-plus"></i>
                新增数据
            </a>
            <div style="margin:10px 0;"></div>
            <div class="row">
                <div class="col-lg-12 col-md-12">
                    <div class="table-responsive">
                        <table class="table table-bordered table-striped">
                            <thead>
                            <tr id="first">
                                <th>
                                    数据类型
                                </th>
                                <th>
                                    索引号
                                </th>
                                <th>
                                    数据描述
                                </th>
                                <th>
                                    所属设备模板
                                </th>
                                <th>
                                    单位
                                </th>
                                <th>
                                    默认值
                                </th>
                                <th>
                                    编辑
                                </th>
                                <th>
                                    删除
                                </th>
                            </tr>
                            </thead>
                            <c:if test="${not empty deviceTemplateDataList}">
                                <c:forEach items="${deviceTemplateDataList}" var="data">
                                    <tr>
                                        <td>
                                            <c:if test="${data.type  == 1}">
                                                遥信
                                            </c:if>
                                            <c:if test="${data.type  == 2}">
                                                遥测
                                            </c:if>
                                            <c:if test="${data.type  == 3}">
                                                电度
                                            </c:if>
                                            <c:if test="${data.type  == 4}">
                                                特殊图表
                                            </c:if>
                                        </td>
                                        <td>
                                            <c:out value="${data.index }"/>
                                        </td>
                                        <td>
                                            <c:out value="${data.desc }"/>
                                        </td>
                                        <td>
                                            <c:out value="${data.deviceTemplate.desc }"/>
                                        </td>
                                        <td>
                                            <c:out value="${data.unitType }"/>
                                        </td>
                                        <td>
                                            <c:out value="${data.defaultValue }"/>
                                        </td>
                                        <td align="center">
                                            <a href="deviceTemplateDataModify?id=${data.id }">
                                                <i class="glyphicon glyphicon-edit"></i>
                                                <%--编辑--%></a>
                                        </td>
                                        <td align="center">
                                            <c:if test="${account.username ne 'admin'}">
                                                <a
                                                   href="javascript:if(confirm('是否确定删除这条记录？')){document.forms[0].deleteIds.value='${data.id}';document.forms[0].del.click();}">
                                                    <%--<i class="glyphicon glyphicon-remove"></i>
                                                    删除--%>
                                                        <img src="/resources/img/Trash.png"/>
                                                </a>
                                            </c:if>
                                        </td>
                                    </tr>
                                </c:forEach>
                            </c:if>
                        </table>
                    </div>
                </div>
            </div>
            <%--</html:form>--%>
        </form:form>

   <%-- </div>
    <!-- /#page-wrapper -->

</div>--%>
<!-- /#wrapper -->
</body>

</html>
