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
    <title>逆变器</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">

    <link href="<c:url value="/resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/laydate/need/laydate.css"/>" rel="stylesheet" type="text/css">
    <%--<link href="<c:url value="/resources/dist/laydate/skins/molv/laydate.css"/>" rel="stylesheet" type="text/css">--%>
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

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
<style>
    .list-group-item{
        border-left: 0;
    }
    .dt-grid-pager-status {
        padding: 0px;
    }
</style>
<script>
    $(function () {
        var mean = $.fn.MeanSelector.init('curve', 'general-chart', '${id}');
        mean.load();

        var event = $.fn.PSREvent.init('event', '${id}');
        event.load();
    })
</script>

<script>
    function Measurement() {
        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();
    }

    var columns = [];

    //名称
    columns.push({
        id: 'eventTime',
        title: '事项时间',
        type: 'date',
        format: 'yyyy-MM-dd hh:mm:ss',
        columnClass: 'text-left;'
    });

    columns.push({
        id: 'eventDes',
        title: '事项描述',
        type: 'string',
        columnClass: 'text-left;'
    });

    columns.push({
        id: 'eventType',
        title: '事件类型',
        type: 'string',
        columnClass: 'text-left;'
    });

    var option = {
        lang: 'zh-cn',
        ajaxLoad: true,
        loadAll: true,
        loadURL: '/InverterAlarmServlet',
        columns: columns,
        gridContainer: 'alarmTable',
        toolbarContainer: 'alarmToolBar',
        tools : '',
        pageSize: 10,
        pageSizeLimit: [10, 20, 50]
    };

    var alarmTable = $.fn.DtGrid.init(option);
    $(function () {
        alarmTable.load();
    });

    window.onload = function () {
        Measurement();

    }
</script>

<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>

    <div id="page-wrapper">--%>
<div class="row">
    <div class="col-lg-12">
        <h3>${arrayName}-${name}</h3>
    </div>
</div>
<div class="row">
    <div class="row">
        <div class="col-lg-12">
            <div class="panel-heading">
                <i class="fa fa-bar-chart-o fa-fw"></i>量测
            </div>
            <div class="panel-body">
                <%--<div>--%>
                <%--<div>--%>
                <div class="row" style="margin-bottom: 10px">
                    <div class="col-md-2 hidden-xs" style="float: left">
                        <%--<img class="img-responsive  m-b-md" src="/resources/img/inverter.png">--%>
                        <img class="img-responsive m-b-md" src="<c:url value="/resources/img/svg/inverter.svg"/>" style="width: 70%;margin-left: 40px;"/>
                    </div>
                    <div class="col-md-1 " style="float: left;margin:40px" hidden>
                        <div style=" width:90%; height:9%; background-color:#9a9a9a; border-radius:50%;">
                            <span style="padding: 20px; line-height:50px; display:block; color:#fff; text-align:center">离线</span>
                        </div>
                    </div>
                    <div class="col-md-10" style="float: left">

                        <div class="equipment first">设备编码
                            <div class="content one">${devEncode}</div>
                        </div>
                        <div class="equipment first">设备厂家
                            <div class="content one">${devFactory}</div>
                        </div>
                        <div class="equipment first">设备型号
                            <div class="content one">${devModel}</div>
                        </div>
                        <c:forEach items="${deviceFormats}" var="deviceformat" varStatus="k">
                            <div class="equipment second">${deviceformat.name}
                                <div class="content two">${deviceformat.comments}</div>
                            </div>
                        </c:forEach>
                    </div>
                </div>

                <div class="col-lg-12">
                    <div class="row">
                        <div class="col-lg-3 col-md-3">
                            <ul class="list-group clear-list">
                                <corsair:measurement displayAreaName="inverter-yc-detail1" PSRID="${id}"/>
                            </ul>
                        </div>
                        <div class="col-lg-3 col-md-3">
                            <ul class="list-group clear-list">
                                <corsair:measurement displayAreaName="inverter-yc-detail2" PSRID="${id}"/>
                            </ul>
                        </div>
                        <div class="col-lg-3 col-md-3">
                            <ul class="list-group clear-list">
                                <corsair:measurement displayAreaName="inverter-yc-detail3" PSRID="${id}"/>
                            </ul>
                        </div>
                        <div class="col-lg-3 col-md-3">
                            <ul class="list-group clear-list">
                                <corsair:measurement displayAreaName="inverter-yc-detail4" PSRID="${id}"/>
                            </ul>
                        </div>
                    </div>
                </div>
            </div>
            <div>
                <div class="row">
                    <corsair:measurement displayAreaName="inverter-yx-detail" PSRID="${id}"/>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-lg-12">
        <div class="panel-heading">
            <i class="fa fa-bar-chart-o fa-fw"></i>图表
        </div>
        <div class="panel-body" id="curve"></div>
    </div>
