<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<%@ include file="/common/taglibs.jsp"%>
<!DOCTYPE html>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>工作票终结</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">


    <link href="<c:url value="/resources/dist/js/html5shiv.min.js"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/js/respond.min.js"/>" rel="stylesheet" type="text/css">

    <script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
    <%--todo 页面自身需要的CSS begin--%>
    <link href="<c:url value="/resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/WdatePicker.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/default/datepicker.css"/>" rel="stylesheet" type="text/css">
    <%--todo 页面自身需要的CSS end--%>
</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/bower_components/dtGrid/js/jquery.dtGrid.min.js"/>"></script>
<script src="<c:url value="/resources/bower_components/dtGrid/i18n/zh-cn.js"/>"></script>
<script src="<c:url value="/resources/bower_components/datePicker/WdatePicker.js"/>"></script>
<script src="<c:url value="/resources/js/workTicket/validate.js"/>"></script>
<style type="text/css">
    .note{color: red}
</style>
<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>

    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-22">
                <h2 class="page-header">工作票终结</h2>
            </div>
            <!-- /.col-lg-22 -->
        </div>
        <form:form cssClass="form-horizontal" action="workTicketEnd" commandName="workTicketModifyForm" onsubmit="return toValid()">
            <form:hidden path="id"/>
            <form:hidden path="processId"/>
            <form:hidden path="orderId"/>
            <form:hidden path="taskId"/>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作终结时间</label>
                    <span style="color:red" title="必填">(*)</span>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="工作终结时间" path="workEndTime"/>
                </div>
                <span class="note" id="endNote">(*)必填</span>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作票创建时间</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <span id="createTime" >${createTime}</span>
                </div>
            </div>

            <form:input type="submit" cssClass="btn"  path="submit" value="办理终结"/>
            <input class="btn" type="button" value="返回" onclick="history.back();" />
        </form:form>

        <script>

            var time = {
                elem: '#workEndTime',
                format: 'YYYY-MM-DD hh:mm:ss',
                max: '2099-06-16',
                istime: true,
                istoday: true
            };

            laydate(time);

            function toValid(){
                //校验日期非空且必须大于工作票创建时间
                if(!checkNull("#workEndTime","#endNote")){
                    return false;
                }

                //校验计划结束时间，必须大于结束时间createTime
                if(!compareTime("#createTime","#workEndTime")){
                    return false;
                }
                return true;
            }

        </script>
    <%--</div>

</div>--%>

</body>

</html>