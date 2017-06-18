<%--
  Created by IntelliJ IDEA.
  User: ck15henry
  Date: 2015/7/8
  Time: 14:13
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<%
    session.getAttributeNames();
%>
<!DOCTYPE html>
<html lang="zh_CN">
<head>
    <meta charset="UTF-8">
    <title>光伏阵列</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/laydate/need/laydate.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/img/sun.png"/>" rel="shortcut icon"/>

    <link href="<c:url value="/resources/dist/js/html5shiv.min.js"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/js/respond.min.js"/>" rel="stylesheet" type="text/css">

</head>
<body>
    <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
    <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
    <script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
    <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
    <script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
    <script src="<c:url value="/resources/js/generalChart.js"/>"></script>
    <script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/robintable.js"/>"></script>
    <script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/measSelector.js"/>"></script>

    <script>
        function Measurement()
        {
            var meaObject = $.fn.GeneralMeasurement.init();
            meaObject.load();
        }
        window.onload=function(){Measurement();}
    </script>

        <div class="row">
            <div class="col-lg-12">
                <h3>${PVArrayName}</h3>
            </div>
        </div>
        <div class="row" style=" margin-left: -5px;">
            <c:forEach items="${ammeters}" var="ammeter" varStatus="i">
                <div class="col-md-6 measurement-container">
                    <div class="panel-heading" style="padding: 10px 15px;font-size: 16px;">
                        ${ammeter.name}
                    </div>
                    <corsair:measurement displayAreaName="station-ammeter-info"
                                         PSRID="${ammeter.id}"/>
                </div>
            </c:forEach>

        </div>
        <div class="row" >
            <c:forEach items="${inverters}" var="inverter" varStatus="i">
                <div class="col-md-3">
                        <div class="panel-heading" style="padding: 10px 15px;font-size: 16px;">
                            <a href="#">
                                ${inverter.name}
                            </a>
                        </div>
                        <div class="panel-body">
                            <div class="row">
                                <div class="col-md-12">
                                    <ul class="list-group clear-list">
                                        <corsair:measurement displayAreaName="inverter-briefing" PSRID="${inverter.id}"/>
                                    </ul>
                                </div>
                                <div style="clear:both;float:left;" class="col-md-12">
                                    <corsair:measurement displayAreaName="inverter-communicate" PSRID="${inverter.id}"/>
                                </div>
                            </div>
                        </div>
                </div>
            </c:forEach>
        </div>
</body>
</html>
