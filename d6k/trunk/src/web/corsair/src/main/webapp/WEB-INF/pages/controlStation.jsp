<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>

<!DOCTYPE html>
<html lang="zh_CN">
<head>
    <meta charset="UTF-8">
    <title>光伏厂站</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/laydate/need/laydate.css"/>" rel="stylesheet" type="text/css">

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
    <link href="<c:url value="/resources/dist/js/html5shiv.min.js"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/js/respond.min.js"/>" rel="stylesheet" type="text/css">

    <![endif]-->
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

<%
    session.getAttributeNames();
%>
<div id="wrapper">
    <jsp:include page="meun.jsp"/>

    <div id="page-wrapper">
        <div class="row">
            <div class="col-lg-12">
                <h3>光伏厂站
                    <span class="alignright">${sessionScope.APP_NAME}</span>
                </h3>
            </div>
        </div>
        <h5></h5>
        <div class="row">
            <div class="col-md-6">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-4">
                                <div class="widget-state style1 green-bg">
                                    <div class="text-center">
                                        <h5 class="font-bold">达坂城光伏电站</h5>
                                    </div>
                                </div>
                                <a href="index?indexID=jnpdc">
                                    <img class="img-responsive" src="/resources/img/dbc.png">
                                </a>
                                <%--<corsair:measurement displayAreaName="transbox-communicate" PSRID="${packageTransformer.id}"/>--%>
                            </div>
                            <div class="col-md-8">
                                <ul class="list-group clear-list">
                                    <li class="list-group-item fist-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">100</em><i> MW</i>
                                        </span>
                                        <span class="label label-info">Power</span>实时功率
                                    </li>
                                    <li class="list-group-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">56</em><i> kWh</i>
                                        </span>
                                        <span class="label navy-bg">Day</span>日发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="6" data-measurementtype="2" data-magic="">1569</em><i> kWh</i>
                                        </span>
                                        <span class="label yellow-bg">Month</span>月发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="3" data-measurementtype="2" data-magic="">203</em><i> MWh</i>
                                        </span>
                                        <span class="label blue-bg">Year</span>年发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="25" data-measurementtype="2" data-magic="">637</em><i> MWh</i>
                                        </span>
                                        <span class="label darkgray-bg">Total</span>累计发电量
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-md-6">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-4">
                                <div class="widget-state style1 green-bg">
                                    <div class="text-center">
                                        <h5 class="font-bold">米东光伏电站</h5>
                                    </div>
                                </div>
                                <a href="index?indexID=smhon">
                                    <img class="img-responsive" src="/resources/img/md.png">
                                </a>
                                <%--<corsair:measurement displayAreaName="transbox-communicate" PSRID="${packageTransformer.id}"/>--%>
                            </div>
                            <div class="col-md-8">
                                <ul class="list-group clear-list">
                                    <li class="list-group-item fist-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">100</em><i> MW</i>
                                        </span>
                                        <span class="label label-info">Power</span>实时功率
                                    </li>
                                    <li class="list-group-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">56</em><i> kWh</i>
                                        </span>
                                        <span class="label navy-bg">Day</span>日发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="6" data-measurementtype="2" data-magic="">1569</em><i> kWh</i>
                                        </span>
                                        <span class="label yellow-bg">Month</span>月发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="3" data-measurementtype="2" data-magic="">203</em><i> MWh</i>
                                        </span>
                                        <span class="label blue-bg">Year</span>年发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="25" data-measurementtype="2" data-magic="">637</em><i> MWh</i>
                                        </span>
                                        <span class="label darkgray-bg">Total</span>累计发电量
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-md-6">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-4">
                                <div class="widget-state style1 green-bg">
                                    <div class="text-center">
                                        <h5 class="font-bold">沙依巴克光伏电站</h5>
                                    </div>
                                </div>
                                <a href="index?indexID=eeshl">
                                    <img class="img-responsive" src="/resources/img/sybk.png">
                                </a>
                                <%--<corsair:measurement displayAreaName="transbox-communicate" PSRID="${packageTransformer.id}"/>--%>
                            </div>
                            <div class="col-md-8">
                                <ul class="list-group clear-list">
                                    <li class="list-group-item fist-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">100</em><i> MW</i>
                                        </span>
                                        <span class="label label-info">Power</span>实时功率
                                    </li>
                                    <li class="list-group-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">56</em><i> kWh</i>
                                        </span>
                                        <span class="label navy-bg">Day</span>日发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="6" data-measurementtype="2" data-magic="">1569</em><i> kWh</i>
                                        </span>
                                        <span class="label yellow-bg">Month</span>月发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="3" data-measurementtype="2" data-magic="">203</em><i> MWh</i>
                                        </span>
                                        <span class="label blue-bg">Year</span>年发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="25" data-measurementtype="2" data-magic="">637</em><i> MWh</i>
                                        </span>
                                        <span class="label darkgray-bg">Total</span>累计发电量
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-md-6">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-4">
                                <div class="widget-state style1 green-bg">
                                    <div class="text-center">
                                        <h5 class="font-bold">天山光伏电站</h5>
                                    </div>
                                </div>
                                <a href="index?indexID=vbfpp">
                                    <img class="img-responsive" src="/resources/img/ts.png">
                                </a>
                                <%--<corsair:measurement displayAreaName="transbox-communicate" PSRID="${packageTransformer.id}"/>--%>
                            </div>
                            <div class="col-md-8">
                                <ul class="list-group clear-list">
                                    <li class="list-group-item fist-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">100</em><i> MW</i>
                                        </span>
                                        <span class="label label-info">Power</span>实时功率
                                    </li>
                                    <li class="list-group-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">56</em><i> kWh</i>
                                        </span>
                                        <span class="label navy-bg">Day</span>日发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="6" data-measurementtype="2" data-magic="">1569</em><i> kWh</i>
                                        </span>
                                        <span class="label yellow-bg">Month</span>月发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="3" data-measurementtype="2" data-magic="">203</em><i> MWh</i>
                                        </span>
                                        <span class="label blue-bg">Year</span>年发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="25" data-measurementtype="2" data-magic="">637</em><i> MWh</i>
                                        </span>
                                        <span class="label darkgray-bg">Total</span>累计发电量
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-md-6">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-4">
                                <div class="widget-state style1 green-bg">
                                    <div class="text-center">
                                        <h5 class="font-bold">头屯河光伏电站</h5>
                                    </div>
                                </div>
                                <a href="index?indexID=goej9">
                                    <img class="img-responsive" src="/resources/img/tch.png">
                                </a>
                                <%--<corsair:measurement displayAreaName="transbox-communicate" PSRID="${packageTransformer.id}"/>--%>
                            </div>
                            <div class="col-md-8">
                                <ul class="list-group clear-list">
                                    <li class="list-group-item fist-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">100</em><i> MW</i>
                                        </span>
                                        <span class="label label-info">Power</span>实时功率
                                    </li>
                                    <li class="list-group-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">56</em><i> kWh</i>
                                        </span>
                                        <span class="label navy-bg">Day</span>日发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="6" data-measurementtype="2" data-magic="">1569</em><i> kWh</i>
                                        </span>
                                        <span class="label yellow-bg">Month</span>月发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="3" data-measurementtype="2" data-magic="">203</em><i> MWh</i>
                                        </span>
                                        <span class="label blue-bg">Year</span>年发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="25" data-measurementtype="2" data-magic="">637</em><i> MWh</i>
                                        </span>
                                        <span class="label darkgray-bg">Total</span>累计发电量
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-md-6">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-4">
                                <div class="widget-state style1 green-bg">
                                    <div class="text-center">
                                        <h5 class="font-bold">新市光伏电站</h5>
                                    </div>
                                </div>
                                <a href="index?indexID=on9ma">
                                    <img class="img-responsive" src="/resources/img/xs.png">
                                </a>
                                <%--<corsair:measurement displayAreaName="transbox-communicate" PSRID="${packageTransformer.id}"/>--%>
                            </div>
                            <div class="col-md-8">
                                <ul class="list-group clear-list">
                                    <li class="list-group-item fist-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">100</em><i> MW</i>
                                        </span>
                                        <span class="label label-info">Power</span>实时功率
                                    </li>
                                    <li class="list-group-item">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="22" data-measurementtype="2" data-magic="">56</em><i> kWh</i>
                                        </span>
                                        <span class="label navy-bg">Day</span>日发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="6" data-measurementtype="2" data-magic="">1569</em><i> kWh</i>
                                        </span>
                                        <span class="label yellow-bg">Month</span>月发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="3" data-measurementtype="2" data-magic="">203</em><i> MWh</i>
                                        </span>
                                        <span class="label blue-bg">Year</span>年发电量
                                    </li>
                                    <li class="list-group-item ">
                                        <span class="pull-right">
                                            <em class="measurement" data-templateindex="25" data-measurementtype="2" data-magic="">637</em><i> MWh</i>
                                        </span>
                                        <span class="label darkgray-bg">Total</span>累计发电量
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</body>
</html>
