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
    $(function () {
        $('.select-icon').click(function () {
            var button = $(this).find('i');
            button.toggleClass('fa-chevron-right').toggleClass('fa-chevron-left');
            $(".pvarray-select-box").toggleClass("show");
        });
        $(".images-content").hide();
        $(".images-wrapper").hover(function () {
            $(".images-content", this).slideToggle(500);
        });
    });

</script>

<script>
    $(function() {
        var mean = $.fn.MeanSelector.init('curve', 'general-chart', '${PVArrayID}');
        mean.load();
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
<script type="text/javascript">

    var columns = [];

    //名称
    columns.push({
        id: 'name',
        title: '汇流箱',
        type: 'string',
        headerClass:'text-center',
        columnClass: 'text-center'
    });

    //各路电流
    for (var i = 1; i <= 16; i++) {
        columns.push({
            id: 'track' + i,
            title: i + '路',
            type: 'object',
            format: '###.##',
            headerClass:'text-center',
            columnClass: 'text-center'
        });
    }

    //平均电流
    columns.push({
        id: 'average',
        title: '平均',
        type: 'number',
        format: '###.##',
        headerClass:'text-center',
        columnClass: 'text-center'
    });

    //最大电流
    columns.push({
        id: 'max',
        title: '最大',
        type: 'number',
        format: '###.##',
        headerClass:'text-center',
        columnClass: 'text-center'
    });

    var option = {
        robin:true,
        timeSpan: 5000,
        URL: '/MatrixServlet?test=a',
        TableContainer : 'jungleTable',
        captain: {
            show: false,
            text: '<h2>汇流箱数据</h2>'
        },
        objectFunction: function (td, data, table, response, row, col){
            td.innerText = data.data;
            if (data.flag == true) {
                $(td).addClass('red-bg');
            } else {
                $(td).removeClass('red-bg');
            }
        },
        columns: columns,
        config:{id:'${PVArrayID}'}
    };


    $(function () {
//        var table = $.fn.RobinTable.init(option);
//        table.load();
    });

</script>

<%--<div id="wrapper">--%>
    <%--<jsp:include page="meun.jsp"/>--%>

    <%--<div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-12">
                <h3>${PVArrayName}</h3>
            </div>
        </div>
        <div class="row">
            <div class="col-md-12">
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-3">
                                <div class="row">
                                    <div class="col-md-12">
                                        <img class="img-responsive" src="<c:url value="/resources/img/cyg.jpg"/>">
                                    </div>
                                    <div class="col-md-12">
                                        <ul class="list-group clear-list m-t">
                                            <corsair:measurement displayAreaName="pvarray-yc" PSRID="${packageTransformer.id}"/>

                                        </ul>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-9">
                                <div id="curve">
                                </div>
                            </div>
                        </div>
                    </div>
            </div>
        </div>
        <div class="row" >
            <c:forEach items="${inverters}" var="inverter" varStatus="i">
                <div class="col-md-4">
                        <div class="panel-heading" style="padding: 10px 15px;font-size: 16px;">
                            <a href="inverter?inverterID=${inverter.id}">
                            逆变器${i.count}
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
            <%--深圳长园集团光伏项目，5个逆变器和1个汇流箱的采集装置,采集装置接着逆变器后面排--%>
            <c:if test="${not empty packageTransformer}">
                <div class="col-md-4">
                        <div class="panel-heading" style="padding: 10px 15px;font-size: 16px;">
                            <a href="packageTransformer?packageTransformerID=${packageTransformer.id}">
                           并网计量柜
                           </a>
                        </div>
                        <div class="panel-body">
                            <div class="row">
                                <div class="col-md-12">
                                    <ul class="list-group clear-list">
                                        <corsair:measurement displayAreaName="transbox-briefing" PSRID="${packageTransformer.id}"/>
                                    </ul>
                                </div>
                                <div style="clear:both;float:left; " class="col-md-12">
                                    <corsair:measurement displayAreaName="transbox-communicate" PSRID="${packageTransformer.id}"/>
                                </div>
                            </div>
                        </div>
                </div>
            </c:if>
            <c:if test="${not empty weatherInstrument}">
                <div class="col-md-4">
                    <div class="panel-heading" style="padding: 10px 15px;font-size: 16px;">
                        <a href="weatherInstrument?weatherInstrumentID=${weatherInstrument.id}">
                            环境监测仪
                        </a>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <div class="col-md-12">
                                <ul class="list-group clear-list">
                                    <corsair:measurement displayAreaName="weatherInstrument-briefing" PSRID="${weatherInstrument.id}"/>
                                </ul>
                            </div>
                            <div style="clear:both;float:left; " class="col-md-12">
                                <corsair:measurement displayAreaName="weatherInstrument-communicate" PSRID="${weatherInstrument.id}"/>
                            </div>
                        </div>
                    </div>
                </div>
            </c:if>
        </div>
 <%--   </div>
</div>>--%>
</body>
</html>
