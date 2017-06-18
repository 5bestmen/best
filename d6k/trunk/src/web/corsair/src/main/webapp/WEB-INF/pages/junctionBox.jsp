<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/7/10
  Time: 11:07
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib uri="http://corsair.com" prefix="corsair" %>
<html>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <title>汇流箱</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/laydate/need/laydate.css"/>" rel="stylesheet" type="text/css">

</head>
<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
<script src="<c:url value="/resources/dist/js/measSelector.js"/>"></script>
<script src="<c:url value="/resources/dist/js/psrevent.js"/>"></script>

<script src="<c:url value="/resources/bower_components/dtGrid/js/jquery.dtGrid.min.js"/>" type="text/javascript"></script>
<script src="<c:url value="/resources/bower_components/dtGrid/i18n/zh-cn.js"/>" type="text/javascript"></script>

<script>
    $(function () {
        var mean = $.fn.MeanSelector.init('curve', 'general-chart', '${id}');
        mean.load();

        var event = $.fn.PSREvent.init('event', '${id}');
        event.load();
    })
</script>

<script>
    function Measurement()
    {
        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();
    }
    window.onload=function(){Measurement();}
</script>

<div id="wrapper">
    <jsp:include page="meun.jsp"/>

    <div id="page-wrapper">
        <div class="row">
            <div class="col-lg-12">
                <h3>${arrayName}-${name}</h3>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-12">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <i class="fa fa-bar-chart-o fa-fw"></i>量测
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-lg-2 hidden-xs">
                                <img class="img-responsive" src="/resources/img/junctionbox.jpg">
                            </div>
                            <div class="col-lg-2">
                                <corsair:measurement displayAreaName="junction-status" PSRID="${id}"/>
                            </div>
                            <div class="col-lg-8">
                                <div class="row">
                                    <div class="col-lg-4 col-md-6">
                                        <ul class="list-group clear-list">
                                            <corsair:measurement displayAreaName="junction-yc-detail1" PSRID="${id}"/>
                                        </ul>
                                    </div>
                                    <div class="col-lg-4 col-md-6">
                                        <ul class="list-group clear-list">
                                            <corsair:measurement displayAreaName="junction-yc-detail2" PSRID="${id}"/>
                                        </ul>
                                    </div>
                                    <div class="col-lg-4 col-md-6">
                                        <ul class="list-group clear-list">
                                            <corsair:measurement displayAreaName="junction-yc-detail3" PSRID="${id}"/>
                                        </ul>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div>
                            <div class="row">
                                <corsair:measurement displayAreaName="junction-yx-detail" PSRID="${id}"/>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-12">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <i class="fa fa-bar-chart-o fa-fw"></i>图表
                    </div>
                    <div class="panel-body" id="curve"></div>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-12">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <i class="fa fa-bar-chart-o fa-fw"></i>事件
                    </div>
                    <div class="panel-body" id="event"></div>
                </div>
            </div>
        </div>
    </div>
</div>
</body>
</html>
