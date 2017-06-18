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
    <title>环境监测仪</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/commonStyle.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>" rel="stylesheet"
          type="text/css">
    <link href="<c:url value="/resources/dist/laydate/need/laydate.css"/>" rel="stylesheet" type="text/css">
    <%--<link href="<c:url value="/resources/dist/laydate/skins/molv/laydate.css"/>" rel="stylesheet" type="text/css">--%>
    <%--<link href="dist/laydate/skins/molv/laydate.css" rel="stylesheet" type="text/css">--%>

    <%--<script src="dist/js/html5shiv.min.js"></script>--%>
    <%--<script src="dist/js/respond.min.js"></script>--%>

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


<script src="<c:url value="/resources/bower_components/dtGrid/js/jquery.dtGrid.min.js"/>"
        type="text/javascript"></script>
<script src="<c:url value="/resources/bower_components/dtGrid/i18n/zh-cn.js"/>" type="text/javascript"></script>

<style>
    .list-group-item{
        border-left: 0;
    }
</style>
<script>
    function Measurement() {
        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();
        var mean = $.fn.MeanSelector.init('curve', 'general-chart', '${id}');
        mean.load();
    }
    window.onload = function () {
        Measurement();
    }
</script>

<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>

  <div id="page-wrapper">--%>
<div class="row">
    <div class="col-lg-12">
        <h3>${arrayName}</h3>
    </div>
</div>
<div class="row">
    <div class="col-lg-12">
        <div class="panel">
            <div class="panel-heading">
                <i class="fa fa-bar-chart-o fa-fw"></i>${name}量测
            </div>
            <div class="panel-body">
                <div class="row">
                    <div class="col-lg-2">
                        <%--<img class="img-responsive" src="img/transform.png">--%>
                        <img class="img-responsive m-b-md" src="<c:url value="/resources/img/svg/weather.svg"/>"/>
                    </div>
                    <div class="col-lg-10">
                        <div class="row">
                            <div class="col-lg-6 col-md-6">
                                <ul class="list-group clear-list">
                                    <corsair:measurement displayAreaName="weather-yc-detail1" PSRID="${id}"/>
                                </ul>
                            </div>
                            <div class="col-lg-6 col-md-6">
                                <ul class="list-group clear-list">
                                    <corsair:measurement displayAreaName="weather-yc-detail2" PSRID="${id}"/>
                                </ul>
                            </div>

                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-lg-12">
                        <div class="panel">
                            <div class="panel-heading">
                                <i class="fa fa-bar-chart-o fa-fw"></i>图表
                            </div>
                            <div class="panel-body" id="curve"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<%--</div>
</div>--%>
</body>
</html>
