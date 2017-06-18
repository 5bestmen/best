<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<html>
<head>
    <title>逆变器转化效率</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

</head>
<body>

    <%--<script src="<c:url value="/resources/dist/js/html5shiv.min.js"/>"></script>--%>
    <%--<script src="<c:url value="/resources/dist/js/respond.min.js"/>"></script>--%>

    <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
    <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
    <script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
    <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
    <script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
    <script src="<c:url value="/resources/js/generalChart.js"/>"></script>
    <script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>

    <script>
        var option = {
            title : {
                text: '逆变器转换效率'
            },
            legend: {
                textStyle:{
                    color:'white'
                },
                data:[]
            },
            toolbox: {
                show : true,
                feature : {
                    magicType : {show: true, type: ['line', 'bar']},
                    restore : {show: true}
                }
            },
            tooltip: {
                trigger: 'axis'
            },
            calculable : true,
            xAxis : [
                {
                    type : 'category',
                    boundaryGap : true,
                    axisLabel : {
                        textStyle: {
                            color:'white'
                        }
                    },
                    data : []
                }
            ],
            yAxis : [
                {
                    type : 'value',
                    axisLabel: {
                        formatter: '{value} %'
                    },
                    min: 90,
                    max: 110
                }
            ],
            series : []
        };

        var inverterOption = {
            ajax: true,
            URL: '/api/statistic',
            robin: false,
            timeSpan: 1000000,
            getConfig: function(){return {type:'inverterEfficiency'};},
            formatNumber: true,
            EchartsOption: option,
            chartContainer: 'inverterTransformEfficiency'
        };

        $(function () {
            var chart = $.fn.GeneralChart.init(inverterOption);
            chart.load();
        });
    </script>

    <%--<div id="wrapper">
      <jsp:include page="meun.jsp"/>
      &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
      <div id="page-wrapper">--%>
        <div class="row">
          <div class="col-lg-12">
            <h3 class="page-header">逆变器转化效率</h3>
              <div class="col-lg-12">
                  <div id="inverterTransformEfficiency" style="height: 600px"></div>
              </div>
          </div>
        </div>
        <%--<div class="row">--%>
          <%--<div class="col-lg-12">--%>
            <%--<div class="panel panel-info">--%>
                <%--<div class="panel-heading">--%>
                    <%--<i></i>逆变器转化效率--%>
                <%--</div>--%>
                <%--<div class="panel-body">--%>
                    <%--<div id="inverterTransformEfficiency" class="general-chart"></div>--%>
                <%--</div>--%>
            <%--</div>--%>
        <%--</div>--%>
        <%--</div>--%>
     <%-- </div>
    </div>--%>
</body>
</html>
