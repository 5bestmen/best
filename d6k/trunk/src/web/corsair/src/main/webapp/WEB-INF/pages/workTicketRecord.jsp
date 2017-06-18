<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<%@ include file="/common/taglibs.jsp" %>
<!DOCTYPE html>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>填写工作时间</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">


    <link href="<c:url value="/resources/dist/js/html5shiv.min.js"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/js/respond.min.js"/>" rel="stylesheet" type="text/css">

    <script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
    <%--todo 页面自身需要的CSS begin--%>
    <link href="<c:url value="/resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>" rel="stylesheet"
          type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/WdatePicker.css"/>" rel="stylesheet"
          type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/default/datepicker.css"/>" rel="stylesheet"
          type="text/css">
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
            <div class="col-lg-12">
                <h3 class="page-header">填写工作记录</h3>
            </div>
            <!-- /.col-lg-22 -->
        </div>
        <form:form cssClass="form-horizontal" action="workTicketRecord" commandName="workTicketRecordForm" onsubmit="return toValid()">
            <form:hidden path="id"/>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">开工时间</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="开工时间" path="startTime"/>
                </div>
                <span class="note" id="startNote">(*)必填</span>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">收工时间</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="收工时间" path="endTime"/>
                </div>
                <span class="note" id="endNote">(*)必填</span>
            </div>

            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作负责人</label>
                    <span style="color:red" title="必填">(*)</span>
                </div>
                <div class="col-xs-3 col-lg-3">
                    <form:select cssClass="form-control" path="responsiblePerson"
                                 items="${accoutRealNames}"></form:select>
                </div>
                <span class="note" id="responsiblePersonNote"></span>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作许可人</label>
                    <span style="color:red" title="必填">(*)</span>
                </div>
                <div class="col-xs-3 col-lg-3">
                    <form:select cssClass="form-control" path="permitPerson" items="${accoutRealNames}"></form:select>
                </div>
                <span class="note" id="permitPersonNote"></span>
            </div>
            <form:input type="submit" cssClass="btn" path="submit" value="保存"/>
            <input class="btn" type="button" value="返回" onclick="history.back();"/>
        </form:form>
        <script>
            function toValid(){


                //校验计划开始时间，非空
                if(!checkNull("#startTime","#startNote")){
                    return false;
                }
                //校验计划结束时间，非空
                if(!checkNull("#endTime","#endNote")){
                    return false;
                }
                var startDay = $("#startTime").val().substring(0, 10);
                var endDay = $("#endTime").val().substring(0, 10);
                if (startDay != endDay) {
                    $("#endNote").html("必须输入同一天且开始时间小于结束时间，请重新输入！");
                    return false;
                }
                //校验计划结束时间，必须大于结束时间
                if(!compareTime("#startTime","#endTime")){
                    return false;
                }

                //校验计划开始时间，非空
                if(!checkNull("#responsiblePerson","#responsiblePersonNote")){
                    return false;
                }
                //校验计划结束时间，非空
                if(!checkNull("#permitPerson","#permitPersonNote")){
                    return false;
                }

                return true;
            }


            var timeStart = {
                elem: '#startTime',
                format: 'YYYY-MM-DD hh:mm:ss',
                max: '2099-06-16',
                istime: true,
                istoday: true
            }
            var timeEnd = {
                elem: '#endTime',
                format: 'YYYY-MM-DD hh:mm:ss',
                max: '2099-06-16',
                istime: true,
                istoday: true
            }
            laydate(timeStart);
            laydate(timeEnd);
        </script>
    <%--</div>

</div>--%>

</body>

</html>