</div>
<c:if test="${not empty childJunctions}">
    <div class="row">
        <div class="col-lg-12">
            <div class="panel">
                <div class="panel-heading">
                    <i class="fa fa-table fa-fw"></i>电能表量测
                </div>
                <div class="panel-body">
                    <div class="row">
                        <c:forEach items="${childJunctions}" var="Junction" varStatus="i">
                            <div class="col-lg-2 col-md-2" style="padding-right: 8px; padding-left: 8px">
                                    <%--<a href="#">--%>
                                <img class="img-responsive" src="/resources/img/ammeter.jpg">
                                    <%--</a>--%>
                                <corsair:measurement displayAreaName="junction-communicate" PSRID="${Junction.id}"/>
                                <ul class="list-group clear-list m-t">
                                    <corsair:measurement displayAreaName="junction-briefing" PSRID="${Junction.id}"/>
                                </ul>
                            </div>
                        </c:forEach>
                    </div>
                </div>
            </div>
        </div>
    </div>
</c:if>
<div class="row">
    <c:forEach items="${childTransformers}" var="PackageTransformer" varStatus="i">
        <div class="col-lg-12">
            <div class="panel">
                <div class="panel-heading">
                    <i class="fa fa-bar-chart-o fa-fw"></i>逆变器9N表量测
                </div>
                <div class="panel-body">
                    <div class="row">
                        <div class="col-lg-2  hidden-xs">
                            <img class="img-responsive m-b-md m-t-md" src="/resources/img/ammeter.jpg">
                            <corsair:measurement displayAreaName="transbox-communicate" PSRID="${PackageTransformer.id}"/>
                        </div>
                        <div class="col-lg-10">
                            <div class="row">
                                <div class="col-lg-3 col-md-3">
                                    <ul class="list-group clear-list">
                                        <corsair:measurement displayAreaName="transbox-yc-detail1" PSRID="${PackageTransformer.id}"/>
                                    </ul>
                                </div>
                                <div class="col-lg-3 col-md-3">
                                    <ul class="list-group clear-list">
                                        <corsair:measurement displayAreaName="transbox-yc-detail2" PSRID="${PackageTransformer.id}"/>
                                    </ul>
                                </div>
                                <div class="col-lg-3 col-md-3">
                                    <ul class="list-group clear-list">
                                        <corsair:measurement displayAreaName="transbox-yc-detail3" PSRID="${PackageTransformer.id}"/>
                                    </ul>
                                </div>
                                <div class="col-lg-3 col-md-3">
                                    <ul class="list-group clear-list">
                                        <corsair:measurement displayAreaName="transbox-yc-detail4" PSRID="${PackageTransformer.id}"/>
                                    </ul>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </c:forEach>
</div>
<div class="row">
    <div class="col-lg-12">
        <div class="panel-heading">
            <i class="fa fa-bar-chart-o fa-fw"></i>事件
        </div>
        <div class="panel-body" id="event"></div>
    </div>
</div>
<%--</div>
</div>--%>
</body>
</html>